#include <sstream>
#include "message_box.h"
#include <string.h>
#include <error.h>
#include <unistd.h>

#define QOS           2
#define TIMEOUT       10000L

#define MSGQ_ELE_CNT  10
#define MSGQ_ELE_SIZE 4096

using namespace std;

MessageBox::MessageBox(std::string client_id, std::string host, uint16_t port) 
    : m_id(client_id), m_host(host), m_port(port), m_conn(false), m_lost_conn(false) {

    // MQTT IPC Message
    stringstream addr;
    addr << "tcp://" << host << ":" << port;

    MQTTClient_create(&client, addr.str().c_str(), client_id.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // Boost IPC Message
    m_que_name = "/MSGQ-MessageBox-" + client_id;
    mq_unlink(m_que_name.c_str());

    struct mq_attr attr;
    attr.mq_flags = 0;  
    attr.mq_maxmsg = MSGQ_ELE_CNT;  
    attr.mq_msgsize = MSGQ_ELE_SIZE;  
    attr.mq_curmsgs = 0;  
    m_msgq_recver = mq_open(m_que_name.c_str(), O_CREAT | O_RDONLY, 0644, &attr);
    if (m_msgq_recver < 0)
        printf("failed to create recver message queueu %s, %d, %s", m_que_name.c_str(), errno, strerror(errno));
    m_msgq_sender = mq_open(m_que_name.c_str(), O_WRONLY); 
    if (m_msgq_sender < 0)
        printf("failed to create sender message queueu %s, %d, %s", m_que_name.c_str(), errno, strerror(errno));
}

MessageBox::~MessageBox() {
    MQTTClient_destroy(&client);

    if (m_msgq_sender > 0)
        mq_close(m_msgq_sender);
    if (m_msgq_recver > 0)
        mq_close(m_msgq_recver);
    mq_unlink(m_que_name.c_str());
}

bool MessageBox::connect() {
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, this, MessageBox::conn_lost, MessageBox::on_message, NULL);

    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d", rc);
        return false;
    }
    printf("connect witch mqtt server");

    m_conn = true;

    return true;
}

bool MessageBox::subscribe(const string &topic) {
    if (! m_conn)
        return false;

    MQTTClient_subscribe(client, topic.c_str(), QOS);
    printf("subcribe topic: %s with qos %d", topic.c_str(), QOS);

    return true;
}

bool MessageBox::disconnect() {
    printf("disconnect mqtt server");

    MQTTClient_disconnect(client, TIMEOUT);
    m_conn = false;
    char notify[] = "{diconnect-notify}";

    /* send insignificant data to wakeup message receiver */
    if (mq_send(m_msgq_sender, (const char *)&notify, strlen(notify), 0) < 0) {
        printf("failed to send disconnected message notify");
        return false;
    }

    return true;
}

int MessageBox::on_message(void *context, char *topic, int len, MQTTClient_message *message) {
    MessageBox *thiz = (MessageBox *)context;
    if (mq_send(thiz->m_msgq_sender, (const char *)message->payload, message->payloadlen, 0) < 0) {
        printf("failed to send message %s", (const char *)message->payload);
        return 1; // TODO
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topic);

    return 1;
}

void MessageBox::conn_lost(void *context, char *cause) {
    printf("Connection lost: %s", cause);
    MessageBox *thiz = (MessageBox *)context;
    thiz->m_lost_conn = true;
}

/*
 * return false when send failed
 */
bool MessageBox::send(std::shared_ptr<Message> msg) {
    string json = msg->json();

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (void *)json.c_str();
    pubmsg.payloadlen = json.length();
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_deliveryToken token;
    if (MQTTCLIENT_SUCCESS != MQTTClient_publishMessage(client, msg->m_topic.c_str(), &pubmsg, &token)) {
        printf("failed to send message: %s", json.c_str());
        return false;
    }
    if (MQTTCLIENT_SUCCESS != MQTTClient_waitForCompletion(client, token, TIMEOUT)) {
        printf("failed to wait message send complete: %s", json.c_str());
        return false;
    }

    printf("%s published: %s on %s",  m_id.c_str(), json.c_str(), msg->m_topic.c_str());

    return true;
}

std::shared_ptr<Message> MessageBox::recv() {
again:
    char json[MSGQ_ELE_SIZE + 1];
    int ret = mq_receive(m_msgq_recver, json, MSGQ_ELE_SIZE, NULL);
    if (ret < 0) {
        std::shared_ptr<MessageInvalid> msg(new MessageInvalid());
        msg->code = MessageInvalid::ERR_ID_INRAL;
        msg->what = "...";
        return msg;
    }
    
    json[ret] = '\0'; /* only accept json string */
    if (strlen(json) == 0) {
        printf("receive 1 bytes unexpectly, maybe there is a bug, drop data and try read again");
        goto again;
    }

    /* check connection */
    if (! m_conn || m_lost_conn) {
        std::shared_ptr<MessageInvalid> msg(new MessageInvalid());
        msg->code = MessageInvalid::ERR_ID_DISCONN;
        msg->what = "disconnected";

        if (m_lost_conn) {
            msg->code = MessageInvalid::ERR_ID_LOST_CONN;
            msg->what = "lost connnection";
        }
        return msg;
    }
    
    return MessageFactory::create(json);
}

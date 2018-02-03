#ifndef _MESSAGE_BOX_H_
#define _MESSAGE_BOX_H_

#include <stdint.h>
#include <string>
#include <memory>
#include <MQTTClient.h>
#include "message.h"
#include <mqueue.h>

class MessageBox {
public:
    MessageBox(std::string client_id, std::string host = "localhost", uint16_t port = 1883);
    ~MessageBox();

    bool subscribe(const std::string &topic);
    bool send(std::shared_ptr<Message> msg);
    std::shared_ptr<Message> recv();

    bool connect();
    bool disconnect();

private:
    static int on_message(void *context, char *topic, int len, MQTTClient_message *message);
    static void conn_lost(void *context, char *cause);

private:
    std::string m_id;
    std::string m_host;
    uint16_t m_port;

    volatile bool m_conn;
    volatile bool m_lost_conn;

    MQTTClient client;

    std::string m_que_name;
    mqd_t m_msgq_sender;
    mqd_t m_msgq_recver;
};

#endif

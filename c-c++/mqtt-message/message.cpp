#include <stdio.h>
#include <stdint.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include "message.h"

using namespace std;
using namespace rapidjson;

/*
 * class Message;
 */

const string Message::TOPIC_INVALID     = "/invalid";
const string Message::TOPIC_HEARTBEAT   = "/local_heartbeat";

template <typename Writer>
void Message::serialize(Writer &writer) const {
    writer.String("id");
    writer.Int(m_id);
}

/*
 * class MessageFactory;
 */

std::shared_ptr<Message> MessageFactory::create(const char *json) {
    printf("json: %s", json);

    Document doc;
    doc.Parse(json);

    if(! doc.HasMember("id")) {
        printf("invalid json for incoming message: %s", json);

        MessageInvalid *msg = new MessageInvalid();
        msg->code = MessageInvalid::ERR_ID_UNKNOW;
        msg->what = "recive invalid json for message";

        return std::shared_ptr<Message>(msg);
    }

    uint32_t id = doc["id"].GetInt();

    switch (id) {
    case Message::ID_HEARTBEAT: {
        printf("create /local_heartbeat message");
        MessageHeartbeat *msg = new MessageHeartbeat();

        if(doc.HasMember("name")) {
            msg->m_name = doc["name"].GetString();
        }

        return std::shared_ptr<Message>(msg);
        }
    default:
        printf("unknow message");
        MessageInvalid *msg = new MessageInvalid();
        msg->code = MessageInvalid::ERR_ID_INRAL;
        msg->what = "unsupported json for message";

        return std::shared_ptr<Message>(msg);
    }
}

/*
 * class MessageInvalid;
 */

MessageInvalid::MessageInvalid() : Message(Message::ID_INVALID, Message::TOPIC_INVALID) { }

std::string MessageInvalid::json() {
    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    serialize(writer);
    return sb.GetString();
}

template <typename Writer>
void MessageInvalid::serialize(Writer &writer) const {
    writer.StartObject();

    Message::serialize(writer);

    writer.String("what");
    writer.String(what.c_str(), static_cast<SizeType>(what.length()));

    writer.EndObject();
}

/*
 * class MessageHeartbeat;
 */

MessageHeartbeat::MessageHeartbeat()
        : Message(Message::ID_HEARTBEAT, Message::TOPIC_HEARTBEAT),
          index(0) {
}

std::string MessageHeartbeat::json() {
    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    serialize(writer);
    return sb.GetString();
}

template <typename Writer>
void MessageHeartbeat::serialize(Writer &writer) const {
    writer.StartObject();

    Message::serialize(writer);

    writer.String("index");
    writer.Int(index);
    writer.String("name");
    writer.String(m_name.data());

    writer.EndObject();
}

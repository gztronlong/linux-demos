/*
 * Important:
 * don't forget to expand the message element's size in ipc queue, 
 * if you want use Message Family define here to transform large data.
 */

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

class Message {
public:
    typedef enum {
        ID_INVALID      = 0,
        ID_HEARTBEAT    = 1,
    } MsgID;

    static const std::string TOPIC_INVALID;
    static const std::string TOPIC_HEARTBEAT;

public:
    Message(uint32_t id, std::string topic) : m_id(id), m_topic(topic) { }

public:
    virtual ~Message() { }

    virtual std::string json() = 0;

protected:
    template <typename Writer>
    void serialize(Writer &writer) const;

public:
    int m_id;
    std::string m_topic;
};

class MessageFactory {
public:
    static std::shared_ptr<Message> create(const char *json);
};

class MessageInvalid : public Message {
public:
    typedef enum {
        ERR_ID_UNKNOW = 0,
        ERR_ID_INRAL,
        ERR_ID_LOST_CONN,
        ERR_ID_DISCONN,
    } MsgID;

public:
    MessageInvalid();

    std::string json();

protected:
    template <typename Writer>
    void serialize(Writer &writer) const;

public:
    uint32_t code;
    std::string what;
};

class MessageHeartbeat : public Message {
public:
    MessageHeartbeat();

    std::string json();

protected:
    template <typename Writer>
    void serialize(Writer &writer) const;

public:
    int index;
    std::string m_name;
};

#endif

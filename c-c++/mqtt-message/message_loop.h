#ifndef _MESSAGE_LOOP_
#define _MESSAGE_LOOP_

#include <pthread.h>
#include "message.h"
#include "message_box.h"
#include <memory>

class MessageHandler {
public:
    MessageHandler() { }
    virtual ~MessageHandler() { }

    virtual bool handle(std::shared_ptr<Message> msg) = 0;
};

class MessageLoop {
public:
    MessageLoop(std::shared_ptr<MessageBox> box, std::shared_ptr<MessageHandler> handler);
    ~MessageLoop();

public:
    bool start();
    bool stop();

private:
    static void *loop(void *arg);

private:
    std::shared_ptr<MessageBox> m_box;    
    std::shared_ptr<MessageHandler> m_hander;    

    pthread_t m_tid;
    volatile bool m_quit;
};

#endif

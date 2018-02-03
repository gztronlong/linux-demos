#include "message_loop.h"

MessageLoop::MessageLoop(std::shared_ptr<MessageBox> box, std::shared_ptr<MessageHandler> handler)
    : m_box(box), m_hander(handler), m_quit(false) { }

MessageLoop::~MessageLoop() { 
    printf("waiting for message loop had stop done");
    pthread_join(m_tid, NULL);
    printf("message loop had stop done");
}

bool MessageLoop::start() {
    printf("start message loop thread");
    pthread_create(&m_tid, NULL, loop, this);  

    return true;
}

bool MessageLoop::stop() {
    m_quit = true;
    return true;
}

void *MessageLoop::loop(void *arg) {
    printf("enter message loop");

    MessageLoop *thiz = (MessageLoop *)arg;
    while (! thiz->m_quit) {
        printf("waiting for message");
        std::shared_ptr<Message> msg = thiz->m_box->recv();
        if (! thiz->m_hander->handle(msg)) {
            printf("handler return false, exit message recving loop");
            break;
        }
    }

    printf("leave message loop");

    return NULL;
}

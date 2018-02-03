#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include "message_box.h"

volatile int g_quit = 0;
MessageBox *msg_box = NULL;

void *heartbeat_loop(void *arg) {
    MessageBox *msg_box = (MessageBox *)arg;

    printf("enter heartbeat loop\n");

    while (! g_quit) {
        std::shared_ptr<MessageHeartbeat> msg(new MessageHeartbeat());    
        msg_box->send(msg);
        sleep(1);
    }

    printf("leave heartbeat loop\n");

    return NULL;
}

static void sigint_handler(int signo) {
    printf("ctrl + c ...\n");
    g_quit = 1;
    msg_box->disconnect();
}

int main(int argc, char **argv) {
    signal(SIGINT, sigint_handler);

    msg_box = new MessageBox(argv[1]);
    msg_box->connect();

    pthread_t tid;
    pthread_create(&tid, NULL, heartbeat_loop, msg_box);  

    while (! g_quit) {
        printf("waiting for message\n");
        std::shared_ptr<Message> msg = msg_box->recv();
        if (msg->m_id == Message::ID_INVALID) {
            MessageInvalid *msg_bad = (MessageInvalid *)msg.get();
            printf("invalid message: %s\n", msg_bad->what.c_str());
        }
    }

    printf("waiting for heartebeat loop done\n");
    pthread_join(tid, NULL);

    return 0;
}

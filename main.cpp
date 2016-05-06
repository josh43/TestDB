#include <iostream>
#include <signal.h>
#include <limits>
#ifdef __APPLE__
    #ifndef TARGET_OS_MAC
        //#include "Server.h"
    #endif
#endif 
#include "SimpleBTree.h"
using namespace std;
static Server * serv = nullptr;

void signal_callback_handler(int signum) {

    if(serv != nullptr){
        printf("Stopping the server\n");
        pthread_mutex_lock(&LISTENING_LOCK);
        serv->stop();
        pthread_mutex_unlock(&LISTENING_LOCK);

    }
    printf("Caught signal %d\n",signum);

}
int main(int argc, char * argv[]) {


    SimpleBTree btree;
    btree.insert(3);
    btree.insert(2);
    btree.insert(5);
    btree.insert(1);
    btree.insert(12);
    btree.insert(4);
    btree.insert(10);
    SimpleBTree::printTree(&btree);
    signal(SIGINT,signal_callback_handler);

    struct sockaddr_un addr;
    int sfd;

    int ret;

    /*
           * In case the program exited inadvertently on the last run,
           * remove the socket.
           */

    if(remove(SOCKET_NAME) == -1 && errno != ENOENT){
        perror("error on removing socket path\n");
    }

    if((sfd= socket(AF_UNIX,SOCK_STREAM,0)) == -1){
        perror("On creating socket file descriptor");
        exit(EXIT_FAILURE);
    }


    memset(&addr,0,sizeof(sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = bind(sfd, (const struct sockaddr *) &addr,
               sizeof(struct sockaddr_un));
    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
/*
            * Prepare for accepting connections. The backlog size is set
            * to 20. So while one request is being processed other requests
            * can be waiting.
            */
    ret = listen(sfd,20);

    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    serv = new Server(100,{0,DEFAULT_TIMEOUT},sfd);
    serv->beginListening();
    //printf("Listening on port : d\n",PORT_NUM);

    return 0;
}
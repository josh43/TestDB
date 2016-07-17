//
// Created by neo on 5/3/16.
//

#ifndef TESTDATABASE_SERVER_H
#define TESTDATABASE_SERVER_H
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <string>
#include "Server.h"
#define DEFAULT_CLIENTS 5000
#define DEFAULT_TIMEOUT 200000 // = 200 millis

static const int BUF_SIZE = 1024;
static const int PORT_NUM = 30;
static const char * SOCKET_NAME = "/tmp/JDB";
pthread_mutex_t LISTENING_LOCK = PTHREAD_MUTEX_INITIALIZER;

signal(SIGINT,signal_callback_handler);

struct sockaddr_un addr;
int sfd;

int ret;

/* helper code to add


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

ret = listen(sfd,20);

if (ret == -1) {
perror("listen");
exit(EXIT_FAILURE);
}
serv = new Server(100,{0,DEFAULT_TIMEOUT},sfd);
serv->beginListening();
//printf("Listening on port : d\n",PORT_NUM);
*/



using namespace std;

class Server{
public:
    Server(unsigned int MAX_CONNECTIONS,
             timespec time,
    int fileDesc):MAX_CLIENTS(MAX_CONNECTIONS),
                    timeOut(time),
                    myFD(fileDesc)
    {
    }
    void stop(){
        LISTEN = false;
    }
    void beginListening(){
        LISTEN = true;

        if(myFD < 0){
            perror("Socket file descriptor < 0\n");
            exit(EXIT_FAILURE);
        }
        listen();
    }
private:
    void listen() {
        FILE * log = NULL;
        log = fopen("log.txt","a+");
        if(log == NULL){
            printf("Could not open log file\n");
            exit(EXIT_FAILURE);
        }
        time_t rawtime;
        struct tm * timeinfo;

        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        // First line will be
        // "TotalClientsServed" unsigned "BytesRead" unsigned "BytesSent" unsigned
        fseek(log, 0L, SEEK_END);
        int sz = ftell(log);
        if(sz == 0){
            fseek(log,0,SEEK_SET);
            fprintf(log,"TotalClientsServed 0 BytesRead 0 BytesSent 0\n");
        }
        fseek(log,0,SEEK_SET);
        char dbuf[256];
        unsigned int clientsHandled = 1;
        unsigned int bytesRead = 1;
        unsigned int bytesSent = 1;

        fscanf(log,"%s %u %s %u %s %u\n",dbuf,&clientsHandled,dbuf,&bytesRead,dbuf,&bytesSent);
        printf("Clients handles %u Bytes REad %u Bytes written %u",clientsHandled,bytesRead,bytesSent);
        fprintf(log,"Database Server Started up on %s \n",asctime (timeinfo));


        struct pollfd  socket;
        memset(&socket,0,sizeof(pollfd));
        socket.fd = myFD;
        socket.events = POLLHUP | POLLRDHUP | POLLIN;

        ssize_t numRead;
        char buf[BUF_SIZE];
        memset(&buf,0,sizeof(buf));
        int data_socket;
        int ret(0);
        int result;
        pthread_mutex_lock(&LISTENING_LOCK);
        while(LISTEN) {
            pthread_mutex_unlock(&LISTENING_LOCK);
            /* Wait for incoming connection. */
            result = ppoll(&socket,1,&timeOut,NULL);
            if(result > 0) {


                data_socket = accept(myFD, NULL, NULL);
                if (ret == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                struct pollfd  clientSocket;
                memset(&clientSocket,0,sizeof(pollfd));
                clientSocket.fd = data_socket;
                clientSocket.events = POLLHUP | POLLRDHUP | POLLIN;
                int totalRead = 0;
                for (; totalRead < BUF_SIZE-1; totalRead += ret) {
                    result = ppoll(&clientSocket, 1,NULL, NULL);
                    if (result > 0) {
                        /* Wait for next data packet. */

                        ret = read(data_socket, (buf + totalRead), BUF_SIZE);
                        if (ret == 0) {
                            // close connection
                            break;
                        }
                        if (ret == -1) {
                            perror("read");
                            printf("Closing client via request\n");
                            close(data_socket);
                            exit(EXIT_FAILURE);
                        }

                        /* Ensure buffer is 0-terminated. */
                        // All commands will be  1024 bytes...
                        buf[BUF_SIZE - 1] = 0;


                        if(strcmp(buf,"WRITE") == 0){
                            handleWrite(clientSocket);
                        }else if(strcmp(buf,"READ")){
                            handleRead(clientSocket);
                        }else {
                            write(data_socket,buf,BUF_SIZE);

                            printf("Client said %s \n", buf);
                        }
                        /* Handle commands. */


                    }

                    clientSocket.events = POLLHUP | POLLRDHUP | POLLIN;

                }
                // handle request

            }else if(result == 0){

            }else{
                // error
                perror("On trying to poll the descriptor\n");
            }
            //write(data_socket,buf,BUF_SIZE);

            pthread_mutex_lock(&LISTENING_LOCK);
        }
        fclose(log);

    }

    void handleWrite(pollfd & pfd){
        pfd.events = POLLHUP | POLLRDHUP | POLLIN;
        string dataToWrite;
        int res;
        char response[BUF_SIZE];
        const char * errMessage = "ERROR failed to write\n";
        strcpy(response,errMessage);
        int numLeftToRead = BUF_SIZE;
        // 3 millis
        timespec t = {0,3000};
        while(res = (ppoll(&pfd,1,&t,NULL) >0) && numLeftToRead > 0){
            if(pfd.revents & (POLLERR | POLLHUP | POLLRDHUP)){
                // done
                    printf("Closing socket via request\n");
                    if (pfd.revents & POLLERR) {
                        if(dataToWrite.size() == 0){
                            printf("Error the client closed the socket prematurely with nothing to write!\n");
                        }
                    }
                    if (pfd.revents & POLLHUP) {
                        if(dataToWrite.size() == 0){
                            printf("Error the client closed the socket prematurely with nothing to write!\n");

                        }
                    }
                return;
            } else{
                // valid
                read(pfd.fd,response,BUF_SIZE);
                dataToWrite.append(response);
                memset(response,0,sizeof(response));
                pfd.events = POLLHUP | POLLRDHUP | POLLIN;

            }
        }
        printf("Read the request to write %s \n",dataToWrite.c_str());
    }
    void handleRead(pollfd & pfd){
        pfd.events = POLLHUP | POLLRDHUP | POLLIN;
    }
    bool LISTEN;
    const int myFD;
    const unsigned int & MAX_CLIENTS;
    struct timespec timeOut;
};
#endif //TESTDATABASE_SERVER_H

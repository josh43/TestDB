//
// Created by neo on 5/4/16.
//
#include <cstdio>
#include <cstdlib>
#include "Server.h"
#include <string>
int main(int argc, char * argv[]){
    struct sockaddr_un addr;
    int i;
    int ret;
    int data_socket;
    char buffer[BUF_SIZE];

    /* Create local socket. */

    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (data_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     */

    memset(&addr, 0, sizeof(struct sockaddr_un));

    /* Connect socket to socket address */

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = connect (data_socket, (const struct sockaddr *) &addr,
                   sizeof(struct sockaddr_un));
    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }

    /* Send arguments. */

        char buf[BUF_SIZE];
        if(argc < 1){
            strcpy(buf,"NO args\n");
        }else{
            strcpy(buf,argv[1]);
        }
        ret = write(data_socket, buf, BUF_SIZE);
        if (ret == -1) {
            perror("write");

        }


    /* Request result. */



    /* Receive result. */

    ret = read(data_socket, buffer, BUF_SIZE);
    if (ret == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    /* Ensure buffer is 0-terminated. */

    buffer[BUF_SIZE - 1] = 0;

    printf("Result = %s\n", buffer);

    /* Close socket. */
    printf("B4 Closing socket\n");
    while(1){

    }
    close(data_socket);
    printf("Past Closing socket\n");

    exit(EXIT_SUCCESS);
}

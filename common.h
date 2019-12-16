//
// Created by yurunyang on 19-11-11.
//

#ifndef FAKESOCKS_COMMON_H
#define FAKESOCKS_COMMON_H


#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>

namespace  common{


    int createSocket(int family, int type, int protocol);

    struct sockaddr_in  * creatServeraddr(char* ip, int port);

    int sendData(int fd, char * data, int datalen);
    int recvData(int fd, char *buf );
}


#endif //FAKESOCKS_COMMON_H

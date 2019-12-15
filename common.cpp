//
// Created by yurunyang on 19-11-11.
//

#include "common.h"

//using namespace common;
struct sockaddr_in  * common::creatServeraddr(char* ip, int port)
    {

//        std::cout << "inet_addr: " << (ip) << std::endl;
        struct sockaddr_in  * sockaddrIn = new struct sockaddr_in;
        bzero(sockaddrIn, sizeof(struct sockaddr));
        sockaddrIn->sin_family= AF_INET;
        sockaddrIn->sin_addr.s_addr= inet_addr(ip);
        sockaddrIn->sin_port = htons(port);
//        std::cout << "after" << (ip) << std::endl;

        return  sockaddrIn;
    }



int common::createSocket(int family, int type, int protocol) {
        int n ;
        if( (n = socket(family, type, protocol)) < 0)
        {
            std::cout<<strerror(errno);
        }
//        std::cout<<"fd: "<<n<<std::endl;
        return (n);
}

int common::sendData(int fd, unsigned char *data, int datalen) {

    int result = send(fd, data, datalen, 0);


    return 0;
}

int common::recvData(int fd, char *buf){

    int len = recv(fd, buf, 2048, 0);

    return len;
}

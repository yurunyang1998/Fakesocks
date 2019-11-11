//
// Created by yurunyang on 19-11-11.
//

#include "common.h"


struct sockaddr_in  * creatServeraddr(char* ip, int port)
    {
        struct sockaddr_in  * sockaddrIn = new struct sockaddr_in;
        bzero(sockaddrIn, sizeof(struct sockaddr));
        sockaddrIn->sin_family= AF_INET;
        sockaddrIn->sin_addr.s_addr= inet_addr(ip);
        sockaddrIn->sin_port = htons(port);

        return  sockaddrIn;
    }



int createSocket(int family, int type, int protocol) {
        int n ;
        if( (n = socket(family, type, protocol)) < 0)
        {
            std::cout<<strerror(errno);
        }
        return (n);
}

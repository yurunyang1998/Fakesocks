//
// Created by yurunyang on 19-11-30.
//

#ifndef FAKESOCKS_EVENTLOOP_H
#define FAKESOCKS_EVENTLOOP_H

#include "commonHeaders.h"
#include "TCPhandler.h"
#define  FDSIZE 1024


#define  MAXEVENTS 2048

class TCPhandler;

class eventLoop {

private:
    int _epollfd;
    struct epoll_event _events[MAXEVENTS];
    std::shared_ptr<TCPhandler> _tcphandler;
    int _listenfd;



public:
    eventLoop();
    int add_fd(int fd, int state);
    int looprun();
    int bindListenfd(int listenfd);

};


#endif //FAKESOCKS_EVENTLOOP_H

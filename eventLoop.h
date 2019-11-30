//
// Created by yurunyang on 19-11-30.
//

#ifndef FAKESOCKS_EVENTLOOP_H
#define FAKESOCKS_EVENTLOOP_H

#include "commonHeaders.h"
#define  FDSIZE 1024


#define  MAXEVENTS 2048
class eventLoop {

private:
    int epollfd;
    struct epoll_event events[MAXEVENTS];


public:
    eventLoop();
    int add_fd(int fd, int state);
    int looprun();

};


#endif //FAKESOCKS_EVENTLOOP_H

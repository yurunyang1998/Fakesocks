//
// Created by yurunyang on 19-11-30.
//

#include "eventLoop.h"


eventLoop::eventLoop() {

    epollfd = epoll_create(FDSIZE);

}

int eventLoop::add_fd(int fd, int state) {

    struct epoll_event ev;
    ev.events = state;
    ev.data.fd  = fd;
    int i = epoll_ctl(this->epollfd, EPOLL_CTL_ADD, fd, &ev);
    if(i!=0)
    {
        perror("epoll add ");
        return -1;
    }
    return 0;
}

int eventLoop::looprun() {

    while()

    return 0;
}


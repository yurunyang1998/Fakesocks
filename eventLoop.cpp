//
// Created by yurunyang on 19-11-30.
//

#include "eventLoop.h"


eventLoop::eventLoop() {

    _epollfd = epoll_create(FDSIZE);
    _tcphandler = std::shared_ptr<TCPhandler>(new TCPhandler(true, this));



}



int eventLoop::add_fd(int fd, int state) {

    struct epoll_event ev;
    ev.events = state;
    ev.data.fd  = fd;
    int i = epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, fd, &ev);
    if(i!=0)
    {
        perror("epoll add ");
        return -1;
    }
    return 0;
}

int eventLoop::looprun() {

    while(1)
    {
        int nfds = epoll_wait(this->_epollfd, _events, 20, 500);
//        printf("action 1 , %d \n",nfds);
        for(int i=0;i<nfds;i++)
        {
            printf("action \n");
            if(_events[i].data.fd==_listenfd)
            {
                printf("recive \n");
            }
        }
    }

    return 0;
}

int eventLoop::bindListenfd(int listenfd) {
    this->_listenfd = listenfd;
    return 0;
}


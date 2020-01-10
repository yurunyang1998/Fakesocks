//
// Created by yurunyang on 19-11-30.
//

#ifndef FAKESOCKS_EVENTLOOP_H
#define FAKESOCKS_EVENTLOOP_H

#include "commonHeaders.h"
#include "TCPhandler.h"
#include "map"
#include "resolveDNS.h"
#define  FDSIZE 1024


#define  MAXEVENTS 2048

class TCPhandler;
class TCPrelayHandler;
class eventLoop {

private:
    int _epollfd;
    struct epoll_event _events[MAXEVENTS];
    std::shared_ptr<TCPhandler> _tcphandler;
    int _listenfd;
    std::map<int ,std::shared_ptr<TCPrelayHandler>> fdmap;



public:
    eventLoop();
    resolveDNS DNS;
    int add_fd(int fd, int state);
    int del_fd(int fd);
    int looprun();
    int bindListenfd(int listenfd);
    int add_to_fd_map(int confd, TCPrelayHandler * tcPrelayHandler);
    int delFromFdMap(int confd);
};


#endif //FAKESOCKS_EVENTLOOP_H

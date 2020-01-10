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
        int nfds = epoll_wait(this->_epollfd, _events, 21, 500);
//        printf("action 1 , %d \n",nfds);
        for(int i=0;i<nfds;i++)
        {
            if(_events[i].data.fd==_listenfd)
            {
//                printf("get listen request  \n");
                SAin clientaddr;
                socklen_t  len = sizeof(clientaddr);
                int confd = accept(_listenfd, (SA *)&clientaddr, &len);
                this->add_fd(confd, EPOLLIN  | EPOLLET);
                std::shared_ptr<TCPrelayHandler> tcpRelayHandler(new TCPrelayHandler(confd, true, this));

                fdmap.insert(std::pair<int, std::shared_ptr<TCPrelayHandler> >(confd, tcpRelayHandler));
                int a;
            } else{

//                printf("get data \n");
                int fd = _events[i].data.fd;

                uint32_t event = _events[i].events;
                auto  iter = fdmap.find(fd);


//                printf("\n\n\n\n");
//                for(auto i = fdmap.begin();i!=fdmap.end();i++)
//                {
//                    printf("%d\n", i->first);
//                }
//
//                printf("\n");
                if(iter != fdmap.end() )
                {

                    auto temphandler = iter->second;
                    //printf("%x\n", iter->first);
                    int result = temphandler->event_handler(fd , event);

                } else{


                    printf("empty \n");
                    fflush(stdout);
                    break;

                }



            }
        }
    }

    return 0;
}

int eventLoop::bindListenfd(int listenfd) {
    this->_listenfd = listenfd;
    return 0;
}

int eventLoop::add_to_fd_map(int confd, TCPrelayHandler *tcprelayHandler) {

    if(!tcprelayHandler)
    {
        perror("tcprelayHandler NULL");
        return -1;
    }
    std::map<int , std::shared_ptr<TCPrelayHandler>>::iterator iter  = this->fdmap.find(confd);
    if(iter == this->fdmap.end())
    {
        fdmap.insert(std::pair<int, TCPrelayHandler*>(confd, tcprelayHandler));

    } else
    {
        perror("confd has in fdmap");
        return 0;
    }
}



int eventLoop::del_fd(int fd) {

    struct epoll_event ev;
    ev.events = 0;
    ev.data.fd  = fd;
    int i = epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, fd, &ev);
//    if(i!=0)
//    {
//
//        printf("%d ", fd);
//        perror("epoll del ");
//        return -1;
//    }
    return 0;
}





int eventLoop::delFromFdMap(int confd) {


    auto iter = fdmap.find(confd);
    //auto point = iter->second;
    if(iter != fdmap.end())
    {
        fdmap.erase(confd);

    }

    return 0;
}


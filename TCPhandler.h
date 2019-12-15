//
// Created by yurunyang on 19-11-29.
//

#ifndef FAKESOCKS_TCPHANDLER_H
#define FAKESOCKS_TCPHANDLER_H


#include "commonHeaders.h"
#include "eventLoop.h"

#define STAGE_INIT 1
#define STAGE_

#define STAGE_DESTORY 2
#define STAGE_






typedef struct sockaddr SA;
typedef struct sockaddr_in  SAin;
//using namespace std;

class eventLoop;

class TCPhandler {

private:


    std::shared_ptr<SAin> _client;
    int _listensock;
    int _is_client;
//    int _stage;
//    eventLoop * _loop; //TODO:loopevent
    eventLoop * _loop;

public:
    TCPhandler(bool is_local, eventLoop * loop);
    int run();
    int addloop();



};


class TCPrelayHandler
{
private:

    int _clientfd;
    bool _is_local;
    int _remotrfd;
    std::shared_ptr<SAin> _clientaddr;
    std::shared_ptr<SAin> _serveraddr;//(new SAin);
    eventLoop * _loop;
    int stage;

public:
    TCPrelayHandler(int confd, bool is_local, eventLoop * loop){
        if(is_local)
        {

            this->_clientfd = confd;
            _is_local = is_local;
//            loop->add_fd(confd,EPOLLIN | EPOLLET);
            this->_loop = loop;
        }
    }

    int event_handler(int fd ,uint32_t events);
};


#endif //FAKESOCKS_TCPHANDLER_H

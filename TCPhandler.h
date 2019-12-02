//
// Created by yurunyang on 19-11-29.
//

#ifndef FAKESOCKS_TCPHANDLER_H
#define FAKESOCKS_TCPHANDLER_H

#include "commonHeaders.h"
#include "eventLoop.h"

#define STAGE_INIT 1
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


#endif //FAKESOCKS_TCPHANDLER_H

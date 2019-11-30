//
// Created by yurunyang on 19-11-29.
//

#ifndef FAKESOCKS_TCPHANDLER_H
#define FAKESOCKS_TCPHANDLER_H

#include "commonHeaders.h"

#define STAGE_INIT 1
typedef struct sockaddr SA;
typedef struct sockaddr_in  SAin;
using namespace std;
class TCPhandler {

private:


    shared_ptr<SAin> client;
    int _listensock;
    int _is_client;
    int _stage;
    int _loop; //TODO:loopevent


public:
    TCPhandler(int is_local, int loop);
    int run();
    int addloop();



};


#endif //FAKESOCKS_TCPHANDLER_H

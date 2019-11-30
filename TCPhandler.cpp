//
// Created by yurunyang on 19-11-29.
//

#include "TCPhandler.h"

int TCPhandler::run() {


    return 0;
}

TCPhandler::TCPhandler(int is_client, int loop) { //TODO:add config file
    this->_is_client = is_client;
    this->addloop(); //TODO:add loop;
    if(is_client){
        this->_listensock = common::createSocket(AF_INET, SOCK_STREAM, 0);
        client = shared_ptr<SAin>(common::creatServeraddr("127.0.0.1",8999));
        bind(this->_listensock, client);


    }

}

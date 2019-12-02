//
// Created by yurunyang on 19-11-29.
//

#include "TCPhandler.h"

int TCPhandler::run() {


    return 0;
}

TCPhandler::TCPhandler(bool is_client, eventLoop * loop) { //TODO:add config file
    this->_is_client = is_client;
    if(loop)
    {
        this->_loop = loop;
    } else{
        perror("loop null");
        return ;
    }
    //this->addloop(); //TODO:add loop;
    if(is_client){
        this->_listensock = common::createSocket(AF_INET, SOCK_STREAM, 0);
        _client = std::shared_ptr<SAin>(common::creatServeraddr("127.0.0.1",8889));
        bind(this->_listensock, (SA *)_client.get(), sizeof(SAin));

        listen(_listensock, 1024);
        if(loop)
        {
//            _loop->add_fd(this->_listensock,EPOLLIN|EPOLLET);
            _loop->add_fd(_listensock, EPOLLIN | EPOLLET);
            _loop->bindListenfd(this->_listensock);
        }
        printf("create listen socket");

    }


}

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
        int on =1;
        int ret = setsockopt(_listensock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
        _client = std::shared_ptr<SAin>(common::creatServeraddr("127.0.0.1",8889));
        bind(this->_listensock, (SA *)_client.get(), sizeof(SAin));
        listen(_listensock, 1024);
        if(loop)
        {
//            _loop->add_fd(this->_listensock,EPOLLIN|EPOLLET);
            _loop->add_fd(_listensock, EPOLLIN | EPOLLET);
            _loop->bindListenfd(this->_listensock);

        }
//        printf("create listen socket");

    }


}

int TCPrelayHandler::event_handler(int fd ,uint32_t events) {

    if(_is_local) {


        if(fd == this->_clientfd)
        {

            char data[2048];
            memset(data, 0, 2048);
            //TODO:five stages;
            if (this->_stage == STAGE_INIT_0)      // browser shake hand request
            {

                int len = recv(fd, data, 2048, 0);
                for(int i=0;i<len;i++)
                    printf("%d", data[i]);
                fflush(stdout);
                if(data[0]==5 && data[1]==1 && data[2]==0)   //verify sock5 protocol
                {
                    unsigned char  buf[100];
                    buf[0]= 0x5;
                    buf[1]= 0x0;
                    common::sendData(fd, buf, 3);
                    this->_stage = STAGE_INIT_1;
                    return 0;
                } else
                {
                    return -1;
                    //TODO: proxy protocol error ,not sock5
                }
            }
            else if (this->_stage == STAGE_INIT_1)
            {
                memset(data,0,2048);
                int len = common::recvData(fd, data);  // browser send protocol ,ip,and port
                sock5result sock5Result;
                resovlesock5((unsigned char *) data, &sock5Result);
                int a;
            }


        }
        else if(fd == this->_remotrfd)
        {










        }
        else // wrong socket
        {
            perror("wrong socket ,not client or remote ");

        }



    }


    return 0;
}

int TCPrelayHandler::resovlesock5( unsigned char *data, sock5result *sock5Result) {
    if(sock5Result != nullptr and data!= nullptr)
    {
        sock5Result->version = data[0];
        sock5Result->cmd =data[1];
        sock5Result->rsv= data[2];
        sock5Result->atyp= data[3];
        if(data[3]==3)   //domain
        {
            for(int i=data[3]+1, k=0;i<=4+data[4];i++,k++)
            {
                sock5Result->dstaddr[k] = data[i];
            }

            int pos = 5+data[4];


            int high =  data[pos];
            high = high<<8;
            int low = data[pos+1];
            sock5Result->dstport= high+low;

        }
        else if(data[3]==1)  //ipv4
        {
            for(int i=4, k=0;i<8;i++,k++)
            {
                sock5Result->dstaddr[k] = data[i];
            }


            int high =  data[8];
            high = high<<8;
            int low = data[9];
            sock5Result->dstport= high+low;

        }
        else if(data[3] == 6)
        {
            //TODO: ipv6
        }
        return  0;

    }
    return -1;
}

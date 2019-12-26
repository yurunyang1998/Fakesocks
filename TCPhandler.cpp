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
        bind(this->_listensock, (SA *)&(*_client), sizeof(SAin));
        listen(_listensock, 1024);
        if(loop)
        {
//            _loop->add_fd(this->_listensock,EPOLLIN|EPOLLET);
            _loop->add_fd(_listensock, EPOLLIN );
            _loop->bindListenfd(this->_listensock);

        }
//        printf("create listen socket");

    }


}

int TCPrelayHandler::event_handler(int fd ,uint32_t events) {

    if(_is_local) {

        //printf("%d fd ",fd);

        if (fd == this->_clientfd) {

            char data[4096];
            memset(data, 0, 4096);
            //TODO:five stages;
            if (this->_stage == STAGE_INIT_0)      // browser shake hand request
            {
                try{

                    int len = recv(fd, data, 4096, 0);

                    fflush(stdout);
                    if (data[0] == 5 && data[1] == 1 && data[2] == 0)   //verify sock5 protocol
                    {
                        char buf[100];
                        buf[0] = 0x5;
                        buf[1] = 0x0;
                        int len =  common::sendData(fd, buf, 2);
                        if(len == 0)
                        {
                            throw errno;
                        }
                        this->_stage = STAGE_INIT_1;
                        return 0;
                    } else {
                        destory();
                        return -1;
                        //TODO: proxy protocol error ,not sock5
                    }
                }
                catch (int e)
                {
                    destory();
                    perror("STAGE_INIT_0 error ");
                    return -1;
                }



            }
            else if (this->_stage == STAGE_INIT_1) {


                try{
                    char data[4096];
                    memset(data, 0, 4096);
                    int len = common::recvData(fd, data);  // browser send protocol ,ip,and port
//                sock5result sock5Result;
                    resovlesock5(data, &sock5Result);
                    int result = realyRequest(_remotefd, sock5Result); //TODO: 应该写成异步回调式，非阻塞的
                    if (result == 0) {
                        unsigned char data[4096];
                        memset(data, 0, 10);
                        data[0] = 0x05;
                        data[1] = 0x00;
                        data[2] = 0x00;
                        if (sock5Result.atyp == 3) {
                            data[3] = 1;
                        } else if (sock5Result.atyp == 1) {
                            data[3] = 1;
                        } else if (sock5Result.atyp == 6) {
                            data[3] = 4;
                        }

                        data[4] = 0;
                        data[5] = 0;
                        data[6] = 0;
                        data[7] = 0;
                        data[8] = 0;
                        data[9] = 0;
                        //data[10] ='\n';
                        int i = write(fd, data, 10);
                        if (i <= 0)
                        {
                            throw errno;
                        }

                        _stage = STAGE_RELAYDATA;
                        return 0;

                    } else { //TODO:can not access to the dst

                        destory();
                        //perror("STAGE_INIT_1 error ");
                        return -1;
                    }
                }
                catch (int e)
                {
                    destory();
                    perror("STAGE_INIT_1 error ");
                    return -1;
                }



            } else if (this->_stage == STAGE_RELAYDATA)  //relay data to remote server
            {
                try{
                    memset(data, 0, 4096);

                    int len = common::recvData(fd, data);
//                    if(len == 0 )
//                    {
//                        destory();
//                    }
                    if(len <=0)
                        throw errno;
                    len = common::sendData(_remotefd, data, len);
                }
                catch (int e)
                {
                    destory();
                    //perror("STAGE_RELAYDATA0 error");
                    return -1;
                }
            }


        } else if (fd == this->_remotefd) {
            try{
                char data[4096];
                if(this->_stage == STAGE_RELAYDATA) {

                    memset(data, 4096, 0);
                    int len = common::recvData(fd, data);
                    if(len<=0)
                    {
                        throw errno;
                    }
                    len = common::sendData(_clientfd, data, len);

                 }
            }catch (int e)
            {
                destory();
                //perror("STAGE_RELAYDATA error");
            }
        }




         else // wrong socket
        {
            perror("wrong socket ,not client or remote ");

        }


    }
    else{ //server





    }

    return 0;
}

int TCPrelayHandler::resovlesock5(char *data, sock5result *sock5Result) {
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

int TCPrelayHandler::realyRequest(int fd, const sock5result &sock5result1) {

    if(sock5result1.cmd == 1)  // tcp
    {
        if(sock5result1.atyp == 1 or (sock5result1.dstaddr[1]<='9' and sock5result1.dstaddr[1]>='1')) //ipv4
        {
            _remotefd = common::createSocket(AF_INET, SOCK_STREAM, 0);
            _loop->add_fd(_remotefd, EPOLLIN ) ;
            _loop->add_to_fd_map(_remotefd, this);
            char * tempip = (char *)sock5result1.dstaddr;
            std::string dstip(++tempip);


            //TODO:


            std::shared_ptr<SAin> serveaddr(common::creatServeraddr(((char*)dstip.c_str()),sock5result1.dstport));
            int result = connect(_remotefd,(SA*)serveaddr.get(), sizeof(SA));
            if(result == 0)
            {
               // printf("successful\n");
                return 0;
            } else
            {
                perror("connect");
                return -1;
            }
        }
        else if(sock5result1.atyp==3)  //domain
        {

        } else if(sock5result1.atyp == 6) //ipv6
        {

        }
    } else if(sock5result1.cmd == 3) //udp
    {

    }


    return 0;
}

TCPrelayHandler::~TCPrelayHandler() {
//    if(this->_clientfd != 0)
//    {
//        _loop->del_fd(_clientfd);
//        close(_clientfd);
//        _loop->delFromFdMap(_clientfd);
//        _clientfd =0;
//    }
//    if(this->_remotefd !=0)
//    {
//        _loop->del_fd(_remotefd);
//        close(_remotefd);
//        _loop->delFromFdMap(_remotefd);
//        _remotefd = 0;
//    }
    destory();

}

int TCPrelayHandler::destory() {
    if(this->_clientfd != 0)
    {
        _loop->del_fd(_clientfd);
        close(_clientfd);
        //_loop->delFromFdMap(_clientfd);
        _clientfd = 0;
    }
    if(this->_remotefd !=0)
    {
        _loop->del_fd(_remotefd);
        close(_remotefd);
        //_loop->delFromFdMap(_remotefd);
        _remotefd = 0;
    }

    return 0;
}

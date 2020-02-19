//
// Created by yurunyang on 2020/2/16.
//

#include <iostream>
#include <execinfo.h>
#include "tcp_connection.h"
#define SOCKWAITREQUEST 0
#define SOCKCONNECTING 1
#define RELAY2SERVER 2
#define RELAY2SERVER 3
#define READFROMSERVER 4

#define debug


void backtrace(void)

{

    int j, nptrs;

#define SIZE 100

    void *buffer[100];

    char **strings;

    nptrs = backtrace(buffer, SIZE);

    printf("backtrace() returned %d addresses\n", nptrs);

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)

     *  would produce similar output to the following: */



    strings = backtrace_symbols(buffer, nptrs);

    if (strings == NULL) {

        perror("backtrace_symbols");

        exit(EXIT_FAILURE);

    }



    for (j = 0; j < nptrs; j++)

        printf("%s\n", strings[j]);


    printf("\n\\n\n\n\n");
    free(strings);

}







void tcp_connection::handle_read(const boost::system::error_code &ec ,size_t size ){

    if(!ec)
    {

#ifdef  debug
//        switch (stag)
//        {
//            case 0:{
//                printf("SOCKWAITREQUEST ");
//                break;
//            }
//            case 1:{
//                printf("SOCKCONNECTING ");
//                break;
//            }
//            case 2:{
//                printf("READFROMSERVER ");
//                break;
//            }
//            case 3:{
//                printf("");
//                break;
//            }
//
//        }

        if(stag == RELAY2SERVER or READFROMSERVER) {

            printf("recv: ");
            for (int i = 0; i < size; i++)
                printf("%c", data_[i]);
        };
#endif
        int length = 0;
        if(this->stag == SOCKWAITREQUEST)
        {
            if(data_[0] == 5 && data_[1]==1 && data_[2] ==0)
            {
                sock5respone(data_);
                length = 2;
                doWrite(localsocket_,length);

            }
            else{
                    localsocket_.close();
                return;
            }
        } else if(this->stag == SOCKCONNECTING)
        {
            resolvesock5(data_,sock5result_.get());
            do_connect(endpoints);
            sock5respone(data_);
            length = 10;
            doWrite(localsocket_,length);

        }
        else if(this->stag == RELAY2SERVER)
        {
            doWrite(serversocket_,size);

        } else if (this->stag == READFROMSERVER){
            doWrite(localsocket_,size);
        }



    }



}


void tcp_connection::handle_write(const boost::system::error_code &ec ,size_t  size)
{
    if(!ec)
    {
//
//        switch (stag)
//        {
//            case 0:{
//                printf("SOCKWAITREQUEST ");
//                break;
//            }
//            case 1:{
//                printf("SOCKCONNECTING ");
//                break;
//            }
//            case 2:{
//                printf("RELAY2SERVER ");
//                break;
//            }
//            case 3:{
//                printf("SOCKTRANS ");
//                break;
//            }
//
//        }

        if(stag == RELAY2SERVER or READFROMSERVER) {

            printf("send: ");
            for (int i = 0; i < size; i++)
                printf("%c", data_[i]);
        }
        printf("\n");
        if(this->stag == SOCKWAITREQUEST){

            this->stag = SOCKCONNECTING;

        } else if (this->stag == SOCKCONNECTING){

            this->stag = RELAY2SERVER;

        } else if (this->stag == RELAY2SERVER){

            doRead(serversocket_);
            this->stag = READFROMSERVER;
            return;

        } else if (this->stag == READFROMSERVER)
        {
            this->stag = RELAY2SERVER;
        }

    } else{
        std::cout<<ec.message()<<std::endl;
    }

//    backtrace();
    doRead(localsocket_);
}

void tcp_connection::doRead(tcp::socket &socket_) {


    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            boost::bind(&tcp_connection::handle_read, shared_from_this(),
                                        boost::asio::placeholders::error  ,
                                        boost::asio::placeholders::bytes_transferred ));

}


void tcp_connection::doWrite(tcp::socket &socket_, int length) {


    socket_.async_write_some(boost::asio::buffer(data_,length),
                             boost::bind(&tcp_connection::handle_write, shared_from_this(),
                                         boost::asio::placeholders::error  ,
                                         boost::asio::placeholders::bytes_transferred ));


}

void tcp_connection::sock5respone(unsigned char  *data_) {

    if(this->stag == SOCKWAITREQUEST)
    {
        bzero(data_, sizeof(data_));
        data_[0]=0x05;
        data_[1]=0x00;
        data_[2]='\0';
    } else if(this->stag == SOCKCONNECTING)
    {

        //TODO: (fixed) 这里问题，下面这个数据是sock5 协议协商的最后阶段，代理服务器给浏览器发响应，这个数据是发出去了，但是浏览器收到以后马上就rst了，不知道怎么回事，
        //TODO: (fixed) 我抓包分析了 shadowsocks 和这个的流量，看不出什么不一样，对了，当listen端口为1080时， wireshark会自动识别为sock请求

        bzero(data_, sizeof(data_));
        data_[0] = 0x05;
        if (sock5result_->atyp == 3) {
            data_[3] = 3;
        } else if (sock5result_->atyp == 1) {
            data_[3] = 1;
        } else if (sock5result_->atyp == 6) {
            data_[3] = 4;
        }

    }

    return;
}


void tcp_connection::resolvesock5(unsigned char *data_, sock5result *sock5Result) {
    if (sock5Result != nullptr and data_ != nullptr) {
        sock5Result->version = data_[0];
        sock5Result->cmd = data_[1];
        sock5Result->rsv = data_[2];
        sock5Result->atyp = data_[3];
        if (data_[3] == 3)   //domain
        {
            for (int i = data_[3] + 1, k = 0; i <= 4 + data_[4]; i++, k++) {
                sock5Result->dstaddr[k] = data_[i];
            }

            int pos = 5 + data_[4];


            int high = data_[pos];
            high = high << 8;
            int low = data_[pos + 1];
            sock5Result->dstport = high + low;


        } else if (data_[3] == 1)  //ipv4
        {
            for (int i = 4, k = 0; i < 8; i++, k++) {
                sock5Result->dstaddr[k] = data_[i];
            }


            int high = data_[8];
            high = high << 8;
            int low = data_[9];
            sock5Result->dstport = high + low;


        } else if (data_[3] == 6) {
            //TODO: ipv6
        }
        return ;

    }
    return ;
}

void tcp_connection::do_connect(tcp::resolver::results_type endpoints) {
    {

//        serversocket_.async_connect(endpoints,
//                                 boost::bind(&tcp_connection::handle_connect, shared_from_this(),
//                                             boost::asio::placeholders::error));
        boost::asio::async_connect(serversocket_, endpoints,
                                   [this](boost::system::error_code ec, tcp::endpoint)
                                   {
                                       if (!ec)
                                       {
                                           printf("connect\n");
//                                           do_read_header();
                                       } else
                                           printf("can't connect\n");
                                   });
    }

}

void tcp_connection::handle_connect(const boost::system::error_code &ec) {

    if(!ec)
    {
        printf("connect ");
    }
}

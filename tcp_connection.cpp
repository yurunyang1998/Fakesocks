//
// Created by yurunyang on 2020/2/16.
//

#include <iostream>
#include "tcp_connection.h"
#define SOCKWAITREQUEST 0
#define SOCKCONNECTING 1
#define SOCKCONNECTED 2
#define SOCKTRANS 3

void tcp_connection::handle_read(const boost::system::error_code &ec ,size_t size ){

    if(!ec)
    {
        printf("recv: ");
        for(int i=0;i<20;i++)
            printf("%c ",data_[i]);
        printf("\n");

        if(this->stag == SOCKWAITREQUEST)
        {
            if(data_[0] == 5 && data_[1]==1 && data_[2] ==0)
            {

                sock5respone(data_);
                doWrite(localsocket_);
                //stag = SOCKCONNECTING;
            } else{
                    localsocket_.close();
                return;
            }
        } else if(this->stag == SOCKCONNECTING)
        {
            printf("SOCKCONNECTING  ");
            resolvesock5(data_,sock5result_.get());
            sock5respone(data_);
            doWrite(localsocket_);


        }
        else if(this->stag == SOCKCONNECTED)
        {
            printf("SOCKCOONECTED  ");
            doWrite(localsocket_);
            //do_connect(endpoints);
        }

        doRead(localsocket_);



    }



}


void tcp_connection::handle_write(const boost::system::error_code &ec ,size_t  size)
{
    if(!ec)
    {
        printf("send: ");
        for(int i=0;i<20;i++)
            printf("%d",data_[i]);
        printf("\n");
        if(this->stag == SOCKWAITREQUEST){

            this->stag = SOCKCONNECTING;

        } else if (this->stag == SOCKCONNECTING){

            this->stag = SOCKCONNECTED;

        } else if (this->stag == SOCKCONNECTED){

            this->stag = SOCKTRANS;
        }

    }

}

void tcp_connection::doRead(tcp::socket &socket_) {

    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                 boost::bind(&tcp_connection::handle_read, shared_from_this(),
                                             boost::asio::placeholders::error  ,
                                             boost::asio::placeholders::bytes_transferred ));


}

void tcp_connection::doWrite(tcp::socket &socket_) {

    socket_.async_write_some(boost::asio::buffer(data_,10),
                                 boost::bind(&tcp_connection::handle_write, shared_from_this(),
                                             boost::asio::placeholders::error  ,
                                             boost::asio::placeholders::bytes_transferred ));


}

void tcp_connection::sock5respone(unsigned char  *data_) {

    if(this->stag == SOCKWAITREQUEST)
    {
        data_[0]=0x05;
        data_[1]=0x00;
        data_[2]='\0';
    } else if(this->stag == SOCKCONNECTING)
    {
        memset(data_, 0,100);
        data_ = 
//        data_[0] = 0x05;
//        data_[1] = 0x00;
//        data_[2] = 0x00;
//        if (sock5result_->atyp == 3) {
//            data_[3] = 0x00;
//        } else if (sock5result_->atyp == 1) {
//            data_[3] = 0x00;
//        } else if (sock5result_->atyp == 6) {
//            data_[3] = 0x00;
//        }
//
//        data_[4] = 0x00;
//        data_[5] = 0x00;
//        data_[6] = 0x00;
//        data_[7] = 0x00;
//        data_[8] = 0x00;
//        data_[9] = 0x00;
//        data_[10] = '\0';
        printf("data :");
        for(int i=0;i<10;i++)
            printf("%d",data_[i]);
        printf("\n");

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
                                           printf("connect");
//                                           do_read_header();
                                       } else
                                           printf("can't connect");
                                   });
    }

}

void tcp_connection::handle_connect(const boost::system::error_code &ec) {

    if(!ec)
    {
        printf("connect ");
    }
}

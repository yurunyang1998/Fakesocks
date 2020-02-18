//
// Created by yurunyang on 2020/2/16.
//

#ifndef FAKESOCKS_TCP_CONNECTION_H
#define FAKESOCKS_TCP_CONNECTION_H

#include "iostream"
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
//#include "tcp_server.h"

#define SOCKWAITREQUEST 0
#define SOCKCONNECTING 1
#define SOCKCONNECTED 2
#define SOCKTRANS 3

using boost::asio::ip::tcp;


typedef struct sock5result
{
    unsigned  char version;
    unsigned  char cmd;  // tcp 1  or udp 3
    unsigned  char rsv;  // 0 default
    unsigned  char atyp; // 1 ip4,3 domain, 4 ip
    char dstaddr[100]; // the first num is length of dstaddr, and followed by dstaddr , which is ip or domain
    int dstport; // two byte to represent port;


}sock5result;



class tcp_connection: public boost::enable_shared_from_this<tcp_connection>
{
public:


    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context& io_context)
    {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket& localsocket()
    {
        return localsocket_;
    }

    tcp::socket& serversocket()
    {
        return serversocket_;
    }

    void start()
    {

        doRead(localsocket_);
    }

    void doRead(tcp::socket &);


    void doWrite(tcp::socket &);

    void do_connect( tcp::resolver::results_type endpoints);

private:
    tcp_connection(boost::asio::io_context& io_context)            //构造函数
            : localsocket_(io_context),serversocket_(io_context),sock5result_(new sock5result){

        tcp::resolver resolver(io_context);
        endpoints = resolver.resolve("127.0.0.1","9999");
    }


    void handle_write(const boost::system::error_code& /*error*/,
                      size_t /*bytes_transferred*/);

    void handle_read(const boost::system::error_code& /*error*/,
                      size_t /*bytes_transferred*/);

    void handle_connect(const boost::system::error_code& /*error*/);

    void sock5respone(unsigned char * data_);
    void resolvesock5(unsigned char * data, sock5result * sock5Result );

    tcp::socket localsocket_;
    tcp::socket serversocket_;


    enum {max_length = 4096};
    tcp::resolver::results_type endpoints;
    unsigned char data_[max_length];
    std::shared_ptr<sock5result> sock5result_;
    int stag = SOCKWAITREQUEST ;
};


#endif //FAKESOCKS_TCP_CONNECTION_H

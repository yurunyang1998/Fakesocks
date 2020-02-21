//
// Created by yurunyang on 2020/2/16.
//

#ifndef FAKESOCKS_TCP_SERVER_H
#define FAKESOCKS_TCP_SERVER_H

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "tcp_connection.h"

using boost::asio::ip::tcp;

//class tcp_connection : public boost::enable_shared_from_this<tcp_connection>;


typedef boost::shared_ptr<tcp_connection> pointer;

class tcp_server
{
public:

    tcp_server(boost::asio::io_context& io_context)
            : io_context_(io_context),
              acceptor_(io_context, tcp::endpoint(tcp::v4(), 1080))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        tcp_connection::pointer new_connection =
                tcp_connection::create(io_context_);

        acceptor_.async_accept(new_connection->localsocket(),
                               boost::bind(&tcp_server::handle_accept, this, new_connection,
                                           boost::asio::placeholders::error));



    }

    void handle_accept(tcp_connection::pointer new_connection,
                       const boost::system::error_code& error)
    {
        if (!error)
        {
            new_connection->start();
        }

        start_accept();
    }




    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};



#endif //FAKESOCKS_TCP_SERVER_H

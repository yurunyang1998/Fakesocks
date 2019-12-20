//
// Created by yurunyang on 19-11-29.
//

#ifndef FAKESOCKS_TCPHANDLER_H
#define FAKESOCKS_TCPHANDLER_H


#include "commonHeaders.h"
#include "eventLoop.h"
#define STAGE_INIT_0 1
#define STAGE_INIT_1 2
#define STAGE_INIT_2 3
#define STAGE_RELAYDATA 4

typedef struct sockaddr SA;
typedef struct sockaddr_in  SAin;
//using namespace std;


typedef struct sock5result
{
    unsigned  char version;
    unsigned  char cmd;  // tcp 1  or udp 3
    unsigned  char rsv;  // 0 default
    unsigned  char atyp; // 1 ip4,3 domain, 4 ip
    char dstaddr[100]; // the first num is length of dstaddr, and followed by dstaddr , which is ip or domain
    int dstport; // two byte to represent port;


}sock5result;








class eventLoop;

class TCPhandler {

private:


    std::shared_ptr<SAin> _client;
    int _listensock;
    int _is_client;
    eventLoop * _loop;

public:
    TCPhandler(bool is_local, eventLoop * loop);
    int run();
    int addloop();



};


class TCPrelayHandler
{
private:

    int _clientfd;
    bool _is_local;
    int _remotefd;
    std::shared_ptr<SAin> _clientaddr;
    std::shared_ptr<SAin> _serveraddr;//(new SAin);
    eventLoop * _loop;
    int _stage;
    sock5result sock5Result;
    int resovlesock5(char * data, sock5result * sock5Result);
    int realyRequest(int fd, const sock5result  &sock5result1);

public:
    TCPrelayHandler(int confd, bool is_local, eventLoop * loop){
        if(is_local)
        {

            this->_clientfd = confd;
            _is_local = is_local;
//            loop->add_fd(confd,EPOLLIN | EPOLLET);
            this->_loop = loop;
            _stage = STAGE_INIT_0;


        }
    }
    ~TCPrelayHandler();
    int destory();
    int event_handler(int fd ,uint32_t events);
};


#endif //FAKESOCKS_TCPHANDLER_H

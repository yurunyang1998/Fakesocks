//
// Created by yurunyang on 19-11-11.
//



#ifndef FAKESOCKS_RESOLVEDNS_H
#define FAKESOCKS_RESOLVEDNS_H

#include "iostream"
#include "map"
#include <netinet/in.h>
#include "string"
#include "common.h"

//typedef struct Flags_t
//{
//
//
//}Flags_t;


typedef struct DnsHeader_t
{
    uint16_t TransactionId;
    uint16_t Flags;
    uint16_t Questions;
    uint16_t AnswerRRs;
    uint16_t AuthorityRRs;
    uint16_t AdditionalRRs;
}DnsHeader_t;

//typedef struct nameInQuery_t
//{
//
//};


typedef struct Question_t
{
    char  Name[20]={0};
}Question_t;


typedef  struct Answers_t
{
    char Name[30];
    uint16_t type;
    uint16_t class_;
    uint16_t TTL;
    uint16_t dataLength;
    char Data[30];
}Answers_t;


typedef struct DNSreq_t
{
    DnsHeader_t  header;
    Question_t question;
//    Answers_t answers;

}DNSreq_t;





class resolveDNS
    {
private:
        std::map<std::string,std::string> DNScache;
        int udpsockfd;
        struct sockaddr_in dnsServe;
public:
        resolveDNS();
        int url2Ip(std::string dstUrl, std::string port);

    };



#endif //FAKESOCKS_RESOLVEDNS_H

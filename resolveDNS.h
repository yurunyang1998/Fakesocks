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

typedef struct Flags_t
{


}Flags_t;


typedef struct DnsHeader_t
{
    uint16_t TransactionId;
    Flags_t Flags;
    uint16_t Questions;
    uint16_t AnswerRRs;
    uint16_t AuthorityRRs;
    uint16_t AdditionalRRs;
}DnsHeader_t;

typedef struct nameInQuery_t
{
    
};


typedef struct Queries_t
{
    uint32_t Name;

};








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

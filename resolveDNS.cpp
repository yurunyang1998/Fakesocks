//
// Created by yurunyang on 19-11-11.
//
#include <arpa/inet.h>
#include "resolveDNS.h"
using namespace std;
typedef struct sockaddr SA;
int resolveDNS::url2Ip(string dstUrl, string port) {

    sendto(udpsockfd, &dstUrl, dstUrl.length(), 0, (SA*) &dnsServe, sizeof(SA));
    char buf[1024];
    int len=recv(udpsockfd, buf, 1024, 0);
    printf("%d",len);
    for(int i=0;i<len;i++)
        printf("%c", buf[i]);
    fflush(stdout);

}

resolveDNS::resolveDNS() {
    udpsockfd = createSocket(AF_INET, SOCK_DGRAM, 0);
    dnsServe.sin_family=AF_INET;
    dnsServe.sin_addr.s_addr=inet_addr("114.114.114.114");
    dnsServe.sin_port = htons(53);
}
//
// Created by yurunyang on 19-11-11.
//
#include <arpa/inet.h>
#include "resolveDNS.h"
#include "string"
using namespace std;
typedef struct sockaddr SA;

void resolveDNS::setHead(unsigned char *buf)
{
    buf[0] = 0x00;
    buf[1] = 0;
    buf[2] = 0x01;
    buf[3] = 0;
    buf[4] = 0;
    buf[5] = 1;
    buf[6] = 0;
    buf[7] = 0;
    buf[8] = buf[9] = buf[10] = buf[11] = 0;
}

void resolveDNS::setQuery(char *name, unsigned char *buf, int len)
{

//    printf("%s",name);
    strcat((char*)buf+12,name);
//    printf("%x",buf[len]);
    fflush(stdout);
    int pos = len + 12;
    buf[pos] = 0;
    buf[pos+1] = 1;
    buf[pos+2] = 0;
    buf[pos+3] = 1;
}
int resolveDNS::changeDN(char *DN, unsigned char *name)
{
    int i = strlen(DN) - 1;
    int j = i + 1;
    int k;
    name[j+1] = 0;
    for(k = 0; i >= 0; i--,j--) {
        if(DN[i] == '.') {
            name[j] = k;
            k = 0;
        }
        else {
            name[j] = DN[i];
            k++;
        }
    }
    name[0] = k;
    return (strlen(DN) + 2);
}


string resolveDNS::resolve(unsigned char *recvMsg, int len, int len_recvMsg)
{
    int pos = len;
    int cnt = 12;
    while(pos < len_recvMsg) {
        unsigned char now_pos = recvMsg[pos+1];
        unsigned char retype = recvMsg[pos+3];
        unsigned char reclass = recvMsg[pos+5];
        unsigned char offset = recvMsg[pos+11];
        if(retype == 1) {
            if(now_pos == cnt && reclass == 1) {
//                printf("%u.%u.%u.%u\n",recvMsg[pos+12],recvMsg[pos+13],recvMsg[pos+14],recvMsg[pos+15]);]
//                string ip="";
                string ip = std::to_string(recvMsg[pos+12])+'.'+std::to_string(recvMsg[pos+13])+'.'+std::to_string(recvMsg[pos+14])
                +'.'+std::to_string(recvMsg[pos+15]);
//                cout<<ip<<endl;
                return ip;
            }
        }
        else if(retype == 5) {
            cnt = pos + 12 ;
        }
        pos = pos + 12 + offset;
    }
}



int resolveDNS::sendDNSPacket(unsigned char *buf, int len, char *recvMsg)
{
    int s;
    struct sockaddr_in sin;

    memset(&sin,0,sizeof(sin));
//    sin.sin_addr.s_addr = inet_addr("114.114.114.114");
//    sin.sin_family = AF_INET;
//    sin.sin_port = htons(53);
//
//    s = socket(PF_INET,SOCK_DGRAM,0);
//    sendto(s,buf,len,0,(struct sockaddr *)&sin,sizeof(sin));
    sendto(udpsockfd,buf,len,0,(struct sockaddr *)&dnsServe,sizeof(dnsServe));

    int i = recv(udpsockfd,recvMsg,1024,0);
    return i;
}


string  resolveDNS::url2Ip(string dstUrl) {

    unsigned char  buf[1024];
    char  *dsurl=(char* ) dstUrl.c_str();
    char name[1024];
    setHead(buf);
    int len = changeDN(dsurl, (unsigned char *) name);
    setQuery(name , buf,len);
    char recvMsg[1024];
    len+=16;
    int recvlen=sendDNSPacket((unsigned char *) buf,len,recvMsg);

    string ip = " ";
    ip = resolve((unsigned char *)recvMsg, len,recvlen);

    return  ip;
}

resolveDNS::resolveDNS() {
    udpsockfd = common::createSocket(AF_INET, SOCK_DGRAM, 0);
    dnsServe.sin_family=AF_INET;
    dnsServe.sin_addr.s_addr=inet_addr("192.168.1.1");
    dnsServe.sin_port = htons(53);
}
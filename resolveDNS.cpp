//
// Created by yurunyang on 19-11-11.
//
#include <arpa/inet.h>
#include "resolveDNS.h"
using namespace std;
typedef struct sockaddr SA;

int sendDNSPacket(unsigned char *buf, int len, char *recvMsg)
{
    int s;
    struct sockaddr_in sin;

    memset(&sin,0,sizeof(sin));
    sin.sin_addr.s_addr = inet_addr("114.114.114.114");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(53);

    s = socket(PF_INET,SOCK_DGRAM,0);
    sendto(s,buf,len,0,(struct sockaddr *)&sin,sizeof(sin));
    return recv(s,recvMsg,1024,0);

}


int resolveDNS::url2Ip(string dstUrl, string port) {


    DNSreq_t req;
    req.header.TransactionId=1;
    req.header.Flags=0x0001;
    req.header.Questions=0x0100;
    req.header.AnswerRRs=0x0000;
    req.header.AuthorityRRs=0x0000;
    req.header.AdditionalRRs=0x0000;
//    req.question.Name='0moc3udiab5www3';

    int len = dstUrl.length();
    req.question.Name[len+1] = 0;
    int k=0;

    for(int i=dstUrl.length()-1;i>=0;i--)
    {
        if(dstUrl[i] == '.')
        {
            req.question.Name[i+1] = k;
            k=0;
        } else
        {
            req.question.Name[i+1] = dstUrl[i];
            k++;
        }
    }
    req.question.Name[0]=k;
    req.question.Name[len+2]=0;
    req.question.Name[len+3]=1;
    req.question.Name[len+4]=0;
    req.question.Name[len+5]=1;

    char  snd_buf[40];
    memset(snd_buf,0,1024);
    memcpy(snd_buf, &req, sizeof(req));


    sendto(udpsockfd, &snd_buf, sizeof(snd_buf), 0, (SA*) &dnsServe, sizeof(SA));
    char  buf[1024];

    sendDNSPacket((unsigned char *) snd_buf,sizeof(snd_buf),buf);

//    socklen_t lens = sizeof(dnsServe);
//    char * area;
//    int lenss= recv(udpsockfd, buf, 1024, 0);
    printf("%s",buf);
    fflush(stdout);
}

resolveDNS::resolveDNS() {
    udpsockfd = createSocket(AF_INET, SOCK_DGRAM, 0);
    dnsServe.sin_family=AF_INET;
    dnsServe.sin_addr.s_addr=inet_addr("114.114.114.114");
    dnsServe.sin_port = htons(53);
}
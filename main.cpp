#include <iostream>
#include <unistd.h>

#include "resolveDNS.h"
#include "common.h"

typedef struct sockaddr SA;

using namespace std;

#define  BUFSIZE 1024

string resolveURL(char * buf, int len)
{
    string data(buf);
//    cout<<buf<<endl;
    int index = data.find("Host");
    int index2 = data.find("Proxy");
//    cout<<index<<" "<<index2<<endl;
//    cout<<data[index-1]<<data[index]<<data[index+1]<<" "<<data[index2-1]<<data[index2]<<data[index2+1]<<endl;

    string dstURL(data, index+6, index2-index-6);

//    cout<<dstURL<<endl;
    return dstURL;



}




int main() {

    resolveDNS * dns = new resolveDNS();
//    dns->url2Ip("google.com");



    int listenfd, connfd;
    struct sockaddr_in * seraddr,  clientaddr;
    listenfd = common::createSocket(AF_INET, SOCK_STREAM, 0);


    int on = 1;
    int ret = setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );



    seraddr = common::creatServeraddr("192.168.1.159",8889);
    int rs = bind(listenfd,(SA*)seraddr, sizeof(SA));
//    cout<<rs<<endl;
    if(rs!=0)
    {
        cout<<strerror(errno)<<endl;
    }
    char  buf[BUFSIZE];
    listen(listenfd, 10);

    while(1)
    {
        socklen_t len = sizeof(SA);
        connfd = accept(listenfd, (SA *) &clientaddr, &len);
        int childpid    = fork();
        if(childpid == 0)  //child
        {
            close(listenfd);
            while(1)
            {


                struct in_addr;
                memset(buf, 0, sizeof(BUFSIZE));
                int len = recv(connfd, buf, BUFSIZE, 0);
                if(len == 0)
                    break;

//                cout<<"len "<<len<<endl;
//                cout<<inet_ntoa(clientaddr.sin_addr)<<endl;
//                cout<<clientaddr.sin_port<<endl;

//                printf("%s",buf);
//                cout<<buf<<endl;
                fflush(stdout);



                string targetUrlAndPort = resolveURL(buf, len);
                string targetPort,targetUrl;


                int isIp=0;
                if(targetUrlAndPort.find_last_of(":") != -1)         //HTTPS URL
                {
                    int index = targetUrlAndPort.find_last_of(":");
                    targetUrl = targetUrlAndPort.substr(0,index);     //URL
                    targetPort = targetUrlAndPort.substr(index+1, targetUrlAndPort.length()-index); //PORT
                }
                else if(targetUrlAndPort[0] < '9')          //IP
                {
                    targetUrl = targetUrlAndPort;
                    targetPort = "80";
                    isIp = 1;

                }
                else{
                    targetUrl = targetUrlAndPort;         //HTTPS URL
                    targetPort = "80";
                }



                cout<<"URL: "<<targetUrl<<" Port:"<<targetPort <<endl;
                fflush(stdout);

                string ip;

                if(!isIp)
                    ip = dns->url2Ip(targetUrl);
                else
                    ip = targetUrl;

//                cout<<ip.length()<<endl;
                if(ip.length()==0) {

                    cout<<"URL: "<<targetUrl<<" can't find ip"<<endl;
                    return 0;
                }
                else
                    cout<<"URL: "<<targetUrl<<" Port:"<<targetPort<<"ip: "<<ip<<endl;



                int relayfd = common::createSocket(AF_INET, SOCK_STREAM, 0);
//                struct sockaddr_in * relayaddr = creatServeraddr()

                struct sockaddr_in * reqaddr;
                reqaddr = common::creatServeraddr((char *)ip.c_str(),atoi(targetPort.c_str()));
                int i;
                if (connect(relayfd, (SA *)reqaddr, sizeof(SA)) < 0) {
                    perror("connect");
                    exit(1);
                }
                cout<<"create succeed"<<endl;

                while (1){

                    if ((i = send(relayfd, buf, strlen(buf), 0)) < 0) {
                        perror("send");
                        exit(1);
                    }
//                    cout<<buf<<endl;

                    memset(buf,0,BUFSIZE);
                    if((i = recv(relayfd,buf,BUFSIZE,0))<0)
                    {
                        perror("recv");
                        exit(1);
                    }

                    // relay to client

                    send(connfd, buf, i, 0);
                    memset(buf,BUFSIZE,0);
                    recv(connfd, buf,4096, 0);


//                    cout<<buf<<endl;
                }



            }
            return 0;
        } else
        {
            close(connfd);
        }

    }
    return  0;
}














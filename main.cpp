#include <iostream>
#include <unistd.h>

#include "resolveDNS.h"
#include "common.h"


using namespace std;
typedef struct sockaddr_in  SA;

#define  BUFSIZE 1024

string resolveURL(char * buf, int len)
{
    int i=0,flag1=0,flag2=0;

    for(int i=0;i<len;i++)    //get url
    {
        if(buf[i] ==' ')
        {
            if(flag1==0)
            {
                flag1=i;
                continue;
            }

            if(flag2==0)
            {
                flag2=i;
                break;
            }
        }
    }

    string dstURL(buf, flag1, flag2-flag1);
    cout<<dstURL<<endl;
    return dstURL;



}




int main() {

    resolveDNS * dns = new resolveDNS();
    dns->url2Ip("google.com");



    int listenfd, connfd;
    struct sockaddr_in * seraddr,  clientaddr;
    listenfd = common::createSocket(AF_INET, SOCK_STREAM, 0);


    int on = 1;
    int ret = setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );



    seraddr = common::creatServeraddr("192.168.1.159",8889);
    int rs = bind(listenfd,(SA *) seraddr, sizeof(SA));
    if(rs!=0)
    {
        cout<<strerror(errno);
    }
    char  buf[BUFSIZE];
    listen(listenfd, 10);

    while(1)
    {
        socklen_t len = sizeof(SA);
        connfd = accept(listenfd, (SA *) &clientaddr, &len);
        if(connfd==0)
        {
            cout<<strerror(errno)<<endl;
        }
            int childpid = fork();
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

                cout<<"len "<<len<<endl;
//                cout<<inet_ntoa(clientaddr.sin_addr)<<endl;
//                cout<<clientaddr.sin_port<<endl;

//                printf("%s",buf);
                cout<<buf<<endl;
                fflush(stdout);



                string targetUrlAndPort = resolveURL(buf, len);
                int index = targetUrlAndPort.find_last_of(":");

                string targetUrl(targetUrlAndPort, 0, index);     //URL
                string targetPort(targetUrlAndPort, index+1, targetUrlAndPort.length()-index); //PORT

                cout<<"URL: "<<targetUrl<<" Port:"<<targetPort <<endl;
                fflush(stdout);

                string ip = dns->url2Ip(targetUrl);


                int relayfd = common::createSocket(AF_INET, SOCK_DGRAM, 0);
//                struct sockaddr_in * relayaddr = creatServeraddr()






            }
            return 0;
        } else
        {
            close(connfd);
        }

    }
    return  0;
}














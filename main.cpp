#include <iostream>

#include "resolveDNS.h"



using namespace std;
//typedef  SA;


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
    dns->url2Ip("google.com","53");

//
//
//    int listenfd, connfd;
//    struct sockaddr_in * seraddr,  clientaddr;
//    listenfd = common::createSocket(AF_INET, SOCK_STREAM, 0);
//
//
//    int on = 1;
//    int ret = setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
//
//
//
//    seraddr = common::creatServeraddr("192.168.1.159",8889);
//    int rs = bind(listenfd,(SA *) seraddr, sizeof(SA));
//    if(rs!=0)
//    {
//        cout<<strerror(errno);
//    }
//    char  buf[BUFSIZE];
//    listen(listenfd, 10);
//
//    while(1)
//    {
//        socklen_t len = sizeof(SA);
//        connfd = accept(listenfd, (SA *) &clientaddr, &len);
//        if(connfd==0)
//        {
//            cout<<strerror(errno)<<endl;
//        }
//            int childpid = fork();
//        if(childpid == 0)  //child
//        {
//            close(listenfd);
//            while(1)
//            {
//
//
//                struct in_addr;
//                memset(buf, 0, sizeof(BUFSIZE));
//                int len = recv(connfd, buf, BUFSIZE, 0);
//                if(len == 0)
//                    break;
//
//                cout<<"len "<<len<<endl;
////                cout<<inet_ntoa(clientaddr.sin_addr)<<endl;
////                cout<<clientaddr.sin_port<<endl;
//
////                printf("%s",buf);
//                cout<<buf<<endl;
//                fflush(stdout);
//
//
//
//                string targetUrlAndPort = resolveURL(buf, len);
//                int index = targetUrlAndPort.find_last_of(":");
//
//                string targetUrl(targetUrlAndPort, 0, index);     //URL
//                string targetPort(targetUrlAndPort, index+1, targetUrlAndPort.length()-index); //PORT
//
//                cout<<"URL: "<<targetUrl<<" Port:"<<targetPort <<endl;
//                fflush(stdout);
//
//
//                int relayfd = common::createSocket(AF_INET, SOCK_DGRAM, 0);
////                struct sockaddr_in * relayaddr = creatServeraddr()
//
//
//
//
//
//
//            }
//            return 0;
//        } else
//        {
//            close(connfd);
//        }
//
//    }
    return  0;
}












//
//
//
//
//
//
//
//
//
//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//#include<errno.h>
//#include<sys/types.h>
//#include<sys/socket.h>
//#include<netinet/in.h>
//#include<netdb.h>
//#include <arpa/inet.h>
//#include <iostream>
//
//#define MAX_SIZE 1024
//#define SERVER_PORT 53
//
//void setHead(unsigned char *buf)
//{
//    buf[0] = 0x00;
//    buf[1] = 0;
//    buf[2] = 0x01;
//    buf[3] = 0;
//    buf[4] = 0;
//    buf[5] = 1;
//    buf[6] = 0;
//    buf[7] = 0;
//    buf[8] = buf[9] = buf[10] = buf[11] = 0;
//}
//
//void setQuery(char *name, unsigned char *buf, int len)
//{
//
////    printf("%s",name);
//    strcat((char*)buf+12,name);
//    printf("%x",buf[len]);
//    fflush(stdout);
//    int pos = len + 12;
//    buf[pos] = 0;
//    buf[pos+1] = 1;
//    buf[pos+2] = 0;
//    buf[pos+3] = 1;
//}
//int changeDN(char *DN,char *name)
//{
//    int i = strlen(DN) - 1;
//    int j = i + 1;
//    int k;
//    name[j+1] = 0;
//    for(k = 0; i >= 0; i--,j--) {
//        if(DN[i] == '.') {
//            name[j] = k;
//            k = 0;
//        }
//        else {
//            name[j] = DN[i];
//            k++;
//        }
//    }
//    name[0] = k;
//    return (strlen(DN) + 2);
//}
//void printName(int len, char *name)
//{
//    int i;
//    for(i = 0; i < len; i++) printf("%x.",name[i]);
//    printf("\n");
//}
//
//int sendDNSPacket(unsigned char *buf, int len, char *recvMsg)
//{
//    int s;
//    struct sockaddr_in sin;
//
//    memset(&sin,0,sizeof(sin));
//    sin.sin_addr.s_addr = inet_addr("114.114.114.114");
//    sin.sin_family = AF_INET;
//    sin.sin_port = htons(SERVER_PORT);
//
//    s = socket(PF_INET,SOCK_DGRAM,0);
//    sendto(s,buf,len,0,(struct sockaddr *)&sin,sizeof(sin));
//    return recv(s,recvMsg,MAX_SIZE,0);
//
//}
//void resolve(unsigned char *recvMsg, int len, int len_recvMsg)
//{
//    int pos = len;
//    int cnt = 12;
//    while(pos < len_recvMsg) {
//        unsigned char now_pos = recvMsg[pos+1];
//        unsigned char retype = recvMsg[pos+3];
//        unsigned char reclass = recvMsg[pos+5];
//        unsigned char offset = recvMsg[pos+11];
//        if(retype == 1) {
//            if(now_pos == cnt && reclass == 1) {
//                printf("%u.%u.%u.%u\n",recvMsg[pos+12],recvMsg[pos+13],recvMsg[pos+14],recvMsg[pos+15]);
//            }
//        }
//        else if(retype == 5) {
//            cnt = pos + 12 ;
//        }
//        pos = pos + 12 + offset;
//    }
//}
//int main()
//{
//    unsigned char buf[MAX_SIZE]; /* socket发送的数据 */
//    char DN[MAX_SIZE]; /* 将要解析的域名(www.xxx.xxx) */
//    char name[MAX_SIZE]; /* 转换为符合DNS报文格式的域名 */
//    char recvMsg[MAX_SIZE]; /* 接收的数据 */
//    int len; /* socket发送数据的长度 */
//    int s; /* socket handler */
//
//    printf("输入需要解析的域名：");
//    scanf("%s",DN);
//
//    len = changeDN(DN,name);
//    //printName(len,name); /* 打印转换后的域名，检测是否转换正确 */
//    int j;
//    //printf("len is %d\n",len);
//    setHead(buf);
//    setQuery(name,buf,len);
//    len += 16;
//    int len_recvMsg = sendDNSPacket(buf,len,recvMsg);
//    printf("接收的报文长度为 %d 字节\n",len_recvMsg);
//    printf("下面是接收报文的16进制表示：\n");
//    int i;
//    for(i = 0; i < len_recvMsg; i++) {
//        printf("%x.",(unsigned char)recvMsg[i]);
//    }
//    printf("\n");
//    printf("%s的IP地址为：\n",DN);
//    resolve((unsigned char *) recvMsg,len,len_recvMsg);
//
//}

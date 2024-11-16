#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
int main(){
    //创建通信套接字
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("socket");
        return -1;
    }
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(6666);
    inet_pton(AF_INET,"192.168.8.153",&saddr.sin_addr.s_addr);
    int ret=connect(fd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret==1){
        perror("connect");
        return -1;
    }
    while(1){
        char buffer[1024];
        sprintf(buffer,"Hello World!");
        send(fd,buffer,strlen(buffer)+1,0);
        memset(buffer,0,sizeof(buffer));
        int len =recv(fd,buffer,sizeof(buffer),0);
        if(len>0){
            std::cout<<buffer<<std::endl;
        }else if(len==0){
            std::cout<<"服务器断开连接"<<std::endl;
            break;
        }else{
            perror("recv");
            break;
        }
        sleep(1);
    }
    close(fd);
    return 0;
}
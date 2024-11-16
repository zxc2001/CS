#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

int main(){
    //创建套接字
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("socket");
        return -1;
    }
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(6666);
    //saddr.sin_addr.s_addr=INADDR_ANY;
    inet_pton(AF_INET,"192.168.8.153",&saddr.sin_addr.s_addr);
    //绑定
    int ret=bind(fd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret==-1){
        perror("bind");
        return -1;
    }
    //监听
    ret=listen(fd,128);
    if(ret==-1){
        perror("listen");
        return -1;
    }
    
    struct sockaddr_in caddr;
    uint32_t len=sizeof(caddr);
    //连接
    std::cout<<"等待连接....."<<std::endl;
    int cfd=accept(fd,(struct sockaddr*)&caddr,&len);
    if(cfd==-1){
        perror("accept");
        return -1;
    
    }
    char ip[32];
    std::cout <<inet_ntop(AF_INET,&caddr.sin_addr.s_addr,ip,sizeof(ip))<<ntohs(caddr.sin_port) << std::endl;
    //收发
    while(1){
        char buffer[1024];
        int len=recv(cfd,buffer,sizeof(buffer),0);
        if(len>0){
            std::cout<<buffer<<std::endl;
            send(cfd,buffer,len,0);
            memset(buffer,0,sizeof(buffer));
        }else if(len==0){
            std::cout<<"客户端已经断开连接"<<std::endl;
            break;
        }else{
            perror("recv");
            break;
        }
        
    }
    //关闭套接字
    close(cfd);
    close(fd);
    return 0;
}


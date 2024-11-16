#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fstream>

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

    std::ifstream file ("test1.txt",std::ios::binary);
    if(!file.is_open()){
        std::cout<<"文件打开失败"<<std::endl;
        return -1;
    }
    std::ofstream RECV ("test2.txt",std::ios::binary|std::ios::app);
    if(!RECV.is_open()){
        std::cout<<"保存文件打开失败"<<std::endl;
        return -1;
    }
    
    char buffer[1024];
    while(file.read(buffer,sizeof(buffer)-25)){
        uint32_t read_size=file.gcount();
        if (send(fd, buffer, read_size, 0) == -1) {
            perror("send");
            close(fd);
            return -1;
        }
        //std::cout<<"\n读取:"<<buffer;
        std::cout<<"\n发送了"<<read_size<<"字节数据"<<std::endl;
        memset(buffer,0,sizeof(buffer));
        int len =recv(fd,buffer,sizeof(buffer),0);
        if(len>0){
            RECV.write(buffer,read_size);
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

    if (file.gcount() > 0) {
        uint32_t read_size = file.gcount();
        if (send(fd, buffer, read_size, 0) == -1) {
            perror("send");
            close(fd);
            return -1;
        }
        std::cout<<"发送了"<<read_size<<"字节数据"<<std::endl;
        memset(buffer,0,sizeof(buffer));
        int len =recv(fd,buffer,sizeof(buffer),0);
        if(len>0){
            RECV.write(buffer,read_size);
            std::cout<<buffer;
        }else if(len==0){
            std::cout<<"服务器断开连接"<<std::endl;
        }else{
            perror("recv");
        }
        sleep(1);
    }
    sleep(3);
    std::cout<<"\n文件发送完毕"<<std::endl;
    RECV.close();
    file.close();
    close(fd);
    return 0;
}
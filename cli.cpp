#include "../source/TcpServer.hpp"
#include <stdlib.h>
int main()
{
    Socket x;
    x.CreateSocket();
    srand(time(0));
    x.Bind(8000+(rand()%1000));
    
    int n=x.Connect("49.232.249.2",8100);
    if(n!=0)
    lg(Debug,"Connect success");
    int i=5;
    for(int i=1;i<=1;i++)
    {
        const char* buf="hellox";
        int n=send(x.fd(),buf,strlen(buf),0);
        if(n<0)
        lg(Error,"write error");
        sleep(1);
        char buff[65535];
       n=recv(x.fd(),buff,65535,0);
        cout<<buff<<endl;
        //sleep(1);
    }
    
    //sleep(3);
    close(x.fd());
}
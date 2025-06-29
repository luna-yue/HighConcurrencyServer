#include "Buffer.hpp"
#include <iostream>
using namespace std;

int main()
{

    Buffer x;
    char buf[65535]="hello";
    x.Write(buf,5);
    x.MoveWriteOffset(5);
    cout<<x.UsedSpace()<<endl;
    char buff[20];
    x.Read(buff,5);
    cout<<"read get:"<<buff<<endl;
}
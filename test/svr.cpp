#include "../source/Server.cpp"
#include "../source/Echo/EchoServer.cpp"
 Socket lis;

 unordered_map<int, shared_ptr<Connection>>all;

int main()
{   //lg.IfDebug=0;
    EchoServer x(7001,10);
    x.Start();
}
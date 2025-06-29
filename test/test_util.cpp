#include "../Utils.hpp"
#include <typeinfo>
int main()
{   
    /* string s="IaaaLoveaaauika!!!3!244!!!!!!";
    vector<string>res;
    string tmp="!";
    Utils::Split(s,tmp,res);
    for (auto &e:res)
    cout<<e<<endl;
    res.clear(); 
    string s="SAKISAKISAKI";
    Utils::WriteFile("testfile.txt",s);
    Utils::ReadFile("testfile.txt",s);
    cout<<s<<endl;
    char s[10];
    char x='%';
    sprintf(s,"%%%02X",x);int i=0;
    for(i=0;s[i]!='\0';i++)
    cout<<s[i]<<" ";cout<<i<<'\n';
    string s="c++";
    string res;
    Utils::EncodeUrl(s,res);
    cout<<res<<'\n';
    s.clear();
    Utils::DecodeUrl(res,s);
    cout<<s<<endl;
    cout<<Utils::GetStatus(404)<<endl;
    cout<<Utils::GetStatus(555)<<endl;
    cout<<Utils::GetMime(".avi")<<endl;
    cout<<Utils::GetMime("!!!!!")<<endl;
    cout<<Utils::GetMime("adqweqwe.qwqwqe")<<endl;
    cout<<Utils::Directory("../test")<<endl;
    cout<<Utils::Directory("test_util")<<endl;
    cout<<Utils::Regular("../test")<<endl;
    cout<<Utils::Regular("test_util")<<endl;
    cout<<Utils::ValidPath("../H");
    cout<<Utils::ValidPath("hello/z/../..");
    string  s= "GET /DIR/INDEX.HTM HTTP/1.1 \r\n";
    
    regex x("(GET|OPTION|HEAD|POST|PUT|DELETE|TRACE|CONNECT) ([^? ]+)(?:\\?([^ ]*))? (HTTP/1\\.[10]) (?:\r|\r\n|\n)?",regex::icase);
    smatch sm;
    bool ret=regex_match(s,sm,x);
    cout<<sm.size()<<endl;
    for(auto e:sm)
    {cout<<typeid(e).name();break;}*/
    string  s1= "GET /DIR/INDEX.HTM HTTP/1.1 \r\n";
    string s="GET /home/login?user=xiaoming&pass=123123 HTTP/1.1\r\n";
    HttpContext x;
    //x.ParseLine(s1);
}
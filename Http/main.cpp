#include "HttpServer.hpp"
std::string RequestStr(const HttpRequest &req)
{
    lg(Debug,"get request str");
    std::stringstream ss;
    ss << req._method << " " << req._path << " " << req._version << "\r\n";
    for (auto &it : req._params)
    {
        ss << it.first << ": " << it.second << "\r\n";
    }
    for (auto &it : req._headers)
    {
        ss << it.first << ": " << it.second << "\r\n";
    }
    ss << "\r\n";
    ss << req._body;
    lg(Debug,"get request str done");
    return ss.str();
}
void PutMethod(const HttpRequest &req, HttpResponse &rsp) {
    //rsp.SetBody(RequestStr(req)+"PutMetheod","text/plain");
    lg(Warning,"put method");
    Utils::WriteFile("./WWWROOT/"+req._path,req._body);
}
void DeleteMethod(const HttpRequest &req, HttpResponse &rsp) {
    rsp.SetBody(RequestStr(req)+"DeleteMetheod","text/plain");
}
void GetMethod(const HttpRequest &req, HttpResponse &rsp) {
    rsp.SetBody(RequestStr(req)+"GetMetheod","text/plain");
    //lg(Debug,"rsp body size:>%d",rsp._body.size());
}
void PostMethod(const HttpRequest &req, HttpResponse &rsp) {
    rsp.SetBody(RequestStr(req)+"PostMetheod","text/plain");
}
void HeadMethod(const HttpRequest &req, HttpResponse &rsp) {
    cout<<"Head method"<<endl;
}
int main()
{
    HttpServer x(8101,2);
    x.SetBaseDir("WWWROOT");
    x.Delete("/DELETE",DeleteMethod);
    x.Put("/1234.txt",PutMethod);
    x.Post("/POST",PostMethod);
    x.Get("/GET",GetMethod);
    x.Head("/Head",HeadMethod);
    x.EnableInactiveRelease(60);
    x.Start();
    /*Buffer test;
    string s="GET / HTTP/1.1\r\nhello\\\r\n";
    test.Write(s);
    test.MoveWriteOffset(s.size());
    cout<<test.UsedSpace()<<endl;
    string tmp;cout<<test.ReadLine(tmp)<<endl;
    cout<<tmp<<endl;
    regex x("(GET|OPTION|HEAD|POST|PUT|DELETE|TRACE|CONNECT)\\s+([^? ]+)(?:\\?([^\\s]*))?\\s+(HTTP/1\\.[10])\\s+(?:\r|\r\n|\n)?", regex::icase);
    string tmp="GET /GET HTTP/1.1\r\n";
    smatch sm;
    bool ret=regex_match(tmp,sm,x);
    cout<<ret<<endl;
    for(auto e:sm)
    cout<<e<<endl;*/
}
#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpContext.hpp"
#include "../source/TcpServer.hpp"
class HttpServer
{
  using Handler = function<void(const HttpRequest &, HttpResponse &)>;
  using Handlers = vector<pair<regex, Handler>>;
  TcpServer _tcpserver;
  string _basedir;
  string _name;
  // 各种请求方式的路由表
  // 同一种method 可能有不同功能 这里采用通过url来区分 那么当请求的url 与用户设置的某种regex 能够匹配 就使用对应的函数处理
  Handlers _post_route;
  Handlers _put_route;
  Handlers _get_route;
  Handlers _delete_route;
  Handlers _head_route; 
  // 判断是否静态请求
  bool IsStatic(HttpRequest &req);
  // 处理静态请求
  void StaticHandler(HttpRequest &req, HttpResponse &rep);
  void Dispatcher(HttpRequest &req, HttpResponse &rep, Handlers &handlers);
  void Route(HttpRequest &req, HttpResponse &rep);
  void WriteResponse(const ConnectionPtr &conn, const HttpRequest &req, HttpResponse &rep);
  void ErrorHandler(const HttpRequest &req, HttpResponse &rep);
  void ConnectionHandler(const ConnectionPtr &conn) ;
  void MessageHandler(const ConnectionPtr &conn, Buffer &buf);
  public:
  HttpServer(int port,int thread_cnt,string name="httpsever",int timeout=DEFAULT_TIMEOUT):_tcpserver(port,thread_cnt),_name(name){
    _tcpserver.SetConnectionCallback(bind(&HttpServer::ConnectionHandler,this,placeholders::_1));
    _tcpserver.SetMessageCallback(bind(&HttpServer::MessageHandler,this,placeholders::_1,placeholders::_2));
 }
  string GetName();
  void Get(const string& pattern,const Handler& handler);
  void Post(const string& pattern,const Handler& handler);
  void Delete(const string& pattern,const Handler& handler);
  void Put(const string& pattern,const Handler& handler);
  void Head(const string& pattern,const Handler& handler);
  void SetBaseDir(const string&path);
  void Start();
  void EnableInactiveRelease(int time=DEFAULT_RELEASE_TIME);
};
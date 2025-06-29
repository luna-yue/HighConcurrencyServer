#pragma once
#include "HttpRequest.hpp"
#include "HttpRequest.hpp"
#include "Utils.hpp"
#include "../source/Buffer.hpp"
typedef enum
{
  _recv_line,
  _recv_head,
  _recv_body,
  _recv_over,
  _recv_error
} HttpRecvStatu; // 当前解析到了哪个阶段
class HttpContext
{
private:
  HttpRequest _req;
  HttpRecvStatu _statu;
  int _rsp_statu; // 返回的状态码
  // 解析
  bool ParseLine(const string &line);
  bool ParseHead(string &head);
  // 获取首行
  bool RecvHttpLine(Buffer &buf);
  // 获取头部
  bool RecvHttpHead(Buffer &buf);
  // 获取正文
  bool RecvHttpBody(Buffer &buf);
public:
  HttpContext();
  void Reset();
  // 接受并解析req 如果不全 则等待下次读事件触发再接着解析
  void Recv_Parse_Request(Buffer &buf);
  HttpRequest &Request();
  int Response_statu();
  int Statu();
};
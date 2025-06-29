#pragma once
#include "Utils.hpp"
class HttpResponse
{
public:
  int _statu;          // 状态码
  bool _redirect_flag; // 重定向；
  string _redirect_url;
  string _body; // 正文
  unordered_map<string, string> _headers;
  HttpResponse() ;
  HttpResponse(int statu);
  void Reset();
  void SetHeader(const string &key, const string &val);
  bool HasHeader(const string &key) const;
  string GetHeader(const string &key) const;
  void SetRedirect(const string &url, int statu = 302);
  void SetBody(const string &body,const string&type);
  bool Close() const;
  void ShowResponse();
};
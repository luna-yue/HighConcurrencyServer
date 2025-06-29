#pragma once
#include "HttpRequest.hpp"
void HttpRequest::Reset()
{
  _method.clear();
  _version = "HTTP/1.1";
  smatch tmp;
  _matches.swap(tmp);
  _headers.clear();
  _params.clear();
}
void HttpRequest::SetHeader(const string key, const string val)
{
  _headers.insert({key, val});
}
bool HttpRequest::HasHeader(const string &key) const
{
  if (_headers.find(key) == _headers.end())
    return false;
  return true;
}
string HttpRequest::GetHeader(const string &key) const
{
  auto it = _headers.find(key);
  if (it == _headers.end())
    return "";
  return it->second;
}
void HttpRequest::SetParam(const string &key, const string &val)
{
  _params.insert({key, val});
}
bool HttpRequest::HasParam(const string &key) const
{
  if (_params.find(key) == _params.end())
    return false;
  return true;
}
string HttpRequest::GetParam(const string &key) const
{
  auto it = _params.find(key);
  if (it == _params.end())
    return "";
  return it->second;
}
// 获取正文长度
size_t HttpRequest::ContentLength()
{
  if (!HasHeader("Content-Length"))
    return 0;
  return stol(GetHeader("Content-Length"));
}
// 判断是否为短链接
bool HttpRequest::Close() const
{
  for(auto e:_headers)
    {
        lg(Debug,"req show header:>%s %s",e.first.c_str() ,e.second.c_str());
    }
    lg(Debug,"req :> has header connection:>%d Getheader :>%s",HasHeader("Connection"),GetHeader("Connection").c_str());
  if (HasHeader("Connection") && GetHeader("Connection") == "keep-alive")
    return false;
  return true;
}

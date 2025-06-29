#pragma once
#include "Utils.hpp"
//全部public 
class HttpRequest
{
public:
  string _method;
  string _version;
  smatch _matches;
  string _body;
  string _path;
  unordered_map<string, string> _headers;
  unordered_map<string, string> _params; // 存放 请求行中 GET等方法带有的参数
  HttpRequest() : _version("HTTP/1.1") {}
  void Reset();
  void SetHeader(const string key, const string val);
  bool HasHeader(const string &key) const;
  string GetHeader(const string &key) const;
  void SetParam(const string &key, const string &val);
  bool HasParam(const string &key) const;
  string GetParam(const string &key) const;
  // 获取正文长度
  size_t ContentLength();
  // 判断是否为短链接
  bool Close() const;
};
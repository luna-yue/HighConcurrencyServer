#pragma once
#include "HttpResponse.hpp"
HttpResponse::HttpResponse() : _statu(200), _redirect_flag(false) {} // 200:OK
HttpResponse::HttpResponse(int statu) : _statu(statu), _redirect_flag(false) {}
void HttpResponse::Reset()
{
    _statu = 200;
    _redirect_flag = false;
    _redirect_url.clear();
    _body.clear();
    _headers.clear();
}
void HttpResponse::SetHeader(const string &key, const string &val)
{
    _headers.insert({key, val});
}
bool HttpResponse::HasHeader(const string &key) const
{
    if (_headers.find(key) == _headers.end())
        return false;
    return true;
}
string HttpResponse::GetHeader(const string &key) const
{
    auto it = _headers.find(key);
    if (it == _headers.end())
        return "";
    return it->second;
}
void HttpResponse::SetRedirect(const string &url, int statu )
{
    _redirect_flag = true;
    _redirect_url = url;
    _statu = statu;
}
void HttpResponse::SetBody(const string &body,const string&type)
{
    _body = body;
    SetHeader("Content-Type",type);
}
bool HttpResponse::Close() const
{
    for(auto e:_headers)
    {
        lg(Debug,"rsp show header:>%s %s",e.first.c_str() ,e.second.c_str());
    }
    if (HasHeader("Connection") && GetHeader("Connection") == "keep-alive")
        return false;
    return true;
}
void HttpResponse::ShowResponse(){
     cout<<"Show Response"<<endl;
     cout<<_statu<<" "<<_body<<endl;
     for(auto e:_headers)
     cout<<e.first<<" "<<e.second<<endl;
}
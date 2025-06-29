#pragma once
#include "HttpContext.hpp"
// 解析
bool HttpContext::ParseLine(const string &line)
{
    // GET /home/login?user=xiaoming&pass=123123 HTTP/1.1\r\n
    regex x("(GET|OPTION|HEAD|POST|PUT|DELETE|TRACE|CONNECT)\\s+([^? ]+)(?:\\?([^\\s]*))?\\s+(HTTP/1\\.[10])\\s+(?:\r|\r\n|\n)?", regex::icase);
   // regex e("(GET|HEAD|POST|PUT|DELETE) ([^?]*)(?:\\?(.*))? (HTTP/1\\.[01])(?:\n|\r\n)?", std::regex::icase);
    string decoded_url;
    string tmpp="GET / HTTP/1.1\r\n";
    lg(Debug,"sz:>%d",line.size());
    lg(Debug,"Parse Line:%s",line.c_str());
    bool ret = regex_match(line, _req._matches, x); // 按照regex 获取 方法 url 协议版本等
    if (!ret)
    {
        lg(Error, "regex_match failed%s",strerror(errno));
        _statu = _recv_error;
        _rsp_statu = 400; // BAD REQUEST
        return false;
    }
    _req._method = _req._matches[1];
    Utils::DecodeUrl(_req._matches[2], _req._path, false); // url路径中的加号并不解码
    _req._version = _req._matches[4];
    int pos = 0;
    string tmp = _req._matches[3];
    int sz = tmp.size();
    while (pos < sz)
    {
        int tmppos; // 下一个&的位置
        tmppos = tmp.find('&', pos);
        if (tmppos == string::npos)
            tmppos = sz;
        int dpos; // 等号的位置
        dpos = tmp.find("=", pos);
        if (dpos == string::npos)
        {
            lg(Error, "Illegal Line parse: = not found");
            _statu = _recv_error;
            _rsp_statu = 400; // BAD REQUEST
            return false;
        }
        string key;
        Utils::DecodeUrl(tmp.substr(pos, dpos - pos), key, true);
        string val;
        Utils::DecodeUrl(tmp.substr(dpos + 1, tmppos - dpos - 1), key, true); // 参数需要进行+到空格的解码
        _req.SetParam(key, val);
        // //cout<<key<<" "<<val<<'\n';
        pos = tmppos + 1;

    }
    return true;
}
bool HttpContext::ParseHead(string &head)
{
    int val1=0;int val2=0;

    if (head.back() == '\n')
        val1=1,head.pop_back();
    if (head.back() == '\r')
        val2=1,head.pop_back();
    int pos = head.find(": ");
    if (pos == string::npos)
    {
        lg(Error, "Illegal Head parse: ': ' not found");
        _statu = _recv_error;
        _rsp_statu = 400; // BAD REQUEST
        return false;
    }
    ////cout<<head<<" "<<pos<<endl;
    string key = head.substr(0, pos);
    string val = head.substr(pos + 2);
    ////cout<<key<<" "<<val<<endl;
    _req.SetHeader(key, val);
    //cout<<"SetHeader done"<<endl;
    if(val1)
    head.push_back('\n');
    if(val2)
    head.push_back('\r');
    return true;
}
// 获取首行
bool HttpContext::RecvHttpLine(Buffer &buf)
{
    if (_statu != _recv_line)
        return false;
    string line;
    bool res = buf.ReadLine(line);
    //lg(Debug,"ReadLine result:>%d",res);
    // ？是否要从buffer中pop数据
    if (!res) // 数据过长 或者 首行不全
    {
       
        if (buf.UsedSpace() > MAX_LINE_SIZE)
        { 
            lg(Error,"URI TOO LONG");
            _rsp_statu = 414; // URI TOO LONG
            _statu = _recv_error;
            return false;
        }
        return true; // 中止这次接受处理req  ？但缓冲区数据保留
    }
    res = ParseLine(line);
    //lg(Debug,"ParseLine result:>%d",res);
    if (!res)
        return false;
    _statu = _recv_head;
    buf.MoveReadOffset(line.size());
    return true;
}

// 获取头部
bool HttpContext::RecvHttpHead(Buffer &buf)
{
    if (_statu != _recv_head)
        return false;
    // 循环读取 直到遇到空行 （'\n' | '\r\n'）
    string empty1 = "\n";
    string empty2 = "\r\n";
    
    while (1)
    {
        string line;
        bool res = buf.ReadLine(line);
        if (!res) // 数据过长 或者 首行不全
        {
            if (buf.UsedSpace() > MAX_LINE_SIZE)
            {
                _rsp_statu = 414; // URI TOO LONG
                _statu = _recv_error;
                return false;
            }
            return true;
        }
        if (line.size() <= 2)
        {
            if (line == empty1 || line == empty2)
            {
                _statu = _recv_body;
                buf.MoveReadOffset(line.size());
                return true;
            }
        }
        //cout<<"parse head start<"<<endl;
        res = ParseHead(line);
        if (!res)
            return false;
        buf.MoveReadOffset(line.size());
        //string mem;
        //buf.Read(mem,buf.UsedSpace());
        ////cout<<mem<<endl;
        //cout<<"success"<<endl;
    }
    return true;
}
// 获取正文
bool HttpContext::RecvHttpBody(Buffer &buf)
{
    if (_statu != _recv_body)
        return false;
        lg(Debug,"Recv body ");
    size_t body_length = _req.ContentLength();
    if (body_length == 0)
    {
        _statu = _recv_over;
        return true;
    }
    uint64_t res_len = body_length - _req._body.size();
    uint64_t res_buf = buf.UsedSpace();
    string tmp;
    buf.Read(tmp, min(res_len, res_buf));
    _req._body += tmp;
    if (res_buf >= res_len)
    {
        _statu = _recv_over;
    }
    buf.MoveReadOffset(min(res_len, res_buf));

    return true;
}
HttpContext::HttpContext() : _statu(_recv_line), _rsp_statu(200) {}
void HttpContext::Reset()
{
    _statu = _recv_line;
    _rsp_statu = 200;
    _req.Reset();
}
void HttpContext::Recv_Parse_Request(Buffer &buf)
{ // 接受并解析req 如果不全 则等待下次读事件触发再接着解析
    switch(_statu){
    case _recv_line:
        RecvHttpLine(buf);
    case _recv_head:
        RecvHttpHead(buf);
    case _recv_body:
        RecvHttpBody(buf);
    }
     
}
HttpRequest &HttpContext::Request() { return _req; }
int HttpContext::Response_statu() { return _rsp_statu; }
int HttpContext::Statu() { return _statu; }

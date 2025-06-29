#pragma once
#include "HttpServer.hpp"
bool HttpServer::IsStatic(HttpRequest &req)
{

    // 必须设置 实际的根目录
    if (_basedir.empty())
        return false;
    // 路径是否合法？ 不能超出指定的根目录
    if (!Utils::ValidPath(req._path))
        return false;
    // method必须为get 或者 head
    if (req._method != "GET" && req._method != "HEAD")
        return false;
    // 请求的必须是常规文件
    //cout << "path and result:>" << req._path << " " << Utils::Regular(req._path) << endl;
    if (!Utils::Regular(req._path))
        return false;
    return true;
}
// 处理静态请求
void HttpServer::StaticHandler(HttpRequest &req, HttpResponse &rsp)
{
    lg(Info, "Static Handler");
    bool ret = Utils::ReadFile(req._path, rsp._body);
    if (!ret)
        return;
    string mime = Utils::GetMime(req._path);
    //cout << "rsp._body length:::" << " " << rsp._body.length() << endl;
    rsp.SetHeader("Content-Length", to_string(rsp._body.length()));
    rsp.SetHeader("Content-Type", mime);
}
void HttpServer::Dispatcher(HttpRequest &req, HttpResponse &rsp, Handlers &handlers)
{
    //cout<<req._path<<endl;
    for (auto &e : handlers)
    {
        regex &re = e.first;
        Handler &functor = e.second;
        bool ret = regex_match(req._path, req._matches, re);
        if (!ret)
            continue;
           // cout<<"match succeed";
        return functor(req, rsp);
    }
   // cout<<"match failed"<<endl;
    rsp._statu = 404;
}
void HttpServer::Route(HttpRequest &req, HttpResponse &rsp)
{
    lg(Warning, "Route start %s", req._method.c_str());
    // 判断请求的是静态资源还是动态资源
    if (IsStatic(req))
    {
        if (req._path.empty() || req._path == "/") // 如果期望访问默认地址 设为index.html 可修改
            req._path = _basedir + req._path + "/index.html";
        else
            req._path = _basedir + req._path;
        //cout << req._path << endl;
        return StaticHandler(req, rsp);
    }
    else if (req._method == "POST")
        return Dispatcher(req, rsp, _post_route);
    else if (req._method == "DELETE")
        return Dispatcher(req, rsp, _delete_route);
    else if (req._method == "PUT")
        {cout<<"here enter put"<<endl;return Dispatcher(req, rsp, _put_route);}
    else if (req._method == "GET")
        return Dispatcher(req, rsp, _get_route);
    else if (req._method == "HEAD")
        return Dispatcher(req, rsp, _head_route);
    rsp._statu = 405;
}
void HttpServer::WriteResponse(const ConnectionPtr &conn, const HttpRequest &req, HttpResponse &rsp)
{
    if (!rsp._body.empty())
    {
        if (!rsp.HasHeader("Content-Length"))
            rsp.SetHeader("Content-Length", to_string(rsp._body.size()));
        if (!rsp.HasHeader("Content-Type"))
            rsp.SetHeader("Content-Type", "application/octet-stream");
    }
    if (req.Close())
        rsp.SetHeader("Connection", "close");
    else
        rsp.SetHeader("Connection", "keep-alive");
    if (rsp._redirect_flag)
        rsp.SetHeader("Location", rsp._redirect_url);
    stringstream rsp_string;
    rsp_string << req._version << " " << to_string(rsp._statu) << " " << Utils::GetStatus(rsp._statu) << "\r\n";
    for (auto &e : rsp._headers)
    {
        rsp_string << e.first << ": " << e.second << "\r\n";
    }
    rsp_string << "\r\n";
    rsp_string << rsp._body;
    //cout << "Write Response:>" << endl;
    //cout << rsp_string.str() << endl;
    //cout << rsp_string.str().size() << endl;
    conn->Send(rsp_string.str().c_str(), rsp_string.str().size());
}
void HttpServer::ErrorHandler(const HttpRequest &req, HttpResponse &rsp)
{
    std::string body;
    body += "<html>";
    body += "<head>";
    body += "<meta http-equiv='Content-Type' content='text/html;charset=utf-8'>";
    body += "</head>";
    body += "<body>";
    body += "<h1>";
    body += std::to_string(rsp._statu);
    body += " ";
    body += Utils::GetStatus(rsp._statu);
    body += "</h1>";
    body += "</body>";
    body += "</html>";
    // 2. 将页面数据，当作响应正文，放入rsp中
    rsp.SetBody(body, "text/html");
    rsp.SetHeader("Content-Type", "text/html");
}
void HttpServer::ConnectionHandler(const ConnectionPtr &conn)
{
    conn->SetContext(HttpContext()); // 设置上下文
    lg(Info, "New Connection %p", conn.get());
}
void HttpServer::MessageHandler(const ConnectionPtr &conn, Buffer &buf)
{
    lg(Debug, "message cb Used space:>%d", buf.UsedSpace());
    while (buf.UsedSpace() > 0)
    {

        // 1. 获取上下文
        HttpContext *context = boost::any_cast<HttpContext>(conn->GetContext());
        // 2. 通过上下文对缓冲区数据进行解析，得到HttpRequest对象
        // 2.1 如果解析出错 进行错误相应
        // 2.2 如果req不完整 等待完整
        lg(Debug, "message handler response statu (start Recv_Parse_Request):>%d %d", context->Response_statu(),context->Statu());
        context->Recv_Parse_Request(buf);
        lg(Debug, "message handler response statu (finish Recv_Parse_Request):>%d", context->Response_statu());
        HttpRequest &req = context->Request();
        HttpResponse rsp(context->Response_statu());
        lg(Debug,"shutdown check---------> context statu:> %d",context->Response_statu());
        if (context->Response_statu() > 200)
        {
            lg(Debug, "response statu error");
            ErrorHandler(req, rsp);
            WriteResponse(conn, req, rsp);
            context->Reset();
            buf.Clear();
            conn->ShutDown();
        }
        lg(Debug, "recv_statu:>%d", context->Statu());
        if (context->Statu() != _recv_over)
           { lg(Debug,"recv not over !");return;}

        // 3. 请求路由 + 业务处理
        Route(req, rsp);
        // 4. 对HttpResponse进行组织发送
        WriteResponse(conn, req, rsp);
        // 5. 重置上下文
        context->Reset();
        lg(Debug,"shutdown check---------> rsp no keepalive?:> %d",rsp.Close());
        // 6. 根据长短连接判断是否关闭连接或者继续处理
        lg(Debug,"connection statu:>%d",conn->GetStatu());
        if (rsp.Close())
            conn->ShutDown(); // 短链接直接关闭
    }
}
void HttpServer::Get(const string &pattern, const Handler &handler)
{
    _get_route.push_back(make_pair(regex(pattern), handler));
}
void HttpServer::Post(const string &pattern, const Handler &handler)
{
    _post_route.push_back(make_pair(regex(pattern), handler));
}
void HttpServer::Delete(const string &pattern, const Handler &handler)
{
    _delete_route.push_back(make_pair(regex(pattern), handler));
}
void HttpServer::Put(const string &pattern, const Handler &handler)
{
    _put_route.push_back(make_pair(regex(pattern), handler));
}
void HttpServer::Head(const string &pattern, const Handler &handler)
{
    _head_route.push_back(make_pair(regex(pattern), handler));
}
string HttpServer::GetName() { return _name; }
void HttpServer::Start()
{
    
    lg(Info, "%s start listening to %d", GetName(), _tcpserver.GetPort());
    _tcpserver.Start();
}
void HttpServer::SetBaseDir(const string &path) { _basedir = path; }
void HttpServer::EnableInactiveRelease(int time)
{
    _tcpserver.EnableInactiveRelease(time);
}
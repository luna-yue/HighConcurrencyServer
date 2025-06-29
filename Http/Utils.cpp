#pragma once
#include "Utils.hpp"
std::unordered_map<int, std::string> _statu_msg = {
    {100, "Continue"},
    {101, "Switching Protocol"},
    {102, "Processing"},
    {103, "Early Hints"},
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {203, "Non-Authoritative Information"},
    {204, "No Content"},
    {205, "Reset Content"},
    {206, "Partial Content"},
    {207, "Multi-Status"},
    {208, "Already Reported"},
    {226, "IM Used"},
    {300, "Multiple Choice"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {303, "See Other"},
    {304, "Not Modified"},
    {305, "Use Proxy"},
    {306, "unused"},
    {307, "Temporary Redirect"},
    {308, "Permanent Redirect"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {402, "Payment Required"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {406, "Not Acceptable"},
    {407, "Proxy Authentication Required"},
    {408, "Request Timeout"},
    {409, "Conflict"},
    {410, "Gone"},
    {411, "Length Required"},
    {412, "Precondition Failed"},
    {413, "Payload Too Large"},
    {414, "URI Too Long"},
    {415, "Unsupported Media Type"},
    {416, "Range Not Satisfiable"},
    {417, "Expectation Failed"},
    {418, "I'm a teapot"},
    {421, "Misdirected Request"},
    {422, "Unprocessable Entity"},
    {423, "Locked"},
    {424, "Failed Dependency"},
    {425, "Too Early"},
    {426, "Upgrade Required"},
    {428, "Precondition Required"},
    {429, "Too Many Requests"},
    {431, "Request Header Fields Too Large"},
    {451, "Unavailable For Legal Reasons"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
    {504, "Gateway Timeout"},
    {505, "HTTP Version Not Supported"},
    {506, "Variant Also Negotiates"},
    {507, "Insufficient Storage"},
    {508, "Loop Detected"},
    {510, "Not Extended"},
    {511, "Network Authentication Required"}};

std::unordered_map<std::string, std::string> _mime_msg = {
    {".aac", "audio/aac"},
    {".abw", "application/x-abiword"},
    {".arc", "application/x-freearc"},
    {".avi", "video/x-msvideo"},
    {".azw", "application/vnd.amazon.ebook"},
    {".bin", "application/octet-stream"},
    {".bmp", "image/bmp"},
    {".bz", "application/x-bzip"},
    {".bz2", "application/x-bzip2"},
    {".csh", "application/x-csh"},
    {".css", "text/css"},
    {".csv", "text/csv"},
    {".doc", "application/msword"},
    {".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {".eot", "application/vnd.ms-fontobject"},
    {".epub", "application/epub+zip"},
    {".gif", "image/gif"},
    {".htm", "text/html"},
    {".html", "text/html"},
    {".ico", "image/vnd.microsoft.icon"},
    {".ics", "text/calendar"},
    {".jar", "application/java-archive"},
    {".jpeg", "image/jpeg"},
    {".jpg", "image/jpeg"},
    {".js", "text/javascript"},
    {".json", "application/json"},
    {".jsonld", "application/ld+json"},
    {".mid", "audio/midi"},
    {".midi", "audio/x-midi"},
    {".mjs", "text/javascript"},
    {".mp3", "audio/mpeg"},
    {".mpeg", "video/mpeg"},
    {".mpkg", "application/vnd.apple.installer+xml"},
    {".odp", "application/vnd.oasis.opendocument.presentation"},
    {".ods", "application/vnd.oasis.opendocument.spreadsheet"},
    {".odt", "application/vnd.oasis.opendocument.text"},
    {".oga", "audio/ogg"},
    {".ogv", "video/ogg"},
    {".ogx", "application/ogg"},
    {".otf", "font/otf"},
    {".png", "image/png"},
    {".pdf", "application/pdf"},
    {".ppt", "application/vnd.ms-powerpoint"},
    {".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
    {".rar", "application/x-rar-compressed"},
    {".rtf", "application/rtf"},
    {".sh", "application/x-sh"},
    {".svg", "image/svg+xml"},
    {".swf", "application/x-shockwave-flash"},
    {".tar", "application/x-tar"},
    {".tif", "image/tiff"},
    {".tiff", "image/tiff"},
    {".ttf", "font/ttf"},
    {".txt", "text/plain"},
    {".vsd", "application/vnd.visio"},
    {".wav", "audio/wav"},
    {".weba", "audio/webm"},
    {".webm", "video/webm"},
    {".webp", "image/webp"},
    {".woff", "font/woff"},
    {".woff2", "font/woff2"},
    {".xhtml", "application/xhtml+xml"},
    {".xls", "application/vnd.ms-excel"},
    {".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
    {".xml", "application/xml"},
    {".xul", "application/vnd.mozilla.xul+xml"},
    {".zip", "application/zip"},
    {".3gp", "video/3gpp"},
    {".3g2", "video/3gpp2"},
    {".7z", "application/x-7z-compressed"}};
unsigned int Utils::Split(const string &ori, const string &sep, vector<string> &res)
{ // 将ori字符串 以 sep作为分割标志分割 存储到res中 返回分割的块数
    if (sep.empty() || ori.empty())
        return 0;
    int pos = 0;
    int prepos = 0;
    int seplen = sep.size();
    pos = ori.find(sep, prepos);
    while (pos != string::npos)
    {
        if (ori.substr(prepos, seplen) != sep)
        {
            res.push_back(ori.substr(prepos, pos - prepos));
        }
        prepos = pos + seplen;
        if (prepos < ori.size())
            pos = ori.find(sep, prepos);
        else
            break;
    }
    if (prepos < ori.size())
        res.push_back(ori.substr(prepos));
    return res.size();
}
// 追加向文件写入 s 中的内容
bool Utils::WriteFile(const string &filename, const string &s)
{
    std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
    if (ofs.is_open() == false)
    {
        lg(Error, "OPEN %s FILE FAILED!!", filename.c_str());
        return false;
    }
    ofs.write(s.c_str(), s.size());
    if (ofs.good() == false)
    {
        lg(Error, "WRITE %s FILE FAILED!", filename.c_str());
        ofs.close();
        return false;
    }
    lg(Warning, "Write success");
    ofs.close();
    return true;
}
// 读文件内容 存入s
bool Utils::ReadFile(const string &filename, string &s)
{
    ifstream ifs(filename, ifstream::binary);
    if (!ifs.is_open())
    {
        lg(Error, "file oepn failed filename:%s reason:%s", filename.c_str(), strerror(errno));
        return false;
    }
    // 得到文件长度
    ifs.seekg(0, ifs.end); // 跳转读写位置
    int length = ifs.tellg();
    ifs.seekg(0, ifs.beg);

    s.resize(length);
    ifs.read(&s[0], length);
    if (!ifs.good())
    {
        lg(Error, "file read failed filename:%s reason:%s", filename.c_str(), strerror(errno));
        ifs.close();
        return false;
    }
    ifs.close();
    return true;
}
// URL编码，避免URL中资源路径与查询字符串中的特殊字符与HTTP请求中特殊字符产生歧义
// 编码格式：将特殊字符的ascii值，转换为两个16进制字符，前缀%   C++ -> C%2B%2B
//   不编码的特殊字符： RFC3986文档规定 . - _ ~ 字母，数字属于绝对不编码字符
// RFC3986文档规定，编码格式 %HH
// W3C标准中规定，查询字符串中的空格，需要编码为+， 解码则是+转空格
void Utils::EncodeUrl(const string &url, string &res, bool Encode_space = true)
{
    char tmp[4];
    for (auto &e : url)
    {
        if (e == '.' || e == '-' || e == '_' || e == '~' || isalnum(e))
        {
            res += e;
            continue;
        }
        if (e == ' ' && Encode_space)
        {
            res += e;
            continue;
        }
        if (e)
            sprintf(tmp, "%%%02X", e);
        res += tmp;
    }
}
int Utils::HexToInt(char x)
{
    if (x >= '0' && x <= '9')
        return x - '0';
    else if (x >= 'a' && x <= 'f')
        return x - 'a' + 10;
    else if (x >= 'A' && x <= 'F')
        return x - 'A' + 10;
    else
    {
        lg(Error, "HEXTOINT OUT RANGE");
        return -1;
    }
}
void Utils::DecodeUrl(const string &url, string &res, bool Decode_space = true)
{
    int sz = url.size();
    for (int i = 0; i < sz; i++)
    {
        if (url[i] == '.' || url[i] == '-' || url[i] == '_' || url[i] == '~')
            res += url[i];
        else if (url[i] == '+' && Decode_space)
            res += ' ';
        else if (url[i] == '%' && i + 2 < sz)
        {
            char x = HexToInt(url[i + 1]) * 16 + HexToInt(url[i + 2]);
            res += x;
            i += 2;
        }
        else
        {
            res += url[i];
        }
    }
}
string Utils::GetStatus(int status)
{
    auto it = _statu_msg.find(status);
    if (it == _statu_msg.end())
        return "unknown";
    return it->second;
}
// 获取文件的mime类型
//  MIME  是描述消息内容类型的标准，用来表示文档、文件或字节流的性质和格式。
string Utils::GetMime(const string &filename)
{
    int pos = filename.find_last_of('.');
    if (pos == string::npos)
        return "application/octet-stream"; // 若不满足条件 则当作二进制数据流
    auto it = _mime_msg.find(filename.substr(pos));
    if (it == _mime_msg.end())
        return "application/octet-stream";
    return it->second;
}
bool Utils::Regular(const string &filename)
{ // 判断是否为常规文件
    struct stat st;
    int ret = stat(filename.c_str(), &st);
    if (ret < 0)
    {
        // lg(Error, "Get file type Error ,%s", strerror(errno));
        return false;
    }
    return S_ISREG(st.st_mode);
}
bool Utils::Directory(const string &filename)
{ // 判断是否为目录
    struct stat st;
    int ret = stat(filename.c_str(), &st);
    if (ret < 0)
    {
        // lg(Error, "Get file type Error ,%s", strerror(errno));
        return false;
    }
    return S_ISDIR(st.st_mode);
}
bool Utils::ValidPath(const string &path)
{ // 判断是否访问了内容的根目录之外
    vector<string> subpath;
    Split(path, "/", subpath);
    string up = "..";
    int now_level = 0;
    for (auto &e : subpath)
    {
        if (e == up)
            now_level--;
        else
            now_level++;
        if (now_level < 0)
            return false;
    }
    return true;
}
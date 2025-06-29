#pragma once
#include <fstream>
#include <ctype.h>
#include <sys/stat.h>
#include <string>
#include <regex>
#include <unordered_map>
#include <string.h>
#include <errno.h>
#include "../source/Buffer.hpp"
#include "../source/Log.hpp"
using namespace std;
#define MAX_LINE_SIZE 8192
#define DEFAULT_TIMEOUT 30
class Utils
{
public:
  // 将ori字符串 以 sep作为分割标志分割 存储到res中 返回分割的块数
  static unsigned int Split(const string &ori, const string &sep, vector<string> &res);
  // 追加向文件写入 s 中的内容
  static bool WriteFile(const string &filename, const string &s);
  // 读文件内容 存入s
  static bool ReadFile(const string &filename, string &s);
  // URL编码，避免URL中资源路径与查询字符串中的特殊字符与HTTP请求中特殊字符产生歧义
  // 编码格式：将特殊字符的ascii值，转换为两个16进制字符，前缀%   C++ -> C%2B%2B
  //   不编码的特殊字符： RFC3986文档规定 . - _ ~ 字母，数字属于绝对不编码字符
  // RFC3986文档规定，编码格式 %HH
  // W3C标准中规定，查询字符串中的空格，需要编码为+， 解码则是+转空格
  static void EncodeUrl(const string &url, string &res, bool Encode_space );
  static int HexToInt(char x);
  static void DecodeUrl(const string &url, string &res, bool Decode_space );
  static string GetStatus(int status);
  // 获取文件的mime类型
  //  MIME  是描述消息内容类型的标准，用来表示文档、文件或字节流的性质和格式。
  static string GetMime(const string &filename);
  // 判断是否为常规文件
  static bool Regular(const string &filename);
  // 判断是否为目录
  static bool Directory(const string &filename);
  // 判断是否访问了内容的根目录之外
  static bool ValidPath(const string &path);
};
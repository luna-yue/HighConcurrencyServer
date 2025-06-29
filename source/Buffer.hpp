#pragma once
#include <vector>
#include <stdint.h>
#include <string>
#include <assert.h>
#include <string.h>
#include <iostream>
//#include "Log.hpp"
using namespace std;
#define DEFAULT_BUFFER_SIZE 1024
#define MAX_BUFFER_SIZE 1000000
class Buffer
{
    uint64_t _wpos; // 读偏移
    uint64_t _rpos; // 写偏移
    vector<char> _buffer;
    void Extend();
    char *ReadPosition();

    char *WritePosition();

public:
    Buffer() : _wpos(0), _rpos(0), _buffer(DEFAULT_BUFFER_SIZE) {}
    bool empty()
    {
        return _rpos == _wpos;
    }
    char *GetSt()
    {
        return nullptr;
    }
    uint64_t UsedSpace()
    {
        return _wpos - _rpos;
    }
    uint64_t ResSpace()
    {
        return _buffer.size() - UsedSpace();
    }
    uint64_t TailSpace()
    {
        return _buffer.size() - _wpos;
    }
    // 保证数据是顺序存储 不会出现 【数据 空闲 数据】的情况
    void SpaceCheck(int sz);
    void Write(const void *s, int sz);
    void Write(string &s);
    void Write(Buffer &tmp);
    void Read(void *buf, uint64_t len); // 当从读缓冲区读取报文 如果报文完整被上层取走 则应移动偏移量从buffer删除，反之保持rpos
    void Read(string&s,uint64_t len);
    bool ReadLine(string &s); // 读取一行数据
    void MoveReadOffset(int len)
    {
        _rpos += len;
        
    }
    void MoveWriteOffset(int len)
    { 
        _wpos+=len;
    }
    void Clear()
    {
        _wpos = _rpos = 0;
    }
};
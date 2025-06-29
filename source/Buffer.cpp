#include "Buffer.hpp"
void Buffer::SpaceCheck(int sz)
{
    assert(sz >= 0);
    if (TailSpace() >= sz)
        return;
    if (ResSpace() >= sz) // 说明 前后各有一部分空间可写入数据
    {
        uint64_t rsz = UsedSpace();
        copy(ReadPosition(), ReadPosition() + rsz, _buffer.begin());
        _wpos=rsz;
        _rpos=0;
    }
    else
    {
        while (ResSpace() < sz)
        {
            Extend();
        }
    }
}
void Buffer::Write(const void *s, int sz)
{
    SpaceCheck(sz);
    int bsz = _buffer.size();
    const char *p = (const char *)s;
    copy(p, p + sz, WritePosition());
   // for(int i=0;i<sz;i++)
   // WritePosition()[i]=p[i];
}
void Buffer::Write(string &s)
{
    Write(s.c_str(), s.size());
}
void Buffer::Write(Buffer &tmp)
{
    Write(tmp.ReadPosition(), tmp.UsedSpace());
}
void Buffer::Read(void *buf, uint64_t len) // 当从读缓冲区读取报文 如果报文完整被上层取走 则应移动偏移量从buffer删除，反之保持rpos
{
    //cout<<"gonna read:"<<len<<" "<<UsedSpace()<<endl;
    assert(len <= UsedSpace());
    copy(ReadPosition(), ReadPosition() + len, (char *)buf);
    //printf("%s",(char*)buf);
}
void Buffer::Read(string& s, uint64_t len)
{
    assert(len<=UsedSpace());
    char *rp=ReadPosition();
    for(int i=0;i<len;i++)
    s.push_back(rp[i]);
}
bool Buffer::ReadLine(string &s) // 读取一行数据
{
    char *res = (char *)memchr(ReadPosition(), '\n', UsedSpace());
    if (res == NULL)
        return false;
    //cout<<res-ReadPosition()<<endl;
    Read(s, res - ReadPosition() + 1);
    return true;
}
void Buffer::Extend()
{
    int bsz = _buffer.size();
    assert(bsz * 2 <= MAX_BUFFER_SIZE);
    _buffer.resize(bsz * 2);
}
char *Buffer::ReadPosition()
{
    return &*_buffer.begin() + _rpos;
}
char *Buffer::WritePosition()
{
    return &*_buffer.begin() + _wpos;
}
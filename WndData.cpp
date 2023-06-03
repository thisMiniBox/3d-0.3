#include"WndData.h"
int getBit(int num, int pos)
{
    return (num >> pos) & 1;
}
ReturnedOfLoadFile operator|(ReturnedOfLoadFile a, ReturnedOfLoadFile b)
{
    return static_cast<ReturnedOfLoadFile>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}
ReturnedOfLoadFile& operator|=(ReturnedOfLoadFile& a, ReturnedOfLoadFile b)
{
    a = static_cast<ReturnedOfLoadFile>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
    return a;
}
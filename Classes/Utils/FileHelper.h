#ifndef __FILE_HELPER__
#define __FILE_HELPER__

#include "cocos2d.h"
#include "Utils/RapidXml.h"

#include <string>
using namespace std;
USING_NS_CC;
class FileHelper
{
public:
    ~FileHelper();

    static FileHelper* Instance();

    bool isExist(const char *filename);
    void copyFile(const char *filename);

    int getRecord(char *out_str);
    bool writeRecord(int score);
protected:
    FileHelper();
};
#endif
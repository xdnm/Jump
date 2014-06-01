#include "Utils/TagHelper.h"



int TagHelper::getTag(ObjectName name)
{
    int tagRet;
    switch(name)
    {
    case ON_ROLE:
        tagRet = 1;
        break;
    case ON_BLOCK:
        tagRet = 2;
        break;
    case ON_EDGE:
        tagRet = 3;
        break;
    case ON_WEAPON:
        tagRet = 4;
        break;
    case ON_ATTACK:
        tagRet = 5;
    default:
        tagRet = 0;
    }

    return tagRet;
}

bool TagHelper::isObject(int tag, ObjectName name)
{
    bool ret = false;

    switch(name)
    {
    case ON_ROLE:
        if(tag == 1)
            ret = true;
        break;
    case ON_BLOCK:
        if(tag == 2)
            ret = true;
        break;

    case ON_EDGE:
        if(tag == 3)
            ret = true;
        break;
    case ON_WEAPON:
            if(tag == 4)
                ret = true;
            break;
    case ON_ATTACK:
            if(tag == 5)
                ret = true;
            break;
    default:
        break;
    }

    return ret;
}
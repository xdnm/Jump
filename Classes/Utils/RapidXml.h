#ifndef __RAPID_XML__
#define __RAPID_XML__

#include "Xml/rapidxml.hpp"
#include "Xml/rapidxml_print.hpp"
#include "Xml/rapidxml_utils.hpp"

using namespace rapidxml;

///save this function for future use.
//CCPoint str2CCPoint(char *ch)
//{
//    string str = ch;
//    CCPoint point = ccp(-1, -1);
//    int x, y;
//
//    int index = str.find(',');
//    if(index == 0 || index == str.length() - 1)
//        return point;
//
//    string num1 = str.substr(0, index);
//    x = atoi(num1.c_str());
//
//    num1 = str.substr(index + 1, str.length() - index - 1);
//    y = atoi(num1.c_str());
//
//    point = ccp(x, y);
//    return point;
//
//}


#endif
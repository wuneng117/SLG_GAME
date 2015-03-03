//
//  types.h
//  testproject
//
//  Created by ZHOUTAO on 15/1/22.
//
//

#ifndef testproject_types_h
#define testproject_types_h

#include <iostream>
#include <string>

#define BIT(x) (1 << (x))                       ///< Returns value with bit x set (2^x)

typedef unsigned int U32;
typedef int S32;

const U32 HERODATA_BEGIN_ID = 10000000;
const U32 HERODATA_END_ID   = 19999999;

const U32 ITEMDATA_BEGIN_ID = 20000000;
const U32 ITEMDATA_END_ID   = 29999999;

const U32 SKILLDATA_BEGIN_ID = 30000000;
const U32 SKILLDATA_END_ID   = 39999999;

const U32 BUFFDATA_BEGIN_ID = 40000000;
const U32 BUFFDATA_END_ID   = 49999999;

const U32 STATUSDATA_BEGIN_ID = 90000000;
const U32 STATUSDATA_END_ID   = 99999999;

const char* const HeroClass[] =
{
    "",
    "人外",
    "偶像",
    "吃货",
    "天才",
    "学生会长",
    "萝莉",
    "贵族",
    "中二",
    "高中生",
    "傲娇",
    "眼镜",
    "皇帝",
    "战斗狂",
    "变态",
    "乙女",
    "公主",
    "大小姐",
    "女仆",
    "女王",
    "御姐",
    "魔女",
    "妹妹",
    "伪娘",
    "兄贵",
    "大叔",
    "正太",
    "热血少年",
    "正义的青年",
    "天然呆"
};

class city;
typedef std::map<U32,city*> CITY_MAP;
typedef CITY_MAP::iterator  CITY_MAPITR;

#endif

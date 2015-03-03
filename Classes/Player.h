//
//  Player.h
//  collectionapp
//
//  Created by ZHOUTAO on 14-7-31.
//
//

#ifndef __collectionapp__Player__
#define __collectionapp__Player__

#include "types.h"
#include "HeroManager.h"
#include "BuffData.h"

//city
class city
{
public:
    
    city();
    ~city();
    
    U32 m_id;   // id
    std::string m_name; // 名字
    std::string m_des;  // 描述
    
    U32 m_gold;         // 基础收入
    U32 m_country;      // 所属国
    
    U32 m_miningLv;   // 矿洞
    U32 m_farmLv;     // 牧场
    U32 m_treeLv;     // 伐木场
    U32 m_alchemyLv;  // 炼金实验室
    
    U32 getMingingLv();
    U32 getFarmLv();
    U32 getTreeLv();
    U32 getAlchemyLv();
    
    U32 m_itemLv;     // 道具开发
    U32 m_weaponLv;   // 武器开发
    U32 m_armorLv;    // 防具开发
    U32 m_charmLv;    // 珠宝研究
    U32 m_manageLv;   // 冒险者工会
    U32 m_communicationLv;  // 酒馆
    U32 m_homeLv;     // 领主宅邸

};


class Player
{
public:
    typedef std::map<U32,HeroRef>  HERO_MAP;
    typedef HERO_MAP::iterator     HERO_MAPITR;
    
    typedef std::map<U32,Buff> BUFF_MAP;
    typedef BUFF_MAP::iterator BUFF_MAPITR;
    
    typedef std::map<U32,city*> CITY_MAP;
    typedef CITY_MAP::iterator  CITY_MAPITR;
    
    Player();
    ~Player();
    
    bool init();
    
    bool readSaveData(void* buffer);    // 读取保存
    bool createNew();   // 新建一个player数据
    bool createNewRandom(); // 随机新建
    
    bool m_cpu;  // 是否是电脑
    
    
    U32 m_country; // 国家
    int m_gold;    // 拥有金钱
    
    HERO_MAP m_heroMap;         // 保存英雄列表
    BUFF_MAP m_battleBuffMap;   // 保存战斗buff列表
    CITY_MAP m_cityMap;         // 保存都市列表
    
};

#endif /* defined(__collectionapp__Player__) */

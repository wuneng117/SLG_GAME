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
    city();
    ~city();
    
    U32 m_miningLv;   //挖矿
    U32 m_farmLv;     //农牧
    U32 m_treeLv;     //土木
    U32 m_alchemyLv;  //炼金
    
    U32 m_itemLv;     //合成（道具）
    U32 m_weaponLv;   //锻造（武器）
    U32 m_armorLv;    //铸甲（防具）
    U32 m_charmLv;    //魅力（珠宝）
    U32 m_manageLv;   //管理
    U32 m_communicationLv;  //交涉
    U32 m_homeLv;     //家政
    U32 m_searchLv;   //探索

};


class Player
{
public:
    typedef std::map<U32,HeroRef>  HERO_MAP;
    typedef HERO_MAP::iterator HERO_MAPITR;
    
    typedef std::map<U32,Buff> BUFF_MAP;
    typedef BUFF_MAP::iterator BUFF_MAPITR;
    
    Player();
    ~Player();
    
    bool init();
    
    bool readSaveData(void* buffer);    //读取保存
    bool createNew();   //新建一个player数据
    bool createNewRandom(); //随机新建
    
    bool m_cpu;  //是否是电脑
    
    

    HERO_MAP m_heroMap;  //保存英雄列表
    BUFF_MAP m_battleBuffMap;   //保存战斗buff列表
};

#endif /* defined(__collectionapp__Player__) */

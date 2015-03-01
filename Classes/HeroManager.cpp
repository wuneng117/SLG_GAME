//
//  HeroManager.cpp
//  lantisi
//
//  Created by ZHOUTAO on 14-5-24.
//
//

#include "HeroManager.h"
#include "BuffData.h"
#include "ctest.h"

static HeroManager* s_HeroManager = nullptr;

HeroManager::HeroManager()
{
};

HeroManager::~HeroManager()
{
    m_heroDataMap.clear();
};

HeroManager* HeroManager::getInstance()
{
    if(!s_HeroManager){
        s_HeroManager = new HeroManager();
    }
    return s_HeroManager;
}

bool HeroManager::read()
{
    ctest* test = ctest::getInstanse();
    test->clear();
    if(!test->readCSV("HeroData.csv")){
        CCLOG("读取HeroData.csv错误");
        return false;
    }
    while (1) {
        HeroData data;

        data.id         = test->readAsInt();
        data.m_name     = test->readAsString();
        data.m_icon     = test->readAsString();
        data.m_fg       = test->readAsString();
        data.m_class1   = test->readAsInt();
        data.m_class2   = test->readAsInt();
        
        data.m_mining   = test->readAsInt();
        data.m_farm     = test->readAsInt();
        data. m_tree    = test->readAsInt();
        data.m_alchemy  = test->readAsInt();
        data.m_item     = test->readAsInt();
        data.m_weapon   = test->readAsInt();
        data.m_armor    = test->readAsInt();
        data.m_charm    = test->readAsInt();
        data.m_manage   = test->readAsInt();
        data.m_communication   = test->readAsInt();
        data. m_home    = test->readAsInt();
        data.m_search   = test->readAsInt();

        for (int i=0; i<9; i++) {
            data.m_baseStatus[i] = test->readAsInt();
        }
        for (int i=0; i<3; i++) {
            data.m_attackSkill[i] = test->readAsInt();
        }
        for (int i=0; i<3; i++) {
            data.m_passiveSkill[i] = test->readAsInt();
        }
        
        for (int i=0; i<9; i++) {
            data.m_addStatus[i] = test->readAsInt();
        }
        
        data.m_des                      = test->readAsString();
        data.m_album                    = test->readAsInt();
        
        m_heroDataMap.insert(HERODATA_MAP::value_type(data.id, data));
        
        if(!test->size())
            break;
    }
    
    MAX_HERODATA_NUM = m_heroDataMap.size();
    return true;
    
}

HeroRef HeroManager::createHero(S32 id)
{
    HERODATA_MAPITR it =  m_heroDataMap.find(id);
    if (it != m_heroDataMap.end()) {
        HeroRef hero(new Hero());
        
        hero->id = id;
        int temp = random(0, 8);
        hero->m_status = StatusManager::getInstance()->getStatus(it->second.m_baseStatus[temp]);
        hero->m_level = 1;
        
        return hero;
    }
    
    return NULL;
}

HeroRef HeroManager::createHeroRandom()
{
    int num = random(0, (int)m_heroDataMap.size()-1);
    
    HERODATA_MAPITR it = m_heroDataMap.begin();
    
    while (num != 0) {
        it++;
        num--;
    }
    
    if (it != m_heroDataMap.end()) {
        HeroRef hero(new Hero());
        
        hero->id = it->second.id;
        int temp = random(0, 8);
        hero->m_status = StatusManager::getInstance()->getStatus(it->second.m_baseStatus[temp]);
        hero->m_level = 1;
        return hero;
    }
    
    return nullptr;
}

HeroData* HeroManager::getHeroData(S32 heroId)
{
    HERODATA_MAPITR itr=m_heroDataMap.find(heroId);
    if(itr!=m_heroDataMap.end())
    {
        return &itr->second;
    }
    return NULL;
}
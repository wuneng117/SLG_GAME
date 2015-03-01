//
//  SkillData.cpp
//  lantisi
//
//  Created by ZHOUTAO on 14-6-11.
//
//

#include "SkillData.h"
#include "ctest.h"

static SkillManager *s_SkillDataManager = nullptr;

SkillManager::SkillManager()
{
    
}

SkillManager::~SkillManager()
{
    m_skillDataMap.clear();

}

SkillManager* SkillManager::getInstance()
{
    if(s_SkillDataManager == nullptr){
        s_SkillDataManager = new SkillManager();
    }
    return s_SkillDataManager;
}

bool SkillManager::read()
{
    ctest* test = ctest::getInstanse();
    test->clear();
    if(!test->readCSV("SkillData.csv")){
        CCLOG("读取SkillData.csv错误");
        return false;
    }
    while (1) {
        SkillData data;
        
        data.id                         = test->readAsInt();
        data.m_name                     = test->readAsString();
        test->readAsString();
        data.m_damage                   = test->readAsInt();
        data.m_skillType                = test->readAsInt();
        data.m_skillCastType            = test->readAsInt();
        data.m_target                   = test->readAsInt();
        data.m_region                   = test->readAsInt();
        data.m_effect                   = test->readAsInt();
        data.m_range_s                  = test->readAsInt();
        data.m_range_l                  = test->readAsInt();
        data.m_selfBuff                 = test->readAsInt();
        data.m_targetBuff               = test->readAsInt();
        data.m_selfStatus               = test->readAsInt();
        data.m_targetStatus             = test->readAsInt();
        data.m_des                      = test->readAsString();
        data.m_cost.HP                  = test->readAsInt();
        data.m_cost.MP                  = test->readAsInt();
        data.m_cost.time                = test->readAsInt();
        data.m_cost.sprite              = test->readAsInt();
        data.m_cast.castAnimation       = test->readAsString();
        data.m_cast.castSound           = test->readAsString();
        
        m_skillDataMap.insert(SKILLDATA_MAP::value_type(data.id, data));
        
        if(!test->size())
            break;
    }
    
    return true;
}


SkillData* SkillManager::getSkillData(unsigned int skillId)
{
    SKILLDATA_MAPITR itr = m_skillDataMap.find(skillId);
    if(itr != m_skillDataMap.end())
        return &(itr->second);
    
    return NULL;
}

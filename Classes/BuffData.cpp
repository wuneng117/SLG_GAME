//
//  BuffData.cpp
//  testproject
//
//  Created by ZHOUTAO on 15/1/23.
//
//

#include "BuffData.h"

static BuffDataManager* s_BuffDataManager = nullptr;

BuffDataManager::BuffDataManager()
{
    
}

BuffDataManager::~BuffDataManager()
{
    m_buffDataMap.clear();
}

BuffDataManager* BuffDataManager::getInstance()
{
    if(s_BuffDataManager == nullptr){
        s_BuffDataManager = new BuffDataManager();
    }
    return s_BuffDataManager;
}

BuffData* BuffDataManager::GetBuff(S32 buffId)
{
    BUFFDATA_MAPITR itr=m_buffDataMap.find(buffId);
    if(itr!=m_buffDataMap.end())
    {
        return &itr->second;
    }
    return NULL;
}

BuffRef BuffDataManager::createBuff(S32 buffId, HeroRef source, HeroRef target)
{
    
    BUFFDATA_MAPITR it = m_buffDataMap.find(buffId);
    if (it != m_buffDataMap.end()) {
        BuffData* data = &it->second;
        
        BuffRef buff(new Buff());
        buff->id = data->id;
        buff->m_source = source;
        buff->m_carrier = target;
        buff->m_time = data->m_time;
        buff->m_turn = data->m_turn;
        
        return buff;
    }
    
    return nullptr;
}

bool BuffDataManager::read()
{
    
}
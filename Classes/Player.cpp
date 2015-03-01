//
//  Player.cpp
//  collectionapp
//
//  Created by ZHOUTAO on 14-7-31.
//
//

#include "Player.h"
#include "HeroManager.h"
//#include "GameClientManager.h"

//extern unsigned int nowTime;

Player::Player():m_cpu(true)
{
}

Player::~Player()
{
}

bool Player::init()
{
    
    return true;
}

bool Player::createNew()
{
    HeroRef hero = HeroManager::getInstance()->createHero(10000003);
    if (hero && m_heroMap.find(hero->id) == m_heroMap.end()) {
        m_heroMap.insert(HERO_MAP::value_type(hero->id,hero));
    }
    
    HeroRef hero1 = HeroManager::getInstance()->createHero(10000002);
    if (hero1 && m_heroMap.find(hero1->id) == m_heroMap.end()) {
        m_heroMap.insert(HERO_MAP::value_type(hero1->id,hero1));
    }
    
    HeroRef hero2 = HeroManager::getInstance()->createHero(10000001);
    if (hero2 && m_heroMap.find(hero2->id) == m_heroMap.end()) {
        m_heroMap.insert(HERO_MAP::value_type(hero2->id,hero2));
    }
    
    return true;
}

bool Player::createNewRandom()
{
    //随机生成3个hero
    int tempi = 3;
    assert(tempi <= MAX_HERODATA_NUM);
    while (tempi > 0) {
        HeroRef hero = HeroManager::getInstance()->createHeroRandom();
        if (hero && m_heroMap.find(hero->id) == m_heroMap.end()) {
            m_heroMap.insert(HERO_MAP::value_type(hero->id,hero));
            tempi--;
        }
    }
    
    return true;
}

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

city::city()
{
    
}
city::~city()
{
    
}

U32 city::getMingingLv()
{
    if (m_miningLv < 10) {
        return 0;
    }
    else if(m_miningLv >=10 && m_miningLv < 30) {
        return 1;
    }
    else if(m_miningLv >=30 && m_miningLv < 60) {
        return 2;
    }
    else if(m_miningLv >=60 && m_miningLv < 100) {
        return 3;
    }
    else if(m_miningLv >=100 && m_miningLv < 150) {
        return 4;
    }
    else/*m_miningLv >= 150)*/{
        return 5;
    }
}

U32 city::getFarmLv()
{
    if (m_farmLv < 10) {
        return 0;
    }
    else if(m_farmLv >=10 && m_farmLv < 30) {
        return 1;
    }
    else if(m_farmLv >=30 && m_farmLv < 60) {
        return 2;
    }
    else if(m_farmLv >=60 && m_farmLv < 100) {
        return 3;
    }
    else if(m_farmLv >=100 && m_farmLv < 150) {
        return 4;
    }
    else/*m_farmLv >= 150)*/{
        return 5;
    }
}

U32 city::getTreeLv()
{
    if (m_treeLv < 10) {
        return 0;
    }
    else if(m_treeLv >=10 && m_treeLv < 30) {
        return 1;
    }
    else if(m_treeLv >=30 && m_treeLv < 60) {
        return 2;
    }
    else if(m_treeLv >=60 && m_treeLv < 100) {
        return 3;
    }
    else if(m_treeLv >=100 && m_treeLv < 150) {
        return 4;
    }
    else/*m_treeLv >= 150)*/{
        return 5;
    }
}

U32 city::getAlchemyLv()
{
    if (m_alchemyLv < 10) {
        return 0;
    }
    else if(m_alchemyLv >=10 && m_alchemyLv < 30) {
        return 1;
    }
    else if(m_alchemyLv >=30 && m_alchemyLv < 60) {
        return 2;
    }
    else if(m_alchemyLv >=60 && m_alchemyLv < 100) {
        return 3;
    }
    else if(m_alchemyLv >=100 && m_alchemyLv < 150) {
        return 4;
    }
    else/*m_alchemyLv >= 150)*/{
        return 5;
    }
}


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

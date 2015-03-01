//
//  GameClientManager.cpp
//  testproject
//
//  Created by ZHOUTAO on 15/1/26.
//
//

#include "GameClientManager.h"

GameClientManager* g_gameClientManager = nullptr;

GameClientManager::GameClientManager()
{
    m_player.m_cpu = false;
}

GameClientManager::~GameClientManager()
{
    
}

bool GameClientManager::init()
{
    return true;
}

bool GameClientManager::createNewGame()
{
    //玩家
    m_player.createNew();
    
    //电脑
    Player player;
    player.createNewRandom();
    m_playerMap.insert(PLAYER_MAP::value_type(1,player));
    
    return true;
}

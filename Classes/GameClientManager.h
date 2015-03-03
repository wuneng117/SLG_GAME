//
//  GameClientManager.h
//  testproject
//
//  Created by ZHOUTAO on 15/1/26.
//
//

#ifndef __testproject__GameClientManager__
#define __testproject__GameClientManager__

#include <stdio.h>
#include "Player.h"
#include "types.h"

class GameClientManager
{
public:
    
    typedef std::map<U32,Player> PLAYER_MAP;
    typedef PLAYER_MAP::iterator PLAYER_MAPITR;
    
    
    
    
    
    
    GameClientManager();
    ~GameClientManager();
    
    bool init();    //初始化
    
    bool createNewGame();   //生成新游戏
    bool readSaveData();    //读取存档
    

    PLAYER_MAP m_playerMap; // 所有玩家
    CITY_MAP   m_cityMap;   // 城市
    
    Player m_player;    //
    
    U32 m_turn;     //当前回合
};
extern GameClientManager* g_gameClientManager;

#endif /* defined(__testproject__GameClientManager__) */

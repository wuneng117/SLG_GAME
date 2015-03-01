//
//  MapMain_Scene.h
//  SLG_GAME
//
//  Created by ZHOUTAO on 15/2/22.
//
//

#ifndef __SLG_GAME__MapMain_Scene__
#define __SLG_GAME__MapMain_Scene__

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "CocosGUI.h"

class MapMain_Scene : public cocos2d::Layer
{
public:
    
    
    static cocos2d::Scene* createScene();
    
    MapMain_Scene();
    ~MapMain_Scene();
    virtual bool init();
    
    
    CREATE_FUNC(MapMain_Scene);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onCityTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    void onButtonTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    void onQuestTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    
//    void updateUnitList(Player::HERO_MAP& heroMap);     // 更新角色列表
//    void showUnit(HeroRef hero);      //显示英雄详细信息
private:
    
    cocos2d::ui::ScrollView* scrollview_map;  // 地图
    cocos2d::ui::Layout*     panel_city;      // city信息
    cocos2d::ui::Layout*     panel_info;      // 时间信息
};

#endif /* defined(__SLG_GAME__MapMain_Scene__) */

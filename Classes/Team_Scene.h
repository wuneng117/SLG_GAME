//
//  Team_Scene.h
//  SLG_GAME
//
//  Created by ZHOUTAO on 15/3/2.
//
//

#ifndef __SLG_GAME__Team_Scene__
#define __SLG_GAME__Team_Scene__

#include <stdio.h>

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "CocosGUI.h"
#include "Player.h"

class Team_Scene : public cocos2d::Layer
{
public:
    
    
    static cocos2d::Scene* createScene();
    
    Team_Scene();
    ~Team_Scene();
    virtual bool init();
    
    
    CREATE_FUNC(Team_Scene);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onListTurnEvent(cocos2d::Ref *obj, cocos2d::ui::PageView::EventType event);    // 队伍列表翻页回调
    void onListUnitTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type); // 队伍列表内单位点击回调
    void onSkillBtnTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type); // 技能按钮回调
    void onSkillPanelTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type);   // 技能界面触摸回调
    void onReturnTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type);   // 返回按钮回调
    
    void updateUnitList(Player::HERO_MAP& heroMap);     // 更新角色列表
    void showUnit(HeroRef hero);      //显示英雄详细信息
    void showSkill(Node* node, U32 skillId);    // 显示英雄技能

private:
    cocos2d::Node* m_root;  //cocostudio主节点
    cocos2d::ui::PageView* m_unitList;  // 角色列表
    cocos2d::ui::PageView* m_unitDetal; // 角色详细
    cocos2d::ui::Layout*   m_unitSkill;  // 角色技能界面
    
    cocos2d::Node* m_selectedUnitWidget;   //选择单位的框
};

#endif /* defined(__SLG_GAME__Team_Scene__) */

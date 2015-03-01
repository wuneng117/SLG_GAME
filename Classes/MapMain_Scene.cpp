//
//  MapMain_Scene.cpp
//  SLG_GAME
//
//  Created by ZHOUTAO on 15/2/22.
//
//

#include "MapMain_Scene.h"


USING_NS_CC;
USING_NS_TIMELINE;
using namespace cocos2d::ui;


MapMain_Scene::MapMain_Scene()
{
}

MapMain_Scene::~MapMain_Scene()
{
    
}

Scene* MapMain_Scene::createScene()
{
    auto scene = Scene::create();
    auto layer = MapMain_Scene::create();
    scene->addChild(layer);
    
    return scene;
}

bool MapMain_Scene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    //为层设置一个点对点触摸
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MapMain_Scene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(MapMain_Scene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //
    //-------------------------------------cocostudio读取-----------------------------------------------------------------
    //
    Node* rootNode = CSLoader::createNode("MapMain_Scene.csb");
    addChild(rootNode);
    
    
    scrollview_map = dynamic_cast<ScrollView*>(rootNode->getChildByName("ScrollView_Map"));    // 地图
    
    for (int i=1; i<15; i++) {
        Vector<Node*> nodeVec = scrollview_map->getChildByTag(i)->getChildren();
        
        for (Vector<Node*>::iterator it = nodeVec.begin(); it != nodeVec.end(); it++) {
            Button* city = dynamic_cast<Button*>(*it);
            std::stringstream str;
            str<<city->getTag();
            city->setTitleText(str.str());
            city->setTitleFontSize(50);
            city->setTitleColor(Color3B(255, 255, 0));
            city->setSwallowTouches(false);
            city->addTouchEventListener(CC_CALLBACK_2(MapMain_Scene::onCityTouchEvent, this));
        }
    }
    
    panel_city = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_City"));     // city信息
    panel_city->addTouchEventListener(CC_CALLBACK_2(MapMain_Scene::onCityTouchEvent,this));
    
    Vector<Node*> btnVec = rootNode->getChildByName("Node_Btn")->getChildren();     // 主按钮
    
    for (Vector<Node*>::iterator it = btnVec.begin(); it != btnVec.end(); it++) {
        Button* btn = dynamic_cast<Button*>(*it);
        btn->addTouchEventListener(CC_CALLBACK_2(MapMain_Scene::onButtonTouchEvent, this));
    }
    
    return true;
}

void MapMain_Scene::onCityTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    Button* city = dynamic_cast<Button*>(ref);
    
    //int tag = city->getTag()*city->getParent()->getTag();
    
    CCLOG("onCityTouchEvent:%d-%d",city->getParent()->getTag(),city->getTag());
}

void MapMain_Scene::onButtonTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    Node* unit_bg = dynamic_cast<Node*>(ref);
    
    int tag = unit_bg->getTag();
    
    CCLOG("onButtonTouchEvent:%d",tag);
}

void MapMain_Scene::onQuestTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    int tag = dynamic_cast<Node*>(ref)->getTag();
    
    CCLOG("onQuestTouchEvent:%d",tag);
    
}

bool MapMain_Scene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return true;
}

void MapMain_Scene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
}
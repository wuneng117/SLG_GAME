//
//  Team_Scene.cpp
//  SLG_GAME
//
//  Created by ZHOUTAO on 15/3/2.
//
//

#include "Team_Scene.h"
#include "Player.h"
#include "GameClientManager.h"
#include "SkillData.h"

USING_NS_CC;
USING_NS_TIMELINE;
using namespace cocos2d::ui;

const U32 PAGE_COLUMN = 5;
const U32 PAGE_ROW    = 4;
const U32 PAGE_NUM    = PAGE_COLUMN*PAGE_ROW;

Team_Scene::Team_Scene():m_selectedUnitWidget(nullptr)
{
}

Team_Scene::~Team_Scene()
{
    
}

Scene* Team_Scene::createScene()
{
    auto scene = Scene::create();
    auto layer = Team_Scene::create();
    scene->addChild(layer);
    
    return scene;
}

bool Team_Scene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    //为层设置一个点对点触摸
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Team_Scene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(Team_Scene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //
    //-------------------------------------cocostudio读取-----------------------------------------------------------------
    //
    m_root = CSLoader::createNode("Team_Scene.csb");
    addChild(m_root);
    
    
    m_unitList = dynamic_cast<PageView*>(m_root->getChildByName("PageView_UnitList"));    // 角色列表
    m_unitList->addEventListener(CC_CALLBACK_2(Team_Scene::onListTurnEvent,this));
    updateUnitList(g_gameClientManager->m_player.m_heroMap);
    
    m_unitDetal = dynamic_cast<PageView*>(m_root->getChildByName("PageView_Unit"));     // 角色信息
    dynamic_cast<Button*>(m_unitDetal->getChildByName("Panel_1")->getChildByName("skill"))->addTouchEventListener(CC_CALLBACK_2(Team_Scene::onSkillBtnTouchEvent,this));
    m_unitDetal->setVisible(false);
    
    m_unitSkill = dynamic_cast<Layout*>(m_root->getChildByName("Panel_SkillBg"));     // 角色技能
    m_unitSkill->addTouchEventListener(CC_CALLBACK_2(Team_Scene::onSkillPanelTouchEvent, this));
    m_unitSkill->setVisible(false);
    
    return true;
}

void Team_Scene::updateUnitList(Player::HERO_MAP& heroMap)
{
    m_unitList->removeAllPages();
    
    unsigned long max_page = heroMap.size()/PAGE_NUM + 1;
    
    m_root->getChildByName("Image_ArrowL")->setVisible(false);
    if (max_page>1) {
        m_root->getChildByName("Image_ArrowR")->setVisible(true);
    }
    else {
        m_root->getChildByName("Image_ArrowR")->setVisible(false);
    }
    
    Player::HERO_MAPITR it = heroMap.begin();
    
    for (int i=0; i<max_page; i++) {
        
        //创建页面
        cocos2d::ui::Layout* page = cocos2d::ui::Layout::create();
        page->setContentSize(Size(600,640));
        m_unitList->addPage(page);
        
        for (int j=0; j<PAGE_NUM; j++) {
            
            //创建每页显示的单位
            Node* unit = CSLoader::createNode("BattleReady_Scene_Unit.csb");
            unit->setPosition(30+j%PAGE_COLUMN*115,470-j/PAGE_COLUMN*150);
            
            Widget* unit_bg = dynamic_cast<Layout*>(unit->getChildByName("bg"));
            unit_bg->setTag(it->first);
            unit_bg->addTouchEventListener(CC_CALLBACK_2(Team_Scene::onListUnitTouchEvent, this));
            
            dynamic_cast<Widget*>(unit_bg->getChildByName("cover"))->setVisible(false);
            dynamic_cast<Widget*>(unit_bg->getChildByName("select"))->setVisible(false);
            
            
            HeroRef hero = it->second;
            HeroData* data = HeroManager::getInstance()->getHeroData(hero->id);
            
            dynamic_cast<Text*>(unit_bg->getChildByName("name"))->setString(data->m_name);
            
            std::stringstream str;
            str<<"Lv:"<<hero->m_level;
            dynamic_cast<Text*>(unit_bg->getChildByName("lv"))->setString(str.str());
            
            dynamic_cast<ImageView*>(unit_bg->getChildByName("icon"))->loadTexture(data->m_icon);
            
            
            m_unitList->addWidgetToPage((Widget*)unit, i, true);
            
            it++;
            if(it == heroMap.end())
                break;
        }
    }
}

void Team_Scene::showUnit(HeroRef hero)
{
    m_unitDetal->setVisible(true);

    HeroData* data = HeroManager::getInstance()->getHeroData(hero->id);
    assert(data);
    
    //第一页
    Layout* panel_1 = m_unitDetal->getPage(0);
    //dynamic_cast<ImageView*>(panel_1->getChildByName("fg"))->loadTexture("100x100_side_move.png");
    dynamic_cast<Text*>(panel_1->getChildByName("name"))->setString(data->m_name);
    
    std::stringstream str;
    str<<"Lv:"<<hero->m_level;
    dynamic_cast<Text*>(panel_1->getChildByName("lv"))->setString(str.str());
    
    Status& status = hero->m_status;
    
    str.str("");
    str<<"HP "<<status.HP<<"/"<<status.MaxHP;
    dynamic_cast<Text*>(panel_1->getChildByName("hp"))->setString(str.str());
    
    
    str.str("");
    str<<"MP "<<status.MP<<"/"<<status.MaxMP;
    dynamic_cast<Text*>(panel_1->getChildByName("mp"))->setString(str.str());
    
    str.str("");
    str<<"职业:"<<HeroClass[data->m_class1]<<" "<<HeroClass[data->m_class2];
    dynamic_cast<Text*>(panel_1->getChildByName("class"))->setString(str.str());
    
    str.str("");
    str<<"格斗:"<<status.ATKS;
    dynamic_cast<Text*>(panel_1->getChildByName("atk_s"))->setString(str.str());
    
    str.str("");
    str<<"射击:"<<status.ATKL;
    dynamic_cast<Text*>(panel_1->getChildByName("atk_l"))->setString(str.str());
    
    str.str("");
    str<<"防御:"<<status.DEF;
    dynamic_cast<Text*>(panel_1->getChildByName("def"))->setString(str.str());
    
    str.str("");
    str<<"回避:"<<status.Dodge;
    dynamic_cast<Text*>(panel_1->getChildByName("dodge"))->setString(str.str());
    
    str.str("");
    str<<"命中:"<<status.Hit;
    dynamic_cast<Text*>(panel_1->getChildByName("hit"))->setString(str.str());
    
    str.str("");
    str<<"移动:"<<status.MOV;
    dynamic_cast<Text*>(panel_1->getChildByName("move"))->setString(str.str());
    
    str.str("");
    str<<"探索:"<<data->m_search;
    dynamic_cast<Text*>(panel_1->getChildByName("search"))->setString(str.str());
    
    str.str("");
    str<<"挖矿:"<<data->m_mining;
    dynamic_cast<Text*>(panel_1->getChildByName("mining"))->setString(str.str());
    
    str.str("");
    str<<"农牧:"<<data->m_farm;
    dynamic_cast<Text*>(panel_1->getChildByName("farm"))->setString(str.str());
    
    str.str("");
    str<<"土木:"<<data->m_tree;
    dynamic_cast<Text*>(panel_1->getChildByName("tree"))->setString(str.str());
    
    str.str("");
    str<<"炼金:"<<data->m_alchemy;
    dynamic_cast<Text*>(panel_1->getChildByName("alchemy"))->setString(str.str());
    
    str.str("");
    str<<"合成:"<<data->m_item;
    dynamic_cast<Text*>(panel_1->getChildByName("item"))->setString(str.str());
    str.str("");
    str<<"锻造:"<<data->m_weapon;
    dynamic_cast<Text*>(panel_1->getChildByName("weapon"))->setString(str.str());
    
    str.str("");
    str<<"铸甲:"<<data->m_armor;
    dynamic_cast<Text*>(panel_1->getChildByName("armor"))->setString(str.str());
    
    str.str("");
    str<<"管理:"<<data->m_manage;
    dynamic_cast<Text*>(panel_1->getChildByName("manage"))->setString(str.str());
    
    str.str("");
    str<<"交涉:"<<data->m_communication;
    dynamic_cast<Text*>(panel_1->getChildByName("communication"))->setString(str.str());
    
    str.str("");
    str<<"珠宝:"<<data->m_charm;
    dynamic_cast<Text*>(panel_1->getChildByName("charm"))->setString(str.str());
    
    str.str("");
    str<<"家政:"<<data->m_home;
    dynamic_cast<Text*>(panel_1->getChildByName("home"))->setString(str.str());
    
    //第二页
    Layout* panel_2 = m_unitDetal->getPage(1);

    dynamic_cast<Text*>(panel_2->getChildByName("name"))->setString(data->m_name);
    
    //第三页
    Layout* panel_3 = m_unitDetal->getPage(2);
    
    dynamic_cast<Text*>(panel_3->getChildByName("name"))->setString(data->m_name);
    dynamic_cast<Text*>(panel_3->getChildByName("des"))->setString(data->m_des);
    
    //技能界面
    for (int i=0; i<3; i++) {
        showSkill(m_unitSkill->getChildren().at(0)->getChildByTag(i+1), data->m_attackSkill[i]);
    }
}

void Team_Scene::showSkill(Node* node, U32 skillId)
{
    if (skillId) {
        SkillData* skilldata = SkillManager::getInstance()->getSkillData(skillId);
        assert(skilldata);
        
        node->setVisible(true);
        
        std::stringstream str;
        str<<skilldata->m_range_s<<"-"<<skilldata->m_range_l;
        dynamic_cast<Text*>(node->getChildByName("range"))->setString(str.str());
        
        str.str("");
        str<<skilldata->m_damage;
        dynamic_cast<Text*>(node->getChildByName("damage"))->setString(str.str());
        
        str.str("");
        str<<skilldata->m_cost.time;
        dynamic_cast<Text*>(node->getChildByName("time"))->setString(str.str());
        
        str.str("");
        str<<skilldata->m_cost.MP;
        dynamic_cast<Text*>(node->getChildByName("cost"))->setString(str.str());
        
        dynamic_cast<Text*>(node->getChildByName("name"))->setString(skilldata->m_name);
        dynamic_cast<Text*>(node->getChildByName("des"))->setString("说明:"+skilldata->m_des);
    }
    else{
        node->setVisible(false);
    }
}

void Team_Scene::onListTurnEvent(cocos2d::Ref *obj, cocos2d::ui::PageView::EventType event)
{
    if(event != PageView::EventType::TURNING)
        return;
    
    Node* arrowL = m_root->getChildByName("Image_ArrowL");
    Node* arrowR = m_root->getChildByName("Image_ArrowR");
    arrowL->setVisible(true);
    arrowR->setVisible(true);
    
    
    if (m_unitList->getCurPageIndex() == 0)
        arrowL->setVisible(false);
    
    Vector<Layout*> vec = m_unitList->getPages();
    
    if (m_unitList->getCurPageIndex() == vec.size()-1)
        arrowR->setVisible(false);
}

void Team_Scene::onListUnitTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    Node* node = dynamic_cast<Node*>(ref);
    
    if(node->getChildByName("cover")->isVisible())
        return;
    
    int tag = node->getTag();
    
    CCLOG("onListUnitTouchEvent: %d",tag);
    
    if (m_selectedUnitWidget)
        m_selectedUnitWidget->getChildByName("select")->setVisible(false);
    
    m_selectedUnitWidget = node;
    m_selectedUnitWidget->getChildByName("select")->setVisible(true);
    
    HeroRef hero = g_gameClientManager->m_player.m_heroMap.find(tag)->second;
    assert(hero);
    
    showUnit(hero);
}

void Team_Scene::onSkillBtnTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    m_unitSkill->setVisible(true);
}

void Team_Scene::onSkillPanelTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    Widget* widget = dynamic_cast<Widget*>(ref);
    
    Point point = widget->getTouchEndPosition();
    if(!widget->getChildByName("Panel_Skill")->getBoundingBox().containsPoint(point))
        widget->setVisible(false);
}

void Team_Scene::onReturnTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;

    Director::getInstance()->popScene();
}



bool Team_Scene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return true;
}

void Team_Scene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
}
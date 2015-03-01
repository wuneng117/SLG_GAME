//
//  BattleReady_Scene.cpp
//  testproject
//
//  Created by ZHOUTAO on 15/2/13.
//
//

#include "BattleReady_Scene.h"
#include "HelloWorldScene.h"
#include "Player.h"
#include "GameClientManager.h"
#include "SkillData.h"
#include "GraySprite.h"

USING_NS_CC;
USING_NS_TIMELINE;
using namespace cocos2d::ui;

const U32 PAGE_COLUMN = 5;
const U32 PAGE_ROW    = 4;
const U32 PAGE_NUM    = PAGE_COLUMN*PAGE_ROW;

BattleReady_Scene::BattleReady_Scene():m_selectedUnitWidget(nullptr)
{
    memset(m_teamUnitWidget, 0, 6*sizeof(Node*));
}

BattleReady_Scene::~BattleReady_Scene()
{
    
}

Scene* BattleReady_Scene::createScene()
{
    auto scene = Scene::create();
    auto layer = BattleReady_Scene::create();
    scene->addChild(layer);
    
    return scene;
}

bool BattleReady_Scene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    //为层设置一个点对点触摸
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(BattleReady_Scene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(BattleReady_Scene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //
    //-------------------------------------cocostudio读取-----------------------------------------------------------------
    //
    Node* rootNode = CSLoader::createNode("BattleReady_Scene.csb");
    addChild(rootNode);
    
    
    m_unitList = dynamic_cast<PageView*>(rootNode->getChildByName("PageView_UnitList"));    // 角色列表
    m_unitList->addEventListener(CC_CALLBACK_2(BattleReady_Scene::onListTurnEvent,this));
    updateUnitList(g_gameClientManager->m_player.m_heroMap);
    
    m_unit = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Unit"));     // 角色信息
    dynamic_cast<Button*>(m_unit->getChildByName("detal"))->addTouchEventListener(CC_CALLBACK_2(BattleReady_Scene::onUnitDetalTouchEvent,this));
    //dynamic_cast<ImageView*>(m_unit->getChildByName("fg"))->loadTexture("100x100_side_move.png");
    dynamic_cast<Text*>(m_unit->getChildByName("name"))->setString("------------");
    dynamic_cast<Text*>(m_unit->getChildByName("lv"))->setString("Lv:--");
    dynamic_cast<Text*>(m_unit->getChildByName("hp"))->setString("HP ----/----");
    dynamic_cast<Text*>(m_unit->getChildByName("mp"))->setString("MP ----/----");
    dynamic_cast<Text*>(m_unit->getChildByName("class"))->setString("职业:------");
    dynamic_cast<Text*>(m_unit->getChildByName("atk_s"))->setString("格斗:------");
    dynamic_cast<Text*>(m_unit->getChildByName("atk_l"))->setString("射击:------");
    dynamic_cast<Text*>(m_unit->getChildByName("def"))->setString("防御:------");
    dynamic_cast<Text*>(m_unit->getChildByName("dodge"))->setString("回避:------");
    dynamic_cast<Text*>(m_unit->getChildByName("hit"))->setString("命中:------");
    dynamic_cast<Text*>(m_unit->getChildByName("move"))->setString("移动:------");
    
    m_unitTeam = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_UnitTeam"));     // 角色队伍
    dynamic_cast<Button*>(m_unitTeam->getChildByName("start"))->addTouchEventListener(CC_CALLBACK_2(BattleReady_Scene::onStartTouchEvent, this));
    
    for (int i=0; i<6; i++) {
        Node* unit = CSLoader::createNode("BattleReady_Scene_Team.csb");
        unit->setPosition(15+i%2*245, 190-i/2*90);
        
        Widget* unit_bg = dynamic_cast<Widget*>(unit->getChildByName("bg"));
        unit_bg->addTouchEventListener(CC_CALLBACK_2(BattleReady_Scene::onTeamUnitTouchEvent, this));
        unit_bg->setTag(i);
        
        dynamic_cast<ImageView*>(unit_bg->getChildByName("icon"))->loadTexture("100x100_side_move.png");
        dynamic_cast<Text*>(unit_bg->getChildByName("name"))->setString("------------");
        dynamic_cast<Text*>(unit_bg->getChildByName("lv"))->setString("Lv:--");
        dynamic_cast<Text*>(unit_bg->getChildByName("hp"))->setString("HP ----/----");
        dynamic_cast<Text*>(unit_bg->getChildByName("mp"))->setString("MP ----/----");
        
        unit_bg->getChildByName("select")->setVisible(false);
        
        m_unitTeam->addChild(unit);

    }
    
    
//    //角色数据
//    panel_myUnit = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_myUnit"));
//    panel_enermyUnit = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_enermyUnit"));
//    panel_enermyUnit->setVisible(false);
//    panel_myUnit->setVisible(false);

    
    
    
    return true;
}

void BattleReady_Scene::updateUnitList(Player::HERO_MAP& heroMap)
{
    m_unitList->removeAllPages();
    
    int max_page = heroMap.size()/PAGE_NUM + 1;
    
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
            unit_bg->addTouchEventListener(CC_CALLBACK_2(BattleReady_Scene::onListUnitTouchEvent, this));
            
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

void BattleReady_Scene::showUnit(HeroRef hero)
{
    m_unit->getChildByName("detal")->setTag(hero->id);
    
    HeroData* data = HeroManager::getInstance()->getHeroData(hero->id);
    assert(data);
    
    //dynamic_cast<ImageView*>(m_unit->getChildByName("fg"))->loadTexture("100x100_side_move.png");
    dynamic_cast<Text*>(m_unit->getChildByName("name"))->setString(data->m_name);
    
    std::stringstream str;
    str<<"Lv:"<<hero->m_level;
    dynamic_cast<Text*>(m_unit->getChildByName("lv"))->setString(str.str());

    Status& status = hero->m_status;
    
    str.str("");
    str<<"HP "<<status.HP<<"/"<<status.MaxHP;
    dynamic_cast<Text*>(m_unit->getChildByName("hp"))->setString(str.str());
    
    
    str.str("");
    str<<"MP "<<status.MP<<"/"<<status.MaxMP;
    dynamic_cast<Text*>(m_unit->getChildByName("mp"))->setString(str.str());
    
    str.str("");
    str<<"职业:"<<HeroClass[data->m_class1]<<" "<<HeroClass[data->m_class2];
    dynamic_cast<Text*>(m_unit->getChildByName("class"))->setString(str.str());
    
    str.str("");
    str<<"格斗:"<<status.ATKS;
    dynamic_cast<Text*>(m_unit->getChildByName("atk_s"))->setString(str.str());
    
    str.str("");
    str<<"射击:"<<status.ATKL;
    dynamic_cast<Text*>(m_unit->getChildByName("atk_l"))->setString(str.str());
    
    str.str("");
    str<<"防御:"<<status.DEF;
    dynamic_cast<Text*>(m_unit->getChildByName("def"))->setString(str.str());
    
    str.str("");
    str<<"回避:"<<status.Dodge;
    dynamic_cast<Text*>(m_unit->getChildByName("dodge"))->setString(str.str());
    
    str.str("");
    str<<"命中:"<<status.Hit;
    dynamic_cast<Text*>(m_unit->getChildByName("hit"))->setString(str.str());
    
    str.str("");
    str<<"移动:"<<status.MOV;
    dynamic_cast<Text*>(m_unit->getChildByName("move"))->setString(str.str());
}

void BattleReady_Scene::onListTurnEvent(cocos2d::Ref *obj, cocos2d::ui::PageView::EventType event)
{
    if(event != PageView::EventType::TURNING)
        return;
    
    if(m_unitList->getCurPageIndex() == 0){
    }
    
    Vector<Layout*> vec = m_unitList->getPages();
    
    if (m_unitList->getCurPageIndex() == vec.size()-1) {
        
    }
}

void BattleReady_Scene::onListUnitTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
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

void BattleReady_Scene::onTeamUnitTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    Node* unit_bg = dynamic_cast<Node*>(ref);
    
    int tag = unit_bg->getTag();
    
    CCLOG("onTeamUnitTouchEvent:%d",tag);
    
    
    if (!m_selectedUnitWidget) {
        if (m_teamUnitWidget[tag]) {
            dynamic_cast<ImageView*>(unit_bg->getChildByName("icon"))->loadTexture("100x100_side_move.png");
            dynamic_cast<Text*>(unit_bg->getChildByName("name"))->setString("------------");
            dynamic_cast<Text*>(unit_bg->getChildByName("lv"))->setString("Lv:--");
            dynamic_cast<Text*>(unit_bg->getChildByName("hp"))->setString("HP ----/----");
            dynamic_cast<Text*>(unit_bg->getChildByName("mp"))->setString("MP ----/----");
            
            m_teamUnitWidget[tag]->getChildByName("cover")->setVisible(false);
            m_teamUnitWidget[tag] = nullptr;
        }
    }
    else {
        
        int id = m_selectedUnitWidget->getTag();
        
        HeroRef hero = g_gameClientManager->m_player.m_heroMap.find(id)->second;
        assert(hero);
        
        HeroData* data = HeroManager::getInstance()->getHeroData(id);
        assert(data);
        
        dynamic_cast<ImageView*>(unit_bg->getChildByName("icon"))->loadTexture(data->m_icon);
        dynamic_cast<Text*>(unit_bg->getChildByName("name"))->setString(data->m_name);
        
        std::stringstream str;
        str<<"Lv:"<<hero->m_level;
        dynamic_cast<Text*>(unit_bg->getChildByName("lv"))->setString(str.str());
        
        str.str("");
        str<<"HP "<<hero->m_status.HP<<"/"<<hero->m_status.MaxHP;
        dynamic_cast<Text*>(unit_bg->getChildByName("hp"))->setString(str.str());
        
        str.str("");
        str<<"MP "<<hero->m_status.MP<<"/"<<hero->m_status.MaxMP;
        dynamic_cast<Text*>(unit_bg->getChildByName("mp"))->setString(str.str());
        
        if (m_teamUnitWidget[tag]) {
            m_teamUnitWidget[tag]->getChildByName("cover")->setVisible(false);
        }
        
        m_teamUnitWidget[tag] = m_selectedUnitWidget;
        m_selectedUnitWidget = nullptr;
        m_teamUnitWidget[tag]->getChildByName("cover")->setVisible(true);
        m_teamUnitWidget[tag]->getChildByName("select")->setVisible(false);
    }
}
                                                                                  
void BattleReady_Scene::onUnitDetalTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    int tag = dynamic_cast<Node*>(ref)->getTag();
    
    CCLOG("onUnitDetalTouchEvent:%d",tag);
    
}

void BattleReady_Scene::onStartTouchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    std::vector<HeroRef> myHero;
    std::vector<HeroRef> enermyHero;
    
    for (int i=0; i<6; i++) {
        if(!m_teamUnitWidget[i])
            continue;
        
        HeroRef hero = g_gameClientManager->m_player.m_heroMap.find(m_teamUnitWidget[i]->getTag())->second;
        assert(hero);
        
        myHero.push_back(hero);
    }
    
    auto scene = HelloWorld::createScene(myHero, enermyHero);
    
    Director::getInstance()->replaceScene(scene);
}



bool BattleReady_Scene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return true;
}

void BattleReady_Scene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
}
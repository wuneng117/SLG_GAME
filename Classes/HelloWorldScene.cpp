#include "HelloWorldScene.h"
#include "Player.h"
#include "GameClientManager.h"
#include "SkillData.h"
#include "GraySprite.h"

USING_NS_CC;
USING_NS_TIMELINE;
using namespace cocos2d::ui;

HelloWorld::HelloWorld()
{
    memset(myUnit, 0, sizeof(myUnit));
    memset(enermyUnit, 0, sizeof(enermyUnit));
}

HelloWorld::~HelloWorld()
{
    for (int i=0; i<6; i++) {
        if (myUnit[i])
            delete myUnit[i];
        if (enermyUnit[i])
            delete enermyUnit[i];
    }
    
    for (int i=0; i<MAP_NUM; i++) {
        delete map[i];
        map[i] = nullptr;
    }
    
    delete mapHelper;
}

Scene* HelloWorld::createScene(std::vector<HeroRef> myHero, std::vector<HeroRef> enermyHero)
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    
    layer->_init(myHero, enermyHero);
    
    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void HelloWorld::_init(std::vector<HeroRef> myHero, std::vector<HeroRef> enermyHero)
{
    //为层设置一个点对点触摸
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //
    //-------------------------------------cocostudio读取-----------------------------------------------------------------
    //
    Node* rootNode = CSLoader::createNode("Battle_Scene.csb");
    addChild(rootNode);
    
    //地图层
    mapLayer = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Map"));
    mapLayer->setTouchEnabled(false);
    
    //角色数据
    panel_myUnit = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_myUnit"));
    panel_enermyUnit = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_enermyUnit"));
    panel_enermyUnit->setVisible(false);
    panel_myUnit->setVisible(false);
    
    //角色菜单
    actionMenu = dynamic_cast<Node*>(rootNode->getChildByName("Node_Menu"));
    actionMenu->setVisible(false);
    for(int i=1;i<5;i++){
        Button* menuButton = dynamic_cast<Button*>(actionMenu->getChildByTag(i));
        menuButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::onActionMenuTouchEvent, this));
    }
    
    //回合菜单
    turnMenu = dynamic_cast<Node*>(rootNode->getChildByName("Node_TurnMenu"));
    turnMenu->setVisible(false);
    for(int i=1;i<5;i++){
        Button* menuButton = dynamic_cast<Button*>(turnMenu->getChildByTag(i));
        menuButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::onTurnMenuTouchEvent, this));
    }
    
    //选中框
    selectImage = dynamic_cast<ImageView*>(Helper::seekWidgetByName((Widget*)rootNode, "select_image"));
    selectImage->setLocalZOrder(SELECTIMAGE_TAG);
    
    //技能层
    panel_skill = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Skill"));
    for (int i=1; i<4; i++) {
        Button* btn = dynamic_cast<Button*>(panel_skill->getChildByTag(i)->getChildByTag(i));
        btn->addTouchEventListener(CC_CALLBACK_2(HelloWorld::onSkillTouchEvent, this));
    }
    panel_skill->setVisible(false);
    panel_skill->setSwallowTouches(true);
    
    //战斗开始信息
    panel_battle = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Battle"));
    panel_battle->setVisible(false);
    dynamic_cast<Button*>(panel_battle->getChildByName("btn"))->addTouchEventListener(CC_CALLBACK_2(HelloWorld::onBattleTouchEvent, this));
    
    //攻击特效
    attack_effect_1 = CSLoader::createNode("attack_effect_1.csb");
    attack_effect_1->setVisible(false);
    mapLayer->addChild(attack_effect_1,ATTACKEFFECT_TAG);
    //--------------------------------------------------------------------------------------------------------------------
    //
    
    //管理类初始化
    mapHelper = new MapHelper(this);
    
    //变量初始化
    touchState = eTouchState::SelectUnit;
    lastTouchState = eTouchState::NullState;
    selectedUnit = NULL;
    myTurn = true;
    //
    //----------------------------------------------------地图初始化----------------------------------------------------
    //
    
    for(int i=0;i<MAP_NUM;i++){
        map[i] = new sMapPoint();
        map[i]->pos = Point(i%MAP_COLUMN, i/MAP_COLUMN);
    }
    
    for(int i=0;i<MAP_NUM;i++){
        mapMove[i] = Sprite::create("100x100_side_move.png");
        mapMove[i]->setPosition(Point(TILE_WIDTH/2+TILE_WIDTH*(i%MAP_COLUMN),TILE_HEIGHT/2+TILE_HEIGHT*(MAP_ROW-1-i/MAP_COLUMN)));
        mapMove[i]->setScale(0.75);
        mapMove[i]->setVisible(false);
        mapLayer->addChild(mapMove[i]);
        
        mapAtk[i] = Sprite::create("100x100_side_def.png");
        mapAtk[i]->setPosition(Point(TILE_WIDTH/2+TILE_WIDTH*(i%MAP_COLUMN),TILE_HEIGHT/2+TILE_HEIGHT*(MAP_ROW-1-i/MAP_COLUMN)));
        mapAtk[i]->setScale(0.75);
        mapAtk[i]->setVisible(false);
        mapLayer->addChild(mapAtk[i], MAPATTACK_TAG);
    }
    //
    //-----------------------------------------------------------------------------------------------------------------
    //
    
    //
    //----------------------------------------------------单位初始化----------------------------------------------------
    //
    //我方
    Player::HERO_MAP& _map = g_gameClientManager->m_player.m_heroMap;
    Player::HERO_MAPITR _it = _map.begin();
    for (int i=0; i<6; i++) {
        if (_it != _map.end()) {
            myUnit[i] = new sMapUnit();
            
            HeroRef hero = _it->second;
            
            HeroData* data = HeroManager::getInstance()->getHeroData(hero->id);
            assert(data);
            
            myUnit[i]->m_force = 0;
            myUnit[i]->hero = hero;
            myUnit[i]->move = hero->m_status.MOV;
            myUnit[i]->icon = Sprite::create(data->m_icon);
            mapLayer->addChild(myUnit[i]->icon);
            
            myUnit[i]->icon_gray = GraySprite::create(data->m_icon);
            myUnit[i]->icon_gray->setVisible(false);
            myUnit[i]->icon_gray->setPosition(myUnit[i]->icon->getContentSize().width/2, myUnit[i]->icon->getContentSize().height/2);
            myUnit[i]->icon->addChild(myUnit[i]->icon_gray);
            
            //纪录技能次数
            for (int j=0; j<3; j++) {
                SkillData* _data = SkillManager::getInstance()->getSkillData(data->m_attackSkill[j]);
                if (_data) {
                    myUnit[i]->attackSkill_times[j] = _data->m_cost.time;
                }
                else {
                    myUnit[i]->attackSkill_times[j] = 0;
                }
            }
            
            //随即位置
            while (1) {
                Point pos = Point(random(0, 3),random(0, 4));
                if (!getUnitByPos(pos)) {
                    setUnitPosition(myUnit[i], pos);
                    break;
                }
            }
            _it++;
        }
        else{
            myUnit[i] = nullptr;
        }
    }
    
    //对方
    Player::HERO_MAP &map = g_gameClientManager->m_playerMap.find(1)->second.m_heroMap;
    Player::HERO_MAPITR it = map.begin();
    for (int i=0; i<6; i++) {
        if (it != map.end()) {
            enermyUnit[i] = new sMapUnit();
            
            HeroRef hero = it->second;
            
            HeroData* data = HeroManager::getInstance()->getHeroData(hero->id);
            assert(data);
            
            enermyUnit[i]->m_force = 1;
            enermyUnit[i]->hero = hero;
            enermyUnit[i]->move = hero->m_status.MOV;
            enermyUnit[i]->icon = Sprite::create(data->m_icon);
            mapLayer->addChild(enermyUnit[i]->icon);
            
            enermyUnit[i]->icon_gray = GraySprite::create(data->m_icon);
            enermyUnit[i]->icon_gray->setVisible(false);
            enermyUnit[i]->icon_gray->setPosition(enermyUnit[i]->icon->getContentSize().width/2,
                                                  enermyUnit[i]->icon->getContentSize().height/2);
            enermyUnit[i]->icon->addChild(enermyUnit[i]->icon_gray);
            
            //纪录技能次数
            for (int j=0; j<3; j++) {
                SkillData* _data = SkillManager::getInstance()->getSkillData(data->m_attackSkill[j]);
                if (_data) {
                    enermyUnit[i]->attackSkill_times[j] = _data->m_cost.time;
                }
                else {
                    enermyUnit[i]->attackSkill_times[j] = 0;
                }
            }
            
            while (1) {
                Point pos = Point(random(MAP_COLUMN-4, MAP_COLUMN-1),random(0, 4));
                if (!getUnitByPos(pos)) {
                    setUnitPosition(enermyUnit[i], pos);
                    break;
                }
            }
            it++;
        }
        else{
            enermyUnit[i] = nullptr;
        }
    }
    
    //    for(int i=0;i<4;i++){
    //        enermyUnit[i] = new sMapUnit();
    //        enermyUnit[i]->icon = Sprite::create("icon_1000001.png");
    //        mapLayer->addChild(enermyUnit[i]->icon);
    //        enermyUnit[i]->move = 3;
    //    }
    //
    //    setUnitPosition(enermyUnit[0], Point(3,3));
    //    setUnitPosition(enermyUnit[1], Point(2,2));
    //    setUnitPosition(enermyUnit[2], Point(4,2));
    //    setUnitPosition(enermyUnit[3], Point(3,2));
    
    //
    //-----------------------------------------------------------------------------------------------------------------
    //
    
}

bool HelloWorld::setUnitPosition(sMapUnit *unit, cocos2d::Point pos)
{
    if(pos.x < 0 || pos.x >= MAP_COLUMN ||pos.y< 0 || pos.y >= MAP_ROW)
        return false;
    
    Point point = posToTouch(pos);
    unit->pos = pos;
    unit->icon->setPosition(point);
    
    return true;
}

bool HelloWorld::moveUnitToPos(sMapUnit *unit, cocos2d::Point pos)
{
    Vector<FiniteTimeAction*> actLine = mapHelper->moveUnitToPos(unit, pos);
    mapHelper->hideMove();
    
    //没有移动路径就会到选择单位状态
    if(actLine.size() == 0){
        touchState = eTouchState::SelectUnit;
        selectedUnit = nullptr;
        return false;
    }
    
    selectImage->setPosition(posToTouch(pos));
    unit->l_pos = unit->pos;
    unit->pos = pos;
    
    touchState = eTouchState::Moving;
    auto callback = CallFunc::create([&](){
        touchState = eTouchState::SelectMenuAfterMove;
        showActionMenu(selectedUnit->pos, true);
        });
    actLine.pushBack(callback);
    
    auto sequence = Sequence::create(actLine);
    
    unit->icon->runAction(sequence);
    
    actLine.clear();
    
    return true;
}

void HelloWorld::removeUnit(sMapUnit* unit)
{
    // 修改 删除对应的buff
    
    delete unit;
    unit = nullptr;
    
    // 死亡动画
}

void HelloWorld::selectUnitForAct(cocos2d::Point point)
{
    Point pos = touchToPos(point);
    sMapUnit* unit = getUnitByPos(pos);
    if(unit){
        if (unit->activity == false) {
            selectedUnit = unit;
            touchState = eTouchState::SelectMenu;
            showActionMenu(selectedUnit->pos);
        }
        if(unit->m_force == 0)
            showPanelMyUnit(unit);
        else
            showPanelEnermyUnit(unit);
    }
    else {
        turnMenu->setVisible(true);
        Point _point = posToTouch(pos);
        turnMenu->setPosition(mapLayer->getPositionX()+_point.x, mapLayer->getPositionY()+_point.y);
    }
    
    Point tempPoint = posToTouch(pos);
    selectImage->setPosition(tempPoint);
}

void HelloWorld::updateUnitPos()
{
    for (int i=0; i<6; i++) {
        
        if (enermyUnit[i]) {
            enermyUnit[i]->pos = touchToPos(enermyUnit[i]->icon->getPosition());
        }
        
        if (myUnit[i]) {
            myUnit[i]->pos = touchToPos(myUnit[i]->icon->getPosition());
        }
    }
}

void HelloWorld::showPanelMyUnit(sMapUnit* unit)
{
    panel_myUnit->setVisible(true);
    
    HeroRef hero = unit->hero;
    
    std::stringstream str;
    str<<hero->m_level;
    dynamic_cast<Text*>(panel_myUnit->getChildByName("LV"))->setString(str.str());
    
    Status status = hero->m_status;
    
    str.str("");
    str<<status.HP<<"/"<<status.MaxHP;
    dynamic_cast<Text*>(panel_myUnit->getChildByName("HP"))->setString(str.str());
    str.str("");
    str<<status.MP<<"/"<<status.MaxMP;
    dynamic_cast<Text*>(panel_myUnit->getChildByName("MP"))->setString(str.str());
    str.str("");
    str<<status.ATKS;
    dynamic_cast<Text*>(panel_myUnit->getChildByName("ATKS"))->setString(str.str());
    str.str("");
    str<<status.ATKL;
    dynamic_cast<Text*>(panel_myUnit->getChildByName("ATKL"))->setString(str.str());
    str.str("");
    str<<status.Dodge;
    dynamic_cast<Text*>(panel_myUnit->getChildByName("Dodge"))->setString(str.str());
    str.str("");
    str<<status.Hit;
    dynamic_cast<Text*>(panel_myUnit->getChildByName("Hit"))->setString(str.str());
    str.str("");
    str<<status.DEF;
    dynamic_cast<Text*>(panel_myUnit->getChildByName("DEF"))->setString(str.str());

    HeroData* data = HeroManager::getInstance()->getHeroData(unit->hero->id);
    assert(data);
    
    dynamic_cast<Text*>(panel_myUnit->getChildByName("name"))->setString(data->m_name);
    str.str("");
    str<<HeroClass[data->m_class1]<<" "<<HeroClass[data->m_class2];
    dynamic_cast<Text*>(panel_myUnit->getChildByName("Class"))->setString(str.str());
}

void HelloWorld::showPanelEnermyUnit(sMapUnit* unit)
{
    panel_enermyUnit->setVisible(true);
    
    HeroRef hero = unit->hero;
    
    std::stringstream str;
    str<<hero->m_level;
    dynamic_cast<Text*>(panel_enermyUnit->getChildByName("LV"))->setString(str.str());
    
    Status status = hero->m_status;
    
    str.str("");
    str<<status.HP<<"/"<<status.MaxHP;
    dynamic_cast<Text*>(panel_enermyUnit->getChildByName("HP"))->setString(str.str());
    str.str("");
    str<<status.MP<<"/"<<status.MaxMP;
    dynamic_cast<Text*>(panel_enermyUnit->getChildByName("MP"))->setString(str.str());
    str.str("");
    str<<status.ATKS;
    dynamic_cast<Text*>(panel_enermyUnit->getChildByName("ATKS"))->setString(str.str());
    str.str("");
    str<<status.ATKL;
    dynamic_cast<Text*>(panel_enermyUnit->getChildByName("ATKL"))->setString(str.str());
    str.str("");
    str<<status.Dodge;
    dynamic_cast<Text*>(panel_enermyUnit->getChildByName("Dodge"))->setString(str.str());
    str.str("");
    str<<status.Hit;
    dynamic_cast<Text*>(panel_enermyUnit->getChildByName("Hit"))->setString(str.str());
    str.str("");
    str<<status.DEF;
    dynamic_cast<Text*>(panel_enermyUnit->getChildByName("DEF"))->setString(str.str());
    
    HeroData* data = HeroManager::getInstance()->getHeroData(unit->hero->id);
    assert(data);
    
    dynamic_cast<Text*>(panel_enermyUnit->getChildByName("name"))->setString(data->m_name);
    str.str("");
    str<<HeroClass[data->m_class1]<<" "<<HeroClass[data->m_class2];
    dynamic_cast<Text*>(panel_enermyUnit->getChildByName("Class"))->setString(str.str());
}


void HelloWorld::showSkill(Node* node, U32 skillId)
{
    if (skillId) {
        SkillData* skilldata = SkillManager::getInstance()->getSkillData(skillId);
        assert(skilldata);
        
        node->setVisible(true);
        
        std::stringstream str;
        str<<skilldata->m_range_s<<"~"<<skilldata->m_range_l;
        dynamic_cast<Text*>(node->getChildByName("range"))->setString(str.str());
        
        str.str("");
        str<<skilldata->m_damage;
        dynamic_cast<Text*>(node->getChildByName("damage"))->setString(str.str());
        
        //修改，技能次数应该为战斗界面临时保存的
        str.str("");
        str<<skilldata->m_cost.time;
        dynamic_cast<Text*>(node->getChildByName("time"))->setString(str.str());
        
        str.str("");
        str<<skilldata->m_cost.MP;
        dynamic_cast<Text*>(node->getChildByName("cost"))->setString(str.str());
        
        dynamic_cast<Text*>(node->getChildByName("name"))->setString(skilldata->m_name);
        dynamic_cast<Text*>(node->getChildByName("des"))->setString("说明："+skilldata->m_des);
    }
    else{
        node->setVisible(false);
    }
}
void HelloWorld::showActionMenu(Point pos, bool bMoved /*=true*/)
{
    actionMenu->setVisible(true);
    
    Point point = posToTouch(pos);
    actionMenu->setPosition(Point(mapLayer->getPositionX()+point.x,mapLayer->getPositionY()+point.y));
    
    if (bMoved) {
        actionMenu->getChildByTag(1)->setVisible(false);
    }
    else {
        actionMenu->getChildByTag(1)->setVisible(true);
    }
}

void HelloWorld::showPanelSkill()
{
    if(!selectedUnit || !selectedUnit->hero)
        return;
    
    panel_skill->setVisible(true);
    
    HeroData* herodata = HeroManager::getInstance()->getHeroData(selectedUnit->hero->id);
    assert(herodata);
    
    for (int i=0; i<3; i++) {
        showSkill(panel_skill->getChildByTag(i+1), herodata->m_attackSkill[i]);
    }
}

void HelloWorld::showPanelSprite()
{
    if(!selectedUnit || !selectedUnit->hero)
        return;
    
}

void HelloWorld::showPanelBattle()
{
    float hit_gpc = calculateHit(selectedUnit, targetUnit);

    panel_battle->setVisible(true);
    
    std::stringstream str;
    str<<selectedSkill->m_name;
    dynamic_cast<Text*>(panel_battle->getChildByName("name"))->setString(str.str());
    
    str.str("");
    str<<"命中率: "<<hit_gpc;
    dynamic_cast<Text*>(panel_battle->getChildByName("hit"))->setString(str.str());
}

bool HelloWorld::backupLastState()
{
    if(lastTouchState == eTouchState::NullState)
        return false;
    
    switch (lastTouchState) {
        case eTouchState::SelectMenuAfterMove:
        {
            showActionMenu(selectedUnit->pos, true);
            break;
        }
        case eTouchState::SelectMenu:
        {
            showActionMenu(selectedUnit->pos);
            break;
        }
        default:
            break;
    }
    //panel_sprite->setVisible(false);
    panel_skill->setVisible(false);
    touchState = lastTouchState;
    lastTouchState = eTouchState::NullState;
    
    return true;
}

void HelloWorld::selectTargetForSkill()
{
    switch (selectedSkill->m_skillCastType) {
        case SkillData::eSkillCastType::Cast_None:
        {
            castSkill(selectedUnit, selectedUnit, selectedSkill, selectedSkillPos);
            selectedUnitActivityEnd();
            break;
        }
            
        case SkillData::eSkillCastType::Cast_Point:case SkillData::eSkillCastType::Cast_Region:case SkillData::eSkillCastType::Cast_All:
        {
            touchState = eTouchState::SelectUnitForSkill;
            
            //高亮攻击范围
            Point pos = selectedUnit->pos;
            for (int i=0; i<MAP_NUM; i++) {
                int x = i%MAP_COLUMN;
                int y = i/MAP_COLUMN;
                if (abs(x-pos.x)+abs(y-pos.y) >= selectedSkill->m_range_s && abs(x-pos.x)+abs(y-pos.y) <= selectedSkill->m_range_l && x>=0 && x <MAP_COLUMN && y>=0 && y<MAP_ROW) {
                    mapAtk[i]->setVisible(true);
                }
            }

            break;
        }
        default:
            break;
    }
    
}

void HelloWorld::castSkill(sMapUnit* source, sMapUnit* target, SkillData* skilldata, int skillnum)
{
    if(!target || !source)
        return;
    HeroRef target_hero = target->hero;
    HeroRef source_hero = source->hero;

    if(!target_hero || !source_hero)
        return;
    
    Status& target_status = target_hero->m_status;
    Status& source_status = source_hero->m_status;

    // 修改 触发在释放技能前起效果的buff
    
    typedef SkillData::eSkillEffect eSkillEffect;
    
    // 无效果，普通伤害
    if (skilldata->m_effect == eSkillEffect::Effect_None) {
        //命中计算
        float hit = calculateHit(source, target);
        int hit_random = random(0, 100);
        if (hit_random > hit) {
            //修改 触发为未命中起效果的buff
            //修改 播放回避动画
        }
        else{
            reduceSkillCost(source, skilldata, skillnum);
            int damage = calculateDamage(target, skilldata);
            // 修改 触发在伤害前起效果的buff
            
            target_status.HP = target_status.HP - damage;
            
            //血量是否超过上限判断
            if (target_status.HP > target_status.MaxHP) {
                damage = damage - target_status.HP - target_status.MaxHP;
                target_status.HP = target_status.MaxHP;
            }
            
            playAtkAnimation(target->pos, damage);
            
            // 修改 触发在伤害后起效果的buff
            
            //死亡判断
            
            if (target_status.HP <= 0) {
                removeUnit(target);
            }
        }
    }
    // 百分比伤害 || 固定伤害
    else if (skilldata->m_effect == eSkillEffect::Effect_DamageP || skilldata->m_effect == eSkillEffect::Effect_DamageN) {
    }

    //buff给予
    else if (skilldata->m_effect == eSkillEffect::Effect_Buff) {
    }
    
    // 一般治疗
    else if (skilldata->m_effect == SkillData::eSkillEffect::Effect_Heal) {
    }
    
    // 百分比治疗
    // 固定数值治疗
    else if (skilldata->m_effect == eSkillEffect::Effect_HealP || skilldata->m_effect == eSkillEffect::Effect_HealN) {
    }
    
    // 百分比恢复精神
    // 固定数值恢复精神
    else if (skilldata->m_effect == eSkillEffect::Effect_SpriteP || skilldata->m_effect == eSkillEffect::Effect_SpriteN) {
    }
    
    // 百分比恢复MP
    // 固定数值恢复MP
    else if (skilldata->m_effect == eSkillEffect::Effect_MPP || skilldata->m_effect == eSkillEffect::Effect_MPN) {
    }
    
    //胜利条件判断
    
    //修改 触发战斗结束后的buff
}

void HelloWorld::reduceSkillCost(sMapUnit* unit, SkillData* data, int skillnum)
{
    Status hero_status = unit->hero->m_status;
    
    if (data->m_cost.HP > 0)
        hero_status.HP = hero_status.HP - data->m_cost.HP;
    
    if (data->m_cost.MP > 0)
        hero_status.MP = hero_status.MP - data->m_cost.MP;
    
    if (data->m_cost.time && unit->attackSkill_times[skillnum] > 0)
        unit->attackSkill_times[skillnum]--;
}

bool HelloWorld::canAttackTargetBySkill(sMapUnit* source, sMapUnit* target, SkillData* data)
{
    if (data->m_target == SkillData::eTarget::Target_Self) {
        if (source != target)
            return false;
        
        return true;
    }
    else if(data->m_target == SkillData::eTarget::Target_Friend) {
        if (source->m_force != target->m_force)
            return false;
        
        return true;
    }
    
    else if(data->m_target == SkillData::eTarget::Target_Enermy) {
        if (source->m_force == target->m_force)
            return false;
        
        return true;
    }
    
    else if(data->m_target == SkillData::eTarget::Target_Enermy_Friend) {
        return true;
    }
    
    return false;
}


bool HelloWorld::canCastSkill(SkillData* data,U32 skillPos)
{
    Status hero_status = selectedUnit->hero->m_status;
    
    if (data->m_cost.HP > hero_status.HP)
        return false;
    
    if (data->m_cost.MP > hero_status.MP)
        return false;
    
    if (data->m_cost.time && selectedUnit->attackSkill_times[skillPos] < 1)
        return false;
    
    return true;
}

float HelloWorld::calculateHit(sMapUnit* source, sMapUnit* target)
{
    //修改 加入装备后要计算装备的属性
    U32 hit   = source->hero->m_status.Hit;
    U32 dodge = target->hero->m_status.Dodge;
    
    //修改 计算下buff加成
    //
    
    int hit_gpc = 100 - MAX(dodge-hit,0);
    
    return hit_gpc;
}


int HelloWorld::calculateDamage(sMapUnit* unit, SkillData* data)
{
    Status source_status = selectedUnit->hero->m_status;
    Status target_status = unit->hero->m_status;
    

    //修改 触发在伤害计算时的buff
    
    //修改 计算buff加成
    
    //伤害公式说明
    //伤害值 ＝ 技能基础伤害＊（1＋（格斗值＊0.25*（5-距离）＋射击值＊0.25*距离－防御值）100）
    int range = abs(selectedUnit->pos.x-unit->pos.x)+abs(selectedUnit->pos.y-unit->pos.y);
    
    int avg_atk = source_status.ATKS*0.25*MAX(0,(5-range)) + source_status.ATKL*0.25*MIN(4, (range-1));
    int damage = data->m_damage*(100 + MAX(-100,avg_atk - target_status.DEF))/100;
    //暴击计算
    int critical_random = random(0, 1000);
    if (critical_random < source_status.Critical_Rating*1000) {
        damage = damage*1.2;
    }
    //最终计算
    damage = MAX(0,damage *(1+source_status.Damage_gPc)*(1-target_status.DamageImmunit_gPc));
    
    return damage;
}

void HelloWorld::selectedUnitActivityEnd()
{
    selectedUnit->icon_gray->setVisible(true);
    selectedUnit->activity = true;
    selectedUnit = nullptr;
    selectedSkillPos = -1;
    touchState = eTouchState::SelectUnit;
    
    panel_battle->setVisible(false);
    panel_enermyUnit->setVisible(false);
    
    targetUnit = nullptr;
    selectedSkill = nullptr;
    mapHelper->hideAtk();
    
}

void HelloWorld::turnForAIAction(int i)
{

    
    if (i>=6) {
        selectedUnit = nullptr;
        myTurn = true;
        
        updateUnitPos();
        
        for (int j=0; j<6; j++) {
            if (myUnit[j]) {
                myUnit[j]->activity = false;
                myUnit[j]->icon_gray->setVisible(false);
            }
        }
        return;
    }
    

    
    do
    {
        if(!enermyUnit[i])
            break;
        
        HeroRef hero    = enermyUnit[i]->hero;
        if(!hero)
            break;
        
        HeroData* data  = HeroManager::getInstance()->getHeroData(hero->id);
        if(!data)
            break;
        
        selectedUnit = enermyUnit[i];
        std::vector<Vector<FiniteTimeAction*>> actVec;  // 移动动画数组
        std::vector<int> skillNumVec;   // 技能数组
        std::vector<int> skillWeightVec;    // 权值数组
        std::vector<std::vector<sMapUnit*>> targetVec;  // 打击目标数组的数组
        std::vector<Point> posVec;  // 打击地点的数组
        
        Point temppos = enermyUnit[i]->pos;
        
        for (int j=0; j<MAP_NUM; j++) {
            Point source_pos = Point(j%MAP_COLUMN,j/MAP_COLUMN);
            
            mapHelper->showMove(enermyUnit[i]->move, enermyUnit[i]->pos);
            Vector<FiniteTimeAction*> act = mapHelper->moveUnitToPos(enermyUnit[i], source_pos);
            mapHelper->hideMove();
            if(!act.size())
                continue;
            
            enermyUnit[i]->pos = source_pos;    //先将行动单位的位置移到可以此地点，因为技能要以自己移动后的位置来算
            
            for (int m=0; m<3; m++) {
                SkillData* _data = SkillManager::getInstance()->getSkillData(data->m_attackSkill[m]);
                if(!_data)
                    continue;
                if(!canCastSkill(_data, m))
                    continue;
                
                if (_data->m_skillCastType == SkillData::eSkillCastType::Cast_All) {
                    std::vector<sMapUnit*> target;
                    for (int n=0; n<6; n++) {
                        if (enermyUnit[n] && canAttackTargetBySkill(enermyUnit[i], enermyUnit[n], _data))
                            target.push_back(enermyUnit[n]);
                        if (myUnit[n] && canAttackTargetBySkill(enermyUnit[i], myUnit[n], _data))
                            target.push_back(myUnit[n]);
                    }
                    
                    if (target.size()) {
                        actVec.push_back(act);
                        skillNumVec.push_back(m);
                        skillWeightVec.push_back(20);
                        targetVec.push_back(target);
                    }
                }
                        
                else if (_data->m_skillCastType == SkillData::eSkillCastType::Cast_None) {
                }
                
                // 范围攻击
                else if (_data->m_skillCastType == SkillData::eSkillCastType::Cast_Region) {
                    for (int n=0; n<MAP_NUM; n++) {
                        int x = n%MAP_COLUMN;
                        int y = n/MAP_COLUMN;
                        
                        // 判断目标地点是否在射程范围内
                        if (abs(x-source_pos.x)+abs(y-source_pos.y) >= _data->m_range_s &&
                            abs(x-source_pos.x)+abs(y-source_pos.y) <= _data->m_range_l &&
                            x>=0 && x<MAP_COLUMN && y>=0 && y<MAP_ROW &&
                            Point(x,y) != source_pos) {
                            
                            mapHelper->hideAtk();
                            mapHelper->showAtkRegion(_data, source_pos, Point(x,y));  // 显示范围攻击的范围
                            
                            std::vector<sMapUnit*> target;
                            
                            // 将范围攻击涉及到的可攻击目标加入目标数组
                            for (int l=0; l<MAP_NUM; l++) {
                                if(!mapAtk[l]->isVisible())
                                    continue;
                                
                                sMapUnit* unit = getUnitByPos(Point(l%MAP_COLUMN, l/MAP_COLUMN));
                                if(!unit || !canAttackTargetBySkill(enermyUnit[i], unit, _data))
                                    continue;
                                
                                target.push_back(unit);
                            }
                            
                            if (!target.size())
                                continue;
                            
                            actVec.push_back(act);
                            skillNumVec.push_back(m);
                            skillWeightVec.push_back(200*target.size());
                            targetVec.push_back(target);
                            posVec.push_back(Point(x,y));
                        }
                    }
                }
                else if (_data->m_skillCastType == SkillData::eSkillCastType::Cast_Point) {
                    for (int n=0; n<MAP_NUM; n++) {
                        int x = n%MAP_COLUMN;
                        int y = n/MAP_COLUMN;
                        
                        sMapUnit* unit = getUnitByPos(Point(x,y));
                        
                        if (unit &&
                            abs(x-source_pos.x)+abs(y-source_pos.y) >= _data->m_range_s &&
                            abs(x-source_pos.x)+abs(y-source_pos.y) <= _data->m_range_l &&
                            x>=0 && x<MAP_COLUMN &&
                            y>=0 && y<MAP_ROW &&
                            canAttackTargetBySkill(enermyUnit[i], unit, _data)) {
                            
                            actVec.push_back(act);
                            skillNumVec.push_back(m);
                            skillWeightVec.push_back(200);
                            
                            std::vector<sMapUnit*> target;
                            target.push_back(unit);
                            targetVec.push_back(target);
                            
                            posVec.push_back(unit->pos);
                        }
                    }
                }
            }
            enermyUnit[i]->pos = temppos;    //先将行动单位的位置移到可以此地点，因为技能要以自己移动后的位置来算
        }
        
        // 如果攻击不到目标就向最近的敌方目标移动
        if (!actVec.size()) {
            Vector<FiniteTimeAction*> act;
            
            for (int j=0; j<6; j++) {
                if(!myUnit[j])
                    continue;
                if(!act.size()){
                    act = mapHelper->moveUnitToUnit(enermyUnit[i], myUnit[j]);
                }
                else{
                    Vector<FiniteTimeAction*> _act = mapHelper->moveUnitToUnit(enermyUnit[i], myUnit[j]);
                    
                    if (_act.size() && _act.size() < act.size())
                        act = _act;
                }
            }
            
            auto callback = CallFunc::create([i,this](){
                turnForAIAction(i+1);
            });
            
            act.pushBack(callback);
            auto sequence = Sequence::create(act);
            
            enermyUnit[i]->icon->runAction(sequence);
            return;
        }
        
        for (std::vector<int>::iterator it = skillWeightVec.begin(); it != skillWeightVec.end(); it++) {
            if (it == skillWeightVec.begin())
                continue;
            // 权值为前面所有值之和
            *it = *it + *(it-1);
        }
        
        int weight = random(0, *skillWeightVec.cbegin());
        int num = -1;
        
        std::vector<int>::iterator it = skillWeightVec.begin();
        for (int p=0; p<skillWeightVec.size(); p++,it++) {
            if (*it >= weight) {
                num = p;
                break;
            }
        }
        
        Vector<FiniteTimeAction*> act = actVec.at(num);
        SkillData* _data =  SkillManager::getInstance()->getSkillData(data->m_attackSkill[skillNumVec.at(num)]);
        Point pos = posVec.at(num);
        std::vector<sMapUnit*> target = targetVec.at(num);
        int skillnum = skillNumVec.at(num);
        
        if (_data->m_skillCastType == SkillData::eSkillCastType::Cast_All) {
            auto callback = CallFunc::create([i,this](){
                enermyUnit[i]->pos = touchToPos(enermyUnit[i]->icon->getPosition());
                mapHelper->showAtkAll();
            });
            
            act.pushBack(callback);
        }
        else if (_data->m_skillCastType == SkillData::eSkillCastType::Cast_Point) {
            auto callback = CallFunc::create([i,this,pos](){
                enermyUnit[i]->pos = touchToPos(enermyUnit[i]->icon->getPosition());
                mapHelper->showAtkBlock(pos.x, pos.y);
            });
            
            act.pushBack(callback);
        }
        else if (_data->m_skillCastType == SkillData::eSkillCastType::Cast_Region) {
            auto callback = CallFunc::create([i,this,_data,pos](){
                enermyUnit[i]->pos = touchToPos(enermyUnit[i]->icon->getPosition());
                mapHelper->showAtkRegion(_data, enermyUnit[i]->pos, pos);
            });
            
            act.pushBack(callback);
        }
        
        act.pushBack(DelayTime::create(0.5));
        
        auto callback_1 = CallFunc::create([i,this,_data,pos,target,skillnum](){
            mapHelper->hideAtk();
            enermyUnit[i]->pos = touchToPos(enermyUnit[i]->icon->getPosition());
            for (std::vector<sMapUnit*>::const_iterator it = target.begin(); it != target.end(); it++) {
                castSkill(enermyUnit[i], *it, _data, skillnum);
            }
        });
        
        act.pushBack(callback_1);
        act.pushBack(DelayTime::create(1));
        
        auto callback_2 = CallFunc::create([i,this]() mutable{
            i++;
            turnForAIAction(i);
        });
        
        act.pushBack(callback_2);
        
        auto sequence = Sequence::create(act);
        enermyUnit[i]->icon->runAction(sequence);
        return;
        
    }while (0);
    

    

    
    i++;
    turnForAIAction(i);
}

void HelloWorld::checkVictoryConditions()
{
    for (int i=0; i<6; i++) {
        if(enermyUnit[i])
            return;
    }
    
    
}

void HelloWorld::playAtkAnimation(Point pos, int damage)
{
    //攻击动画
    ActionTimeline *action = CSLoader::createTimeline("attack_effect_1.csb");
    attack_effect_1->setVisible(true);
    attack_effect_1->setPosition(posToTouch(pos));
    std::stringstream str;
    str<<damage;
    dynamic_cast<Text*>(attack_effect_1->getChildByName("num"))->setString(str.str());
    attack_effect_1->runAction(action);
    action->gotoFrameAndPlay(0,60,false);
}

void HelloWorld::onActionMenuTouchEvent(Ref *ref, Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    int tag = dynamic_cast<Button*>(ref)->getTag();
    
    switch(tag)
    {
        case 1:
        {
            CCLOG("单位移动");
            
            actionMenu->setVisible(false);
            mapHelper->showMove(selectedUnit->move, selectedUnit->pos);
            touchState = eTouchState::SelectPosMove;
            break;
        }
        case 2:
        {
            CCLOG("单位攻击");
            
            lastTouchState = touchState;
            touchState = eTouchState::SelectSkill;
            actionMenu->setVisible(false);
            
            showPanelSkill();
            break;
        }
        case 3:
        {
            CCLOG("单位精神");
            
            lastTouchState = touchState;
            touchState = eTouchState::SelectSprite;
            actionMenu->setVisible(false);
            
            showPanelSprite();
        }
        case 4:
        {
            CCLOG("单位待命");
            selectedUnitActivityEnd();
            actionMenu->setVisible(false);
            break;
        }
        default:
            break;
    }
}

void HelloWorld::onTurnMenuTouchEvent(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type)
{
    {
        if(type != Widget::TouchEventType::ENDED)
            return;
        
        int tag = dynamic_cast<Button*>(ref)->getTag();
        
        switch(tag)
        {
            case 1:
            {
                CCLOG("回合结束");
                myTurn = false;
                turnMenu->setVisible(false);
                turnForAIAction(0);
                
                break;
            }
            case 2:
            {
                CCLOG("胜利条件");
                
                break;
            }
            case 3:
            {
                CCLOG("取消选择");
                turnMenu->setVisible(false);
                break;
            }
            case 4:
            {
                break;
            }
            default:
                break;
        }
    }
}

void HelloWorld::onSkillTouchEvent(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    if(!selectedUnit || !selectedUnit->hero)
        return;
    
    HeroData* data =  HeroManager::getInstance()->getHeroData(selectedUnit->hero->id);
    
    if(!data)
        return;
    
    int tag = dynamic_cast<Button*>(ref)->getTag();
    
    if(data->m_attackSkill[tag-1] <= 0)
        return;
    
    SkillData* _data = SkillManager::getInstance()->getSkillData(data->m_attackSkill[tag-1]);
    
    if(!_data)
        return;
    
    
    if (!canCastSkill(_data, tag-1)) {
        CCLOG("%d号技能释放不能",tag);
        return;
    }
    
    //修改 触发选择技能时的buff
    //
    
    panel_skill->setVisible(false);
    selectedSkill = _data;
    selectedSkillPos = tag-1;
    CCLOG("%d号技能",tag);
    
    selectTargetForSkill();
}

void HelloWorld::onBattleTouchEvent(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED)
        return;
    
    CCLOG("开始战斗！！！！");

    castSkill(selectedUnit, targetUnit, selectedSkill, selectedSkillPos);
    selectedUnitActivityEnd();
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void HelloWorld::onTouchEnded(Touch* touch, cocos2d::Event *event)
{
    if(!myTurn)
        return;
    
    Point point = touch->getLocation();
    if(mapLayer->getBoundingBox().containsPoint(point)){
        point.x = point.x - mapLayer->getPosition().x-4;
        point.y = point.y - mapLayer->getPosition().y+4;
        
        turnMenu->setVisible(false);
        
        switch (touchState) {
            case eTouchState::SelectUnit:
            {
                CCLOG("选择了单位/打开了菜单");
                if (!selectedUnit) {
                    selectUnitForAct(point);
                }
            }
                break;
            case eTouchState::SelectMenu:
            {
                CCLOG("关闭了菜单");
                actionMenu->setVisible(false);
                touchState = eTouchState::SelectUnit;
                selectedUnit = NULL;
            }
                break;
            case eTouchState::SelectPosMove:
            {
                CCLOG("选择了移动地点");
                if(!moveUnitToPos(selectedUnit, touchToPos(point))){
                    
                }
            }
                break;
            case eTouchState::Moving:
            {
                CCLOG("移动中>>>>>>>>>>>>>!");
                touchState = eTouchState::SelectUnit;
            }
                break;
            case eTouchState::SelectMenuAfterMove:
            {
                CCLOG("取消本次移动");
                touchState = eTouchState::SelectMenu;
                setUnitPosition(selectedUnit, selectedUnit->l_pos);
                showActionMenu(selectedUnit->pos);
                selectImage->setPosition(posToTouch(selectedUnit->pos));
            }
                break;
            case eTouchState::SelectSkill:
            {
                CCLOG("取消技能选择");
                backupLastState();
            }
                break;
            case eTouchState::SelectSprite:
            {
                CCLOG("取消精神选择");
                backupLastState();
            }
                break;
            case eTouchState::SelectUnitForSkill:
            {
                CCLOG("选择了攻击对象");
                Point pos = touchToPos(point);
                
                if (!mapAtk[(int)pos.x+MAP_COLUMN*(int)pos.y]->isVisible()) {
                    mapHelper->hideAtk();
                    panel_skill->setVisible(true);
                    selectedSkill = nullptr;
                    targetUnit = nullptr;
                    panel_battle->setVisible(false);
                    touchState = eTouchState::SelectSkill;
                    break;
                }
                
                switch (selectedSkill->m_skillCastType) {
                    case SkillData::eSkillCastType::Cast_Point:
                    {
                        targetUnit = getUnitByPos(pos);
                        if(!targetUnit)
                            break;
                        showPanelEnermyUnit(targetUnit);
                        //如果是攻击就显示命中率等等战斗面板，否则直接作用于目标
                        if (selectedSkill->m_skillType == SkillData::eSkillType::SkillType_Normal) {
                            showPanelBattle();
                        }
                        else {
                            castSkill(selectedUnit, targetUnit, selectedSkill, selectedSkillPos);
                            selectedUnitActivityEnd();
                        }

                    }
                        break;
                    case SkillData::eSkillCastType::Cast_Region:
                    {
                        mapHelper->hideAtk();
                        mapHelper->showAtkRegion(selectedSkill, selectedUnit->pos, touchToPos(point));
                    }
                        break;
                    case SkillData::eSkillCastType::Cast_All:
                    {
                        for (int i=0; i<6; i++) {
                            castSkill(selectedUnit, myUnit[i], selectedSkill, selectedSkillPos);
                            castSkill(selectedUnit, enermyUnit[i], selectedSkill, selectedSkillPos);
                        }
                        
                        selectedUnitActivityEnd();
                    }
                        break;
                        
                    default:
                        break;
                }
            }
                break;
            case eTouchState::SelectUnitForSprite:
            {
                CCLOG("选择了精神效果对象");
                touchState = eTouchState::SelectUnit;
                
                Point pos = touchToPos(point);
                
                sMapUnit* unit = getUnitByPos(pos);
                if (!unit) {
                    break;
                }
                
                castSkill(selectedUnit, unit, selectedSkill, selectedSkillPos);
                
                backupLastState();
                
                targetUnit = nullptr;
                selectedSkill = nullptr;
            }
                break;
            case eTouchState::SelectUnitForSkillByRegion:
            {
                CCLOG("选择了攻击技能区域");
                Point pos = touchToPos(point);
                if (!mapAtk[(int)pos.x+MAP_COLUMN*(int)pos.y]->isVisible()) {
                    mapHelper->hideAtk();
                    selectTargetForSkill();
                    break;
                }
                
                std::vector<sMapUnit*> vec = mapHelper->checkAtkUnit();
                
                if(!vec.size())
                    break;
                std::vector<sMapUnit*>::iterator it = vec.begin();
                
                for (; it != vec.end(); it++) {
                    castSkill(selectedUnit, *it, selectedSkill, selectedSkillPos);
                }
                
                selectedUnitActivityEnd();
                touchState = eTouchState::SelectUnit;
            }
                break;
            default:
                
                break;
        }
    }
}
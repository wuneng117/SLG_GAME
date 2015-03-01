#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "CocosGUI.h"

#include "MapHelper.h"
#include "HeroManager.h"

class SkillData;

struct sMapPoint
{
    sMapPoint():f(0),g(0),h(0),bClose(false),bOpen(false),bStart(false),bEnd(false),sParnet(NULL)
    {
    }
    ~sMapPoint()
    {
        sParnet = nullptr;
    };
    
    cocos2d::Point pos;
    int f;
    int g;
    int h;
    bool bClose;
    bool bOpen;
    bool bStart;
    bool bEnd;
    sMapPoint* sParnet;
};

struct sMapUnit
{
    sMapUnit():m_force(0),icon(nullptr),icon_gray(nullptr),move(0),activity(0),hero(NULL)
    {}
    ~sMapUnit()
    {
        icon->removeFromParent();
    };
    
    int m_force;    //势力
    cocos2d::Sprite* icon;
    cocos2d::Sprite* icon_gray;
    cocos2d::Point pos;
    cocos2d::Point l_pos;
    int move;
    int activity;
    HeroRef hero;
    U32 attackSkill_times[3];
};

class HelloWorld : public cocos2d::Layer
{
    
    friend class MapHelper;
public:
    
    //触摸时的状态
    enum eTouchState
    {
        NullState,      //无效状态
        SelectUnit,     //等待选择单位进行行动
        SelectMenu,     //打开单位菜单选择行动
        SelectPosMove,   //等待选择移动的地点
        Moving,             //移动中
        SelectMenuAfterMove, //打开单位菜单选择行动（移动后）
        SelectSkill,        //选择技能
        SelectSprite,        //选择精神
        SelectUnitForSkill,   //选择技能目标/地点(攻击||精神)
        SelectUnitForSkillByRegion,   //选择技能地点目标的确认(攻击||精神)
        SelectUnitForSprite,    //选择精神效果对象
    };
    
    static cocos2d::Scene* createScene(std::vector<HeroRef> myHero, std::vector<HeroRef> enermyHero);
    
    HelloWorld();
    ~HelloWorld();
    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(HelloWorld);
    
    void _init(std::vector<HeroRef> myHero, std::vector<HeroRef> enermyHero);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    //按钮回调
    void onActionMenuTouchEvent(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);   //角色菜单
    void onTurnMenuTouchEvent(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);  //回合菜单
    void onSkillTouchEvent(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);    //技能菜单
    void onBattleTouchEvent(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);    //攻击开始按钮
    
    //坐标转换
    cocos2d::Point touchToPos(cocos2d::Point touch){ return mapHelper->touchToPos(touch);}
    cocos2d::Point posToTouch(cocos2d::Point pos){ return mapHelper->posToTouch(pos);}

    bool setUnitPosition(sMapUnit* unit, cocos2d::Point point);     //摆放单位
    bool moveUnitToPos(sMapUnit* unit, cocos2d::Point point);       //移动单位
    void removeUnit(sMapUnit* unit);    // 删除单位
    sMapUnit* getUnitByPos(cocos2d::Point pos){ return mapHelper->getUnitByPos(pos);} //获取点击位置上的单位
    void selectUnitForAct(cocos2d::Point point);  //无单位情况下点击地图
    void updateUnitPos();       // 将角色的pos和它的icon同步
    
    void showPanelMyUnit(sMapUnit* unit);   //显示我的角色属性
    void showPanelEnermyUnit(sMapUnit* unit);   //显示目标的角色属性
    void showSkill(Node* node ,U32 skillId);    //显示技能
    void showActionMenu(cocos2d::Point pos, bool bMoved = false);    //显示行动菜单
    void showPanelSkill();    //显示技能菜单
    void showPanelSprite();   //显示技能菜单
    void showPanelBattle();   //显示战斗信息
    bool backupLastState();   //恢复上一次的状态
    
    void selectTargetForSkill();   //选择技能目标
    void castSkill(sMapUnit* source, sMapUnit* target, SkillData* skilldata, int skillnum); //目标对目标释放制定技能
    void reduceSkillCost(sMapUnit* unit, SkillData* data, int skillnum); //技能消耗
    bool canAttackTargetBySkill(sMapUnit* source, sMapUnit* target, SkillData* data);   // 判断目标是否可以用技能攻击
    bool canCastSkill(SkillData* data,U32 skillPos);    //释放技能消耗够不够
    float calculateHit(sMapUnit* source, sMapUnit* target);  //计算命中率
    int calculateDamage(sMapUnit* unit, SkillData* data); //计算伤害
    
    void selectedUnitActivityEnd();     //选择单位行动结束
    void turnForAIAction(int i);    //电脑行动
    void checkVictoryConditions();  //判断胜利条件
    
    void playAtkAnimation(cocos2d::Point pos, int damage);    // 播放攻击动画
private:
    cocos2d::ui::Layout* mapLayer;          //地图层底
    cocos2d::ui::Layout* panel_myUnit;      //我方单位数据框
    cocos2d::ui::Layout* panel_enermyUnit;      //敌方单位数据框
    Node* actionMenu;        //行动菜单
    Node* turnMenu;     //回合菜单
    cocos2d::ui::ImageView* selectImage;    //选择框
    cocos2d::ui::Layout* panel_skill;   //技能层
    cocos2d::ui::Layout* panel_battle;  //战斗信息层
    Node* attack_effect_1;  // 攻击特效
    
    
    
    cocos2d::Sprite* mapMove[MAP_NUM];      //移动高亮
    cocos2d::Sprite* mapAtk[MAP_NUM];       //攻击高亮
    sMapPoint* map[MAP_NUM];
    
    sMapUnit* myUnit[6];    // 自己的角色
    sMapUnit* enermyUnit[6];    // 对方角色
    sMapUnit* selectedUnit; // 选中的角色
    sMapUnit* targetUnit;   // 选中角色的目标角色
    SkillData* selectedSkill;   // 选中角色释放的技能
    U32 selectedSkillPos; //释放的技能在数组的位置
    
    bool myTurn;            //我的回合
    MapHelper* mapHelper;   //地图管理
    eTouchState touchState; //当前触摸
    eTouchState lastTouchState; //上次触摸
    
    //常量
    const int ATTACKEFFECT_TAG = 1000;
    const int MAPATTACK_TAG    = 100;
    const int SELECTIMAGE_TAG  = 900;
};

#endif // __HELLOWORLD_SCENE_H__

//
//  HeroManager.h
//  lantisi
//
//  Created by ZHOUTAO on 14-5-24.
//
//

#ifndef __lantisi__HeroManager__
#define __lantisi__HeroManager__

#include "types.h"
#include "Status.h"
#include <memory>

struct Hero;
typedef std::shared_ptr<Hero> HeroRef;

const U32 MAX_NAME_LENGTH			= 32;
const U32 MAX_HERO_PASSIVESKILL     = 3;					// 卡牌被动技能个数
const U32 MAX_HERO_ATTACKSKILL		= 2;					// 卡牌攻击技能个数
const U32 PLAYER_MAX_LEVEL			= 200;					// 玩家最高等级
static U32 MAX_HERODATA_NUM = 10;

//职业
enum eClass
{
    ClassNone = 0,
    RenWai  = 1,     //人外
    OuXiang = 2,    //偶像
    ChiHuo  = 3,     //吃货
    TianCai = 4,    //天才
    XueShengHuiZhang = 5,   //学生会长
    LoLi    = 6,   //萝莉
    GuiZu   = 7,  //贵族
    ZhongEr = 8,   //中二
    GaoZhongSheng = 9,  //高中生
    AoJiao  = 10, //傲娇
    YanJing = 11,    //眼镜
    HuangDi = 12,    //皇帝
    ZhanDouKuang = 13,   //战斗狂
    HenTai  = 14,     //变态
    YiNv    = 15,   //乙女
    GongZhu = 16,    //公主
    DaXiaoJie = 17,  //大小姐
    NvPu    = 18,   //女仆
    NvWang  = 19, //女王
    YuJie   = 20,  //御姐
    MoNv    = 21,   //魔女
    MeiMei  = 22, //妹妹
    WeiNiang = 23,   //伪娘
    XiongGui = 24,   //兄贵
    DaShu   = 25,  //大叔
    ZhengTai = 26,   //正太
    ReXueShaoNian = 27,  //热血少年
    ZhengYiDeQingNian = 28,  //正义的青年
    TianRanDai = 29,    //天然呆
};

struct HeroData
{
    
    HeroData():id(0),m_name(""),m_icon(""),m_fg(""),m_class1(0),m_class2(0),m_des(""),m_album(0)
    {
        memset(m_attackSkill, 0, sizeof(m_attackSkill));
        memset(m_passiveSkill, 0, sizeof(m_passiveSkill));
        memset(m_baseStatus, 0, sizeof(m_baseStatus));
        memset(m_addStatus, 0, sizeof(m_addStatus));
    }

    ~HeroData() {};

    U32 id; //npcid
    U32 m_class1; //职业1
    U32 m_class2; //职业2
    U32 m_baseStatus[9]; //初始属性
    U32 m_attackSkill[3]; //攻击技能,3为职业技能
    U32 m_passiveSkill[3]; //被动技能
    U32 m_addStatus[9]; //成长属性
    U32 m_album; //出处作品
    
    std::string m_name; //名字
    std::string m_icon; //头像
    std::string m_fg; //立绘
    std::string m_des;  //描述
    
    //商业技能
    
    U32 m_mining;   //挖矿
    U32 m_farm;     //农牧
    U32 m_tree;     //土木
    U32 m_alchemy;  //炼金
    U32 m_item;     //合成（道具）
    U32 m_weapon;   //锻造（武器）
    U32 m_armor;    //铸甲（防具）
    U32 m_charm;    //魅力（珠宝）
    U32 m_manage;   //管理
    U32 m_communication;  //交涉
    U32 m_home;     //家政
    U32 m_search;   //探索


};

struct Hero
{
    U32 id;                        // 英雄模板ID
    U32 m_level;                   // 等级
    U32 m_exp;                     // 经验
    Status m_status;               // 属性
    cocos2d::Point pos;            // 战斗起始位置
    Hero(){ memset(&id, 0, sizeof(Hero));}
    

    
    void WriteData();
    void ReadData();
};

class HeroManager
{
public:
    HeroManager();
    ~HeroManager();
    
    typedef std::map<int,HeroData> HERODATA_MAP;
    typedef HERODATA_MAP::iterator HERODATA_MAPITR;
    
    static HeroManager* getInstance();
    bool read();
    
    HeroRef createHero(S32 id);  //生成新英雄
    HeroRef createHeroRandom();    //随机生成新英雄
    
    HeroData* getHeroData(S32 heroId);  //返回英雄data
    
private:
    HERODATA_MAP m_heroDataMap; //保存herodata
    
};


#endif /* defined(__lantisi__HeroManager__) */

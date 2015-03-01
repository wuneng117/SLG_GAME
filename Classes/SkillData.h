//
//  SkillData.h
//  lantisi
//
//  Created by ZHOUTAO on 14-6-11.
//
//

#ifndef __lantisi__SkillData__
#define __lantisi__SkillData__

#include <iostream>
#include "types.h"

class SkillData
{
public:
    //技能类型
    enum eSkillType
    {
        SkillType_Normal            = 1,    // 普通技能
        SkillType_Passive			= 2,	// 被动技能
        SkillType_Spirit            = 3,    // 精神
        SkillType_Special           = 4,    // 特殊技能
    };
    
    //技能释放类别
    enum eSkillCastType
    {
        Cast_None   = 0,    //无
        Cast_Region = 1,    //区域型
        Cast_Point  = 2,    //指向型
        Cast_All    = 4,    //全体
        Cast_Region_Point = Cast_Region | Cast_Point,   //3
    };
    
    // 技能作用对象
    enum eTarget
    {
        Target_None           = 0,    //无
        Target_Enermy         = 1,    //敌方
        Target_Friend         = 2,    //友方（包括自身）
        Target_Self           = 4,    //自己
        Target_Enermy_Friend  = Target_Enermy | Target_Friend,    //双方（包括自己）3

    };
    
    // 技能区域
    enum eRegion
    {
        Region_None       = 0,
        Region_Cross      = 1,    // 十字
        Region_Cross_2    = 2,    // 叉叉
        Region_Line       = 3,    // 横
        Region_Line_3     = 4,    // 3竖
        Region_Squre      = 5,    // 方块
        Region_Triangle   = 6,    // 三角形
    };
    
    //技能效果
    enum eSkillEffect
    {
        Effect_None     = 0,    //无效果(普通伤害)~
        Effect_Buff     = 1,    //buff付与
        Effect_Heal     = 2,    //治疗量治疗
        Effect_HealP    = 3,    //百分比治疗
        Effect_HealN    = 4,    //固定数值治疗
        Effect_DamageP  = 5,    //百分比伤害
        Effect_DamageN  = 6,    //固定数值伤害
        Effect_SpriteP  = 7,    //百分比恢复精神
        Effect_SpriteN  = 8,    //固定数值恢复精神
        Effect_MPP      = 9,    //百分比恢复MP
        Effect_MPN      = 10,   //固定恢复MP
    };
    
    struct Cost
    {
        int HP;			// 消耗生命值
        int MP;			// 消耗魔法值
        int time;       // 消耗次数
        int sprite;     // 消耗精神
    };
    
    struct Cast
    {
        std::string castAnimation;      // 技能动画
        std::string castSound;          // 技能施放音效
    };
    
    //被动技能发动时机
    enum ePassiveActiveTime
    {
        
    };
    
public:
    SkillData():id(0),m_skillType(0),m_skillCastType(0),m_target(0),m_region(0),m_effect(0),m_range_s(0),m_range_l(0),m_selfBuff(0),m_targetBuff(0),m_selfStatus(0),
    m_targetStatus(0),m_name(""),m_des("")
    {
    }
    
    ~SkillData()
    {
    }
    
    U32     id;                     // 技能ID
    U32     m_damage;               // 基础伤害
    U32     m_skillType;            // 技能类别
    U32     m_skillCastType;        // 技能释放类别
    U32     m_target;               // 技能对象
    U32     m_region;               // 技能作用范围
    U32     m_effect;               // 技能效果
    
    U32     m_range_s;              // 技能最短射程
    U32     m_range_l;              // 技能最长射程
    U32     m_selfBuff;             //给自己添加的buff(如果目标可以为自己的话这个应该不触发)
    U32     m_targetBuff;           //给对方添加的buff
    
    U32     m_selfStatus;           // 自身状态
    U32     m_targetStatus;         // 目标状态
    
    std::string     m_name;         // 技能名称
    std::string     m_des;          // 技能描述
    
    Cost			m_cost;         // 技能施放消耗
    Cast			m_cast;			// 施放
};

class SkillManager
{
public:
    
    typedef std::map<unsigned int, SkillData> SKILLDATA_MAP;
    typedef SKILLDATA_MAP::iterator SKILLDATA_MAPITR;
    
    SkillManager();
    ~SkillManager();
    static SkillManager* getInstance();
    
    bool read();
    SkillData* getSkillData(unsigned int skillId);
    
private:
    SKILLDATA_MAP m_skillDataMap;
};

#endif /* defined(__lantisi__SkillData__) */

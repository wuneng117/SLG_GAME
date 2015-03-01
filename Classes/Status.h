//
//  Status.h
//  testproject
//
//  Created by ZHOUTAO on 15/1/22.
//
//

#ifndef __testproject__Status__
#define __testproject__Status__

#include <stdio.h>
#include "types.h"


struct Status
{
    U32         id;
    
    S32         MOV;                //移动力
    
    S32			HP;					// 生命
    float		HP_gPc;				// 生命百分比
    
    S32			MaxHP;				// 最大生命
    float		MaxHP_gPc;			// 最大生命百分比
    
    S32         MP;                 // 魔法
    float       MP_gPc;             // 魔法百分比
    
    S32         MaxMP;              // 最大魔法
    float       MaxMP_gPc;          // 最大魔法百分比
    
    S32			ATKS;               // 格斗
    float       ATKS_gPc;           // 格斗百分比
    
    S32			ATKL;               // 射击
    float		ATKL_gPc;           // 射击百分比
    
    S32			DEF;                // 防御
    float		DEF_gPc;            // 防御百分比
    
    S32         Dodge;              // 回避
    float       Dodge_gPc;          // 回避百分比
    
    S32         Hit;                // 命中
    float       Hit_gPc;            // 命中百分比
    
    S32         Hit_Rating;         //命中几率
    S32         Critical_Rating;    // 暴击几率
    S32         Dodge_Rating;       // 回避几率
    
    float       Damage_gPc;         // 伤害调整百分比
    float       Recover_gPc;        // 治疗调整百分比
    
    float       DamageImmunit_gPc;  // 受伤害调整百分比
    float       RecoverImmunit_gPc; // 受治疗调整百分比
    
    float       Exp_gPc;    //经验调整百分比
    
    Status();
    void Clear();
    
    Status& operator+=(const Status&);
    Status& operator-=(const Status&);
    Status& operator*=(const float);
    
    // 最终计算
    void Calc(const Status& _add);
    // 数值增强（用于状态、技能的增强）
    void Plus(const Status& _orig, const Status& _plus);
    // 属性叠加，与+=相比增加了叠加数
    Status& Add(const Status& _add, S32 _count = 1);
    
    void percent(const float val);
};

class StatusManager
{
public:
    typedef std::map<int,Status> STATUS_MAP;
    typedef STATUS_MAP::iterator STATUS_MAPITR;
    
    StatusManager();
    ~StatusManager();
    static StatusManager* getInstance();
    
    bool read();
    
    Status getStatus(U32 statusId);
    
private:
    
    STATUS_MAP m_statusMap;
};
#endif /* defined(__testproject__Status__) */

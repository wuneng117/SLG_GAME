//
//  BuffData.h
//  testproject
//
//  Created by ZHOUTAO on 15/1/23.
//
//

#ifndef __testproject__BuffData__
#define __testproject__BuffData__

#include <stdio.h>
#include <memory>
#include "types.h"
#include "Status.h"
#include "HeroManager.h"

class Buff;
typedef std::shared_ptr<Buff> BuffRef;

class BuffData
{
public:
    //buff持续类型
    enum eBuffCastType
    {
        BuffCastNone        = 0,        // 无，表示永久
        BuffCastTurn        = 1,		// 回合
        BuffCasttime        = 2,        // 战斗次数
        
    };
    
    //buff触发时机
    enum BuffTrigger
    {
        Trigger_None			= 0,			// 无触发
        Trigger_Death			= BIT(0),		// 死亡
        Trigger_Rise			= BIT(1),		// 复活
        Trigger_Attack          = BIT(2),		// 攻击
        Trigger_Move			= BIT(3),		// 移动
        Trigger_Heal			= BIT(4),		// 治疗
        Trigger_Spirit          = BIT(5),       // 使用精神
        Trigger_UnderHealAttack	= BIT(8),       // 受到治疗
        Trigger_UnderPhyAttack	= BIT(9),		// 被攻击
        Trigger_BuffTick		= BIT(12),		// 状态每一跳触发（只触发技能)
        Trigger_Remove          = BIT(13),      // 移除触发
        Trigger_DodgeRating     = BIT(14),      // 闪避
        Trigger_CriticalRating  = BIT(15),      // 暴击
        Trigger_Turn            = BIT(16),      // 每回合触发
    };
    
    // 状态效果
    enum BuffEffect
    {
        Effect_Status               = BIT(0),	// 状态提升(下降)
        Effect_DamageImmunit10		= BIT(1),	// 承受伤害为10
        Effect_ActPlus				= BIT(2),	// 增加一次行动
        Effect_Leave10				= BIT(3),	// 饶命(下一次伤害留给敌人10HP)
        Effect_Immobility			= BIT(4),	// 定身
    };
    
    //buff效果对象
    enum eTarget
    {
        TargetNone,
        TT_Source,			// 源施放者对象
        TT_Carrier,			// 本体携带者
        TT_Trigger,			// 触发者
    };

    //初始回合
    BuffData():id(0),m_name(""),m_icon(""),m_des(""),m_buffCastType(0),m_buffTrigger(0),m_buffEffect(0),m_target(0),
    m_turn(0),m_time(0)
    {
    }
    
    ~BuffData()
    {
    }
    
    U32 id;
    
    U32 m_buffCastType; // buff持续类型
    U32 m_buffTrigger;  // buff触发时机类型
    U32 m_buffEffect;   // 状态效果类型
    U32 m_target;       // buff效果对象类型
    
    U32 m_turn;         // 持续回合
    U32 m_time;         // 持续次数
    
    std::string m_name;        // 状态名称
    std::string m_icon;        // 状态图标
    std::string m_des;         // 状态描述
    
  
    Status m_status;

    inline void Clear()             { m_status.Clear();};
};

//具体buff
class Buff
{
public:
    
    Buff():id(0),m_source(nullptr),m_carrier(nullptr),m_turn(0),m_time(0)
    {
        
    }
    ~Buff()
    {
        m_source = nullptr;
    }
    
    U32 id;     // 对应编号
    
    HeroRef m_source; // buff给予对象
    HeroRef m_carrier; // buff携带者
    
    U32 m_turn; // buff剩余持续回合
    U32 m_time; // buff剩余持续次数
};

class BuffDataManager {
    
public:
    
    typedef std::map<S32,BuffData> BUFFDATA_MAP;
    typedef BUFFDATA_MAP::iterator BUFFDATA_MAPITR;
    
    BuffDataManager();
    ~BuffDataManager();
    static BuffDataManager* getInstance();
    
    bool read();
    
    BuffData* GetBuff(S32 buffId);
    BuffRef createBuff(S32 buffId, HeroRef source, HeroRef target);
    
private:
    BUFFDATA_MAP m_buffDataMap;
};


#endif /* defined(__testproject__BuffData__) */

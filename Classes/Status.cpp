//
//  Status.cpp
//  testproject
//
//  Created by ZHOUTAO on 15/1/22.
//
//

#include "Status.h"
#include "ctest.h"

static StatusManager* s_StatusManager = nullptr;

Status::Status()
{
    Clear();
}

void Status::Clear()
{
    memset(&id, 0, sizeof(Status));
}

Status& Status::operator+=(const Status& _add)
{
    MOV    += _add.MOV;
    
    HP     += _add.HP;
    HP_gPc += _add.HP_gPc;
    
    MaxHP     += _add.MaxHP;
    MaxHP_gPc += _add.MaxHP_gPc;
    
    MP     += _add.MP;
    MP_gPc += _add.MP_gPc;
    
    MaxMP     += _add.MaxMP;
    MaxMP_gPc += _add.MaxMP_gPc;
    
    ATKS    += _add.ATKS;
    ATKS_gPc += _add.ATKS_gPc;
    
    ATKL         += _add.ATKL;
    ATKL_gPc     += _add.ATKL_gPc;
    
    DEF     += _add.DEF;
    DEF_gPc += _add.DEF_gPc;
    
    Dodge     += _add.Dodge;
    Dodge_gPc += _add.Dodge_gPc;
    
    Hit += _add.Hit;
    Hit_gPc += _add.Hit_gPc;
    
    Hit_Rating += _add.Hit_Rating;
    Critical_Rating += _add.Critical_Rating;
    Dodge_Rating += _add.Dodge_Rating;
    
    Damage_gPc += _add.Damage_gPc;
    Recover_gPc -= _add.Recover_gPc;
    
    DamageImmunit_gPc += _add.DamageImmunit_gPc;
    RecoverImmunit_gPc += _add.RecoverImmunit_gPc;

    return *this;
}

Status& Status::operator-=(const Status& _add)
{
    MOV    -= _add.MOV;
    
    HP     -= _add.HP;
    HP_gPc -= _add.HP_gPc;
    
    MaxHP     -= _add.MaxHP;
    MaxHP_gPc -= _add.MaxHP_gPc;
    
    MP     -= _add.MP;
    MP_gPc -= _add.MP_gPc;
    
    MaxMP     -= _add.MaxMP;
    MaxMP_gPc -= _add.MaxMP_gPc;
    
    ATKS    -= _add.ATKS;
    ATKS_gPc -= _add.ATKS_gPc;
    
    ATKL         -= _add.ATKL;
    ATKL_gPc     -= _add.ATKL_gPc;
    
    DEF     -= _add.DEF;
    DEF_gPc -= _add.DEF_gPc;
    
    Dodge     -= _add.Dodge;
    Dodge_gPc -= _add.Dodge_gPc;
    
    Hit -= _add.Hit;
    Hit_gPc -= _add.Hit_gPc;
    
    Hit_Rating -= _add.Hit_Rating;
    Critical_Rating -= _add.Critical_Rating;
    Dodge_Rating -= _add.Dodge_Rating;
    
    Damage_gPc -= _add.Damage_gPc;
    Recover_gPc -= _add.Recover_gPc;
    
    DamageImmunit_gPc -= _add.DamageImmunit_gPc;
    RecoverImmunit_gPc -= _add.RecoverImmunit_gPc;

    return *this;
}

Status& Status::operator*=(const float scale)
{
    MOV    *= scale;
    
    HP     *= scale;
    HP_gPc *= scale;
    
    MaxHP     *= scale;
    MaxHP_gPc *= scale;
    
    MP     *= scale;
    MP_gPc *= scale;
    
    MaxMP     *= scale;
    MaxMP_gPc *= scale;
    
    ATKS    *= scale;
    ATKS_gPc *= scale;
    
    ATKL         *= scale;
    ATKL_gPc     *= scale;
    
    DEF     *= scale;
    DEF_gPc *= scale;
    
    Dodge     *= scale;
    Dodge_gPc *= scale;
    
    Hit *= scale;
    Hit_gPc *= scale;
    
    Hit_Rating *= scale;
    Critical_Rating *= scale;
    Dodge_Rating *= scale;
    
    Damage_gPc *= scale;
    Recover_gPc *= scale;
    
    DamageImmunit_gPc *= scale;
    RecoverImmunit_gPc *= scale;
    return *this;
}

void Status::Calc(const Status& _add)
{
    Clear();
    HP = MAX(_add.HP*(1+_add.HP_gPc),0);
    MaxHP = MAX(_add.MaxHP*(1+_add.MaxHP_gPc),0);
    MP = MAX(_add.MP*(1+_add.MP_gPc),0);
    MaxMP = MAX(_add.MaxMP*(1+_add.MaxMP_gPc),0);
    ATKS = MAX(_add.ATKS*(1+_add.ATKS_gPc),0);
    ATKL = MAX(_add.ATKL*(1+_add.ATKL_gPc),0);
    DEF = MAX(_add.DEF*(1+_add.DEF_gPc),0);
    Dodge = MAX(_add.Dodge*(1+_add.Dodge_gPc),0);
    Hit = MAX(_add.Hit*(1+_add.Hit_gPc),0);
    
    MOV = _add.MOV;
    Hit_Rating = _add.Hit_Rating;
    Critical_Rating = _add.Critical_Rating;
    Dodge_Rating = _add.Dodge_Rating;
    Damage_gPc = _add.Damage_gPc;
    Recover_gPc = _add.Recover_gPc;
    DamageImmunit_gPc = _add.DamageImmunit_gPc;
    RecoverImmunit_gPc = _add.RecoverImmunit_gPc;
}

void Status::Plus(const Status& _orig, const Status& _plus)
{
    MOV = _orig.MOV + _plus.MOV;
    
    HP = _orig.HP + _plus.HP;
    HP_gPc = _orig.HP_gPc + _plus.HP_gPc;
    
    MaxHP = _orig.MaxHP + _plus.MaxHP;
    MaxHP_gPc = _orig.MaxHP_gPc + _plus.MaxHP_gPc;
    
    MP = _orig.MP + _plus.MP;
    MP_gPc = _orig.MP_gPc + _plus.MP_gPc;
    
    MaxMP = _orig.MaxMP + _plus.MaxMP;
    MaxMP_gPc = _orig.MaxMP_gPc + _plus.MaxMP_gPc;
    
    ATKS = _orig.ATKS + _plus.ATKS;
    ATKS_gPc = _orig.ATKS_gPc + _plus.ATKS_gPc;
    
    ATKL = _orig.ATKL + _plus.ATKL;
    ATKL_gPc = _orig.ATKL_gPc + _plus.ATKL_gPc;
    
    DEF = _orig.DEF + _plus.DEF;
    DEF_gPc = _orig.DEF_gPc + _plus.DEF_gPc;
    
    Dodge = _orig.Dodge + _plus.Dodge;
    Dodge_gPc = _orig.Dodge_gPc + _plus.Dodge_gPc;
    
    Hit = _orig.Hit + _plus.Hit;
    Hit_gPc = _orig.Hit_gPc + _plus.Hit_gPc;
    
    Hit_Rating = _orig.Hit_Rating + _plus.Hit_Rating;
    Critical_Rating = _orig.Critical_Rating + _plus.Critical_Rating;
    Dodge_Rating = _orig.Dodge_Rating + _plus.Dodge_Rating;
    
    Damage_gPc = _orig.Damage_gPc + _plus.Damage_gPc;
    Recover_gPc = _orig.Recover_gPc + _plus.Recover_gPc;
    
    DamageImmunit_gPc = _orig.DamageImmunit_gPc + _plus.DamageImmunit_gPc;
    RecoverImmunit_gPc = _orig.RecoverImmunit_gPc + _plus.RecoverImmunit_gPc;
}

Status& Status::Add(const Status& _add, S32 _count /* = 1 */)
{
    MOV    += _add.MOV *_count;
    
    HP     += _add.HP * _count;
    HP_gPc += _add.HP_gPc * _count;
    
    MaxHP     += _add.MaxHP * _count;
    MaxHP_gPc += _add.MaxHP_gPc * _count;
    
    MP     += _add.MP * _count;
    MP_gPc += _add.MP_gPc * _count;
    
    MaxMP     += _add.MaxMP * _count;
    MaxMP_gPc += _add.MaxMP_gPc * _count;
    
    ATKS    += _add.ATKS * _count;
    ATKS_gPc += _add.ATKS_gPc * _count;
    
    ATKL         += _add.ATKL * _count;
    ATKL_gPc     += _add.ATKL_gPc * _count;
    
    DEF     += _add.DEF * _count;
    DEF_gPc += _add.DEF_gPc * _count;
    
    Dodge     += _add.Dodge * _count;
    Dodge_gPc += _add.Dodge_gPc * _count;
    
    Hit += _add.Hit * _count;
    Hit_gPc += _add.Hit_gPc * _count;
    
    Hit_Rating += _add.Hit_Rating * _count;
    Critical_Rating += _add.Critical_Rating * _count;
    Dodge_Rating += _add.Dodge_Rating * _count;
    
    Damage_gPc += _add.Damage_gPc * _count;
    Recover_gPc -= _add.Recover_gPc * _count;
    
    DamageImmunit_gPc += _add.DamageImmunit_gPc * _count;
    RecoverImmunit_gPc += _add.RecoverImmunit_gPc * _count;
    return *this;
}

StatusManager::StatusManager()
{
    
}

StatusManager::~StatusManager()
{
    m_statusMap.clear();
}

StatusManager* StatusManager::getInstance()
{
    if(s_StatusManager == nullptr){
        s_StatusManager = new StatusManager();
    }
    return s_StatusManager;
}

bool StatusManager::read()
{
    ctest* test = ctest::getInstanse();
    if(!test->readCSV("StatusData.csv")){
        CCLOG("读取StatusData.csv错误");
        return false;
    }
    
    while (1) {
        Status status;
        status.id   = test->readAsInt();        //id
        test->readAsString();                   //描述跳过
        status.MOV  = test->readAsInt();        //移动力
        
        status.HP   = test->readAsInt();					// 生命
        status.HP_gPc   = test->readAsFloat();				// 生命百分比
        
        status.MaxHP   = test->readAsInt();				// 最大生命
        status.MaxHP_gPc   = test->readAsFloat();			// 最大生命百分比
        
        status.MP   = test->readAsInt();                 // 魔法
        status.MP_gPc   = test->readAsFloat();             // 魔法百分比
        
        status.MaxMP   = test->readAsInt();              // 最大魔法
        status.MaxMP_gPc   = test->readAsFloat();          // 最大魔法百分比
        
        status.ATKS   = test->readAsInt();               // 格斗
        status.ATKS_gPc   = test->readAsFloat();           // 格斗百分比
        
        status.ATKL   = test->readAsInt();               // 射击
        status.ATKL_gPc   = test->readAsFloat();           // 射击百分比
        
        status.DEF   = test->readAsInt();                // 防御
        status.DEF_gPc   = test->readAsFloat();            // 防御百分比
        
        status.Dodge   = test->readAsInt();              // 回避
        status.Dodge_gPc   = test->readAsFloat();          // 回避百分比
        
        status.Hit   = test->readAsInt();                // 命中
        status.Hit_gPc   = test->readAsFloat();            // 命中百分比
        
        status.Hit_Rating   = test->readAsInt();         //命中几率
        status.Critical_Rating   = test->readAsInt();    // 暴击几率
        status.Dodge_Rating   = test->readAsInt();       // 回避几率
        
        status.Damage_gPc   = test->readAsFloat();         // 伤害调整百分比
        status.Recover_gPc   = test->readAsFloat();        // 治疗调整百分比
        
        status.DamageImmunit_gPc   = test->readAsFloat();  // 受伤害调整百分比
        status.RecoverImmunit_gPc   = test->readAsFloat(); // 受治疗调整百分比
        
        status.Exp_gPc   = test->readAsFloat();    //经验调整百分比
        
        m_statusMap.insert(STATUS_MAP::value_type(status.id,status));
        
        if (!test->size())
            break;
    }
    
    return true;
}

Status StatusManager::getStatus(U32 statusId)
{
    Status temp;
    STATUS_MAPITR itr = m_statusMap.find(statusId);
    if(itr != m_statusMap.end()){
        temp = itr->second;
    }
    return temp;
}


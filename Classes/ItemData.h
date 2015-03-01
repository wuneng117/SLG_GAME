//
//  ItemData.h
//  testproject
//
//  Created by ZHOUTAO on 15/1/22.
//
//

#ifndef __testproject__ItemData__
#define __testproject__ItemData__

#include <stdio.h>

struct ItemData
{
    
    ItemData():m_type(NONE)
    {
    }
    ~ItemData()
    {
    }
    
    
    //物品种类
    enum ItemType
    {
        NONE       = 0,
        WEAPON     = 1,
        ARMOR      = 2,
        JEWEILY    = 4
    };
    
    ItemType m_type;        //类型
    int id;                 //id
    int m_state;            //属性
    std::string m_name;     //名称
    std::string m_des;      //描述
};

#endif /* defined(__testproject__ItemData__) */

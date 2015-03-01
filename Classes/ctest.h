//
//  ctest.h
//  lantisi
//
//  Created by ZHOUTAO on 14-6-30.
//
//

#ifndef __lantisi__ctest__
#define __lantisi__ctest__

#include <iostream>
#include "cocos2d.h"
#include <string>

using namespace cocos2d;

enum COMMAND{
    COM_TEXT = 0,
    COM_NAME
};


class scriptNode :public Layer{
    
public:
    scriptNode();
    ~scriptNode();
    virtual bool init();
    CREATE_FUNC(scriptNode);
    
    virtual bool onTouchesBegan(Touch *touch, Event *unused_event);
    
protected:
    Label* m_text;       //剧本显示
    Label* m_name;       //角色名字显示
    Sprite* m_bg;        //背景

};

class ctest{
    
public:
    ctest();
    ~ctest();
    static ctest* getInstanse();
    //clear
    void clear();
    //返回长度
    bool size();
    //读文件
    bool readFile(const char* filename);
    //读csv
    bool readCSV(const char* filename);
    //----------------------------读数据用----------------------------------
    //读一个逗号
    std::string readAsString();
    int readAsInt();
    float readAsFloat();
    //---------------------------脚本解析用---------------------------------
    //读命令行直到文本为止
    std::vector<std::string> getCommand();
    //解释命令行作用
    int explainCommand(std::string command);
    
private:
    
    std::string m_buffer;           //文件流
};

#endif /* defined(__lantisi__ctest__) */


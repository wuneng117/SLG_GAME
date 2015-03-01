//
//  ctest.cpp
//  lantisi
//
//  Created by ZHOUTAO on 14-6-30.
//
//

#include "ctest.h"

static ctest* m_textReader = NULL;

scriptNode::scriptNode()
{
    
}

scriptNode::~scriptNode()
{
    
}

bool scriptNode::init()
{
    if(!Layer::init()){
        return false;
    }
    
    auto size = Director::getInstance()->getWinSize();
    
    //背景
    m_bg = Sprite::create("bg.jpg");
    m_bg->setPosition(Point(size.width/2, size.height/2));
    addChild(m_bg);
    
    //初始化名字显示
    m_name = Label::createWithSystemFont("", "黑体", 32);
    m_name->setPosition(Point(0, size.height/3));
    m_name->setAnchorPoint(Point(0,0.5));
    addChild(m_name);
    
    //初始化文本
    m_text = Label::createWithSystemFont("", "黑体", 28, Size(640,size.height/5));
    m_text->setPosition(Point(0, size.height/5));
    m_text->setAnchorPoint(Point(0,1));
    addChild(m_text);
    
    //添加触摸
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(scriptNode::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
//    std::vector<std::string> tempCommand = ctest::getInstanse()->getCommand();
//    CCLOG("名字：%s",tempCommand[0].c_str());
//    CCLOG("内容：%s",tempCommand[1].c_str());
    
//    for(int i=0;i<tempCommand.size();++i){
//        if(ctest::getInstanse()->explainCommand(tempCommand[i]) == 1)
//            m_name->setString(tempCommand[i].c_str());
//        if(ctest::getInstanse()->explainCommand(tempCommand[i]) == 0)
//            m_text->setString(tempCommand[i]);
//    }
    return true;
}

bool scriptNode::onTouchesBegan(Touch *pTouch, Event *pEvent)
{
    CCLOG("script touches");
    
//    std::vector<std::string> tempCommand = ctest::getInstanse()->getCommand();
//    
//    if(tempCommand.size() == 0){
//        CCLOG("结束");
//    }
//    else{
//        for(int i=0;i<tempCommand.size();++i){
//            if(ctest::getInstanse()->explainCommand(tempCommand[i]) == 1)
//                m_name->setString(tempCommand[i].c_str());
//            if(ctest::getInstanse()->explainCommand(tempCommand[i]) == 0)
//                m_text->setString(tempCommand[i]);
//        }
//    }
    return true;
}


ctest::ctest()
{
    
}

ctest::~ctest()
{
    
}

ctest* ctest::getInstanse()
{
    if(!m_textReader){
        m_textReader = new ctest();
    }
    
    return m_textReader;
}

void ctest::clear()
{
    m_buffer.clear();
}

bool ctest::size()
{
    return m_buffer.size();
}

bool ctest::readFile(const char* filename)
{
    std::string path = FileUtils::getInstance()->fullPathForFilename(filename);
    CCLOG("读取路径：%s",path.c_str());
    CCLOG("开始读取时间:%f",(float)clock()/CLOCKS_PER_SEC);
    FILE *file = fopen(path.c_str(), "r");
    if(file){
        fseek(file, 0, SEEK_END);
        
        int lenth = ftell(file);
        CCLOG("文件字节长度：%d",lenth);
        rewind(file);
        char* buf = new char[lenth];
        memset(buf, 0, lenth);
        int nlenth = fread(buf, sizeof(char), lenth, file);
        CCLOG("读取完毕时间：%f",(float)clock()/CLOCKS_PER_SEC);
        buf[nlenth] = '\0';
        CCLOG("文件长度:%d",nlenth);
        CCLOG("read file = %s",buf);
        
        m_buffer = buf;
        delete [] buf;
        fclose(file);
    }
    else{
        CCLOG("文件不存在");
        return false;
    }
    return true;
}

bool ctest::readCSV(const char* filename)
{
    if(!readFile(filename))
        return false;
    
    //跳过标题行
    std::string::iterator it = m_buffer.begin();
    while(1){
        if(*it == 10){
            m_buffer.erase(m_buffer.begin(), it+1);
            break;
        }
        it++;
    }
    return true;
}

std::string ctest::readAsString()
{
    std::string::iterator it = m_buffer.begin();
    std::string tempout;
    while(1){
        if(*it == 44 ||*it == 10 ||it == m_buffer.end()){
            m_buffer.erase(m_buffer.begin(), it+1);
            break;
        }
        tempout = tempout + *it;
        it++;
    }
    return tempout;
}

int ctest::readAsInt()
{
    return atoi(readAsString().c_str());
}

float ctest::readAsFloat()
{
    return atof(readAsString().c_str());
}

std::vector<std::string> ctest::getCommand()
{
    std::vector<std::string> commandVector;
    std::string::iterator it;
    std::string tempout;
    it = m_buffer.begin();

    while (1) {
        //末尾返回
        if(it == m_buffer.end()){
            CCLOG("读取命令结束");
            CCLOG("最后读入的对话：%s",tempout.c_str());
            
            m_buffer.erase(m_buffer.begin(), it);
            
            if(tempout.size() == 0)
                return commandVector;
            else{
                commandVector.push_back(tempout);
                return commandVector;
            }
        }
        //判断字符，进行处理
        switch (*it) {
            //换行
            case 10:
            {
                commandVector.push_back(tempout);
                CCLOG("最后读入的命令行：%s",tempout.c_str());
                tempout = "";
            }
                break;
            //双引号
            case 34:
            {
                tempout = *it;
                it++;
                while(1){
                    tempout = tempout + *it;

                    if(*it == 34){
                        commandVector.push_back(tempout);
                        m_buffer.erase(m_buffer.begin(), it+1);
                        return commandVector;
                    }
                    it++;
                }
            }
                break;
            default:
                tempout = tempout + *it;
                break;
        }
        
        it++;
    }
}

int ctest::explainCommand(std::string command)
{
    if(command[0] == 34)
        return 0;
    return 1;
    
}

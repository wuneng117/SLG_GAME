//
//  MapHelper.h
//  testproject
//
//  Created by ZHOUTAO on 15/1/18.
//
//

#ifndef __testproject__MapHelper__
#define __testproject__MapHelper__

#include <stdio.h>
#include "cocos2d.h"

const int TILE_WIDTH  = 75;
const int TILE_HEIGHT = 75;
const int MAP_ROW     = 5;
const int MAP_COLUMN  = 13;

const int MAP_NUM = MAP_COLUMN*MAP_ROW;

struct sMapPoint;
struct sMapUnit;
class HelloWorld;
class SkillData;

class MapHelper
{
private:
    HelloWorld* helloWorld;
    
public:
    MapHelper(HelloWorld* hello);
    ~MapHelper();
    
    //坐标转换
    cocos2d::Point touchToPos(cocos2d::Point touch);
    cocos2d::Point posToTouch(cocos2d::Point pos);
    
    cocos2d::Vector<cocos2d::FiniteTimeAction*> moveUnitToPos(sMapUnit* unit, cocos2d::Point pos);    //计算移动路径
    cocos2d::Vector<cocos2d::FiniteTimeAction*> moveUnitToUnit(sMapUnit* source, sMapUnit* target);  // 计算单位向单位移动的路径

    bool searchAStar(sMapPoint* mapPoint, sMapPoint* endPoint);
    void checkAStarPoint(int x, int y, sMapPoint* parent, sMapPoint* endPoint);
    sMapUnit* getUnitByPos(cocos2d::Point pos);     //获取坐标上的单位
    
    void showMove(int move, cocos2d::Point pos);    //显示可移动路径
    void showMoveAll();     //显示所有移动路径
    void hideMove();    //隐藏移动路径
    
    void showAtkRegion(SkillData* data, cocos2d::Point source_pos, cocos2d::Point pos);     //显示范围攻击地点
    void showAtk(SkillData* data, cocos2d::Point pos);  //显示攻击范围
    void showAtkAll();  //显示所有攻击地点
    void hideAtk();     //隐藏攻击地点
    
    void showAtkBlock(int x, int y);      // 显示攻击块的点
    
    std::vector<sMapUnit*> checkAtkUnit();    //返回攻击范围内可以攻击的单位
    bool checkPosEmpty(cocos2d::Point pos);    //检查地点上是否有单位或者出界

};
#endif /* defined(__testproject__MapHelper__) */

//
//  MapHelper.cpp
//  testproject
//
//  Created by ZHOUTAO on 15/1/18.
//
//

#include "MapHelper.h"
#include "HelloWorldScene.h"
#include "SkillData.h"

USING_NS_CC;

MapHelper::MapHelper(HelloWorld* hello)
{
    helloWorld = hello;
}

MapHelper::~MapHelper()
{
    
}

Point MapHelper::touchToPos(cocos2d::Point point)
{
    Point pos;
    pos.x = (int)(point.x/TILE_WIDTH);
    pos.y = MAP_ROW-1-(int)(point.y/TILE_HEIGHT);
    return pos;
}

Point MapHelper::posToTouch(Point pos)
{
    Point point;
    point.x = pos.x*TILE_WIDTH + TILE_WIDTH/2;
    point.y = (MAP_ROW-pos.y-1)*TILE_HEIGHT + TILE_HEIGHT/2;
    return point;
}

Vector<FiniteTimeAction*> MapHelper::moveUnitToPos(sMapUnit *unit, cocos2d::Point pos)
{
    Vector<FiniteTimeAction*> actLine;
    
    sMapPoint** map = helloWorld->map;
    
    int desx = unit->pos.x;
    int desy = unit->pos.y;
    int srcx = pos.x;
    int srcy = pos.y;
    
    if(pos.x < 0 || pos.x >= MAP_COLUMN ||pos.y< 0 || pos.y >= MAP_ROW ||
       pos == unit->pos || getUnitByPos(pos) || !helloWorld->mapMove[srcx+srcy*MAP_COLUMN]->isVisible())
        return actLine;
    
    //开始寻路
    //起点
    sMapPoint* startPoint = map[srcy*MAP_COLUMN+srcx];
    startPoint->bStart = true;
    startPoint->g = 0;
    startPoint->h = abs(desx-srcx)+abs(desy-srcy);
    startPoint->f = startPoint->g+startPoint->h;
    startPoint->bClose = true;
    
    //终点
    sMapPoint* endPoint = map[desy*MAP_COLUMN+desx];
    endPoint->bEnd = true;
    
    if(searchAStar(startPoint,endPoint)){
        sMapPoint* tempPoint = endPoint->sParnet;
        while (tempPoint){
            CCLOG("%f,%f",tempPoint->pos.x,tempPoint->pos.y);
            auto act = MoveTo::create(0.1,posToTouch(Point(tempPoint->pos.x,tempPoint->pos.y)));
            actLine.pushBack(act);
            tempPoint = tempPoint->sParnet;
        }
    }

    //重置map
    for(int i=0;i<MAP_NUM;i++){
        map[i]->f = 0;
        map[i]->g = 0;
        map[i]->h = 0;
        map[i]->bClose = false;
        map[i]->bOpen = false;
        map[i]->bStart = false;
        map[i]->bEnd = false;
        map[i]->sParnet = NULL;
    }
    
    return actLine;
}

Vector<FiniteTimeAction*> MapHelper::moveUnitToUnit(sMapUnit* source, sMapUnit* target)
{
    Vector<FiniteTimeAction*> act;
    
    if(!target)
        return act;

    std::vector<Point> posVec;
    std::vector<Vector<FiniteTimeAction*>> actLine;
    
    // 保存所有能移动到的地点
    for (int x=0; x<MAP_COLUMN; x++) {
        for (int y=0; y<MAP_ROW; y++) {
            Point pos = Point(x,y);
            
            helloWorld->selectedUnit = source;
            showMove(source->move, source->pos);
            Vector<FiniteTimeAction*> tempact = moveUnitToPos(source, pos);
            hideMove();
            if (tempact.size()) {
                posVec.push_back(pos);
                actLine.push_back(tempact);
            }
        }
    }
    
    int size = MAP_NUM;
    std::vector<Point>::iterator it=posVec.begin();
    // 对所有地点进行计算，找出移动到目标距离最短的地点
    for (int i=0; i<posVec.size(); i++, it++) {
        showMoveAll();
        helloWorld->selectedUnit = target;
        Vector<FiniteTimeAction*> _tempact = moveUnitToPos(target, *it);
        hideMove();
        if (_tempact.size() && _tempact.size() < size) {
            act = actLine.at(i);
            size = _tempact.size();
        }
    }
    
    return act;
}

bool MapHelper::searchAStar(sMapPoint* mapPoint, sMapPoint* endPoint)
{
    sMapPoint** map = helloWorld->map;
    
    int x = mapPoint->pos.x;
    int y = mapPoint->pos.y;
    
    checkAStarPoint(x, y-1, mapPoint, endPoint);   //上
    checkAStarPoint(x, y+1, mapPoint, endPoint);   //下
    checkAStarPoint(x-1, y, mapPoint, endPoint);     //左
    checkAStarPoint(x+1, y, mapPoint, endPoint);     //右

    
    int tempf = 999999;
    sMapPoint* point_f = NULL;
    
    for(int i=0;i<MAP_NUM;i++){
        if(map[i]->bOpen){
            if(map[i]->f < tempf){
                tempf = map[i]->f;
                point_f = map[i];
            }
        }
    }
    
    if(point_f == NULL)
        return false;
    
    if(point_f->bEnd == true)
        return true;
    
    point_f->bClose = true;
    point_f->bOpen = false;
    
    return searchAStar(point_f, endPoint);
}

void MapHelper::checkAStarPoint(int x, int y, sMapPoint* parent, sMapPoint* endPoint)
{
    sMapPoint** map = helloWorld->map;
    Point temp = Point(x,y);
    if((!checkPosEmpty(temp)) && temp != helloWorld->selectedUnit->pos)
        return;
    
    if(helloWorld->mapMove[x+y*MAP_COLUMN]->isVisible() == false && temp != helloWorld->selectedUnit->pos)
        return;
    
    sMapPoint* point = map[x+y*MAP_COLUMN];
    
    if(point->bClose == true)
        return;
    if (point->bOpen == false) {
        point->bOpen = true;
        point->sParnet = parent;
        point->g = parent->g+1;
        point->h = abs(endPoint->pos.x-point->pos.x)+abs(endPoint->pos.y-point->pos.y);
        point->f = point->g+point->h;
    }
    else{
        int g = parent->g+1;
        if(g < point->g){
            point->sParnet = parent;
            point->g = g;
            point->f = point->g+point->h;
        }
    }
}

sMapUnit* MapHelper::getUnitByPos(Point pos)
{
    for(int i=0;i<6;i++){
        if(helloWorld->enermyUnit[i] && helloWorld->enermyUnit[i]->pos == pos)
            return helloWorld->enermyUnit[i];
        if(helloWorld->myUnit[i] && helloWorld->myUnit[i]->pos == pos)
            return helloWorld->myUnit[i];
    }
    
    return NULL;
}

void MapHelper::showMove(int move, Point pos)
{
    if(move <= 0)
        return;
    
    move--;
    
    int x = pos.x;
    int y = pos.y;
    
    if(checkPosEmpty(Point(x-1,y))){
        helloWorld->mapMove[x-1+y*MAP_COLUMN]->setVisible(true);
        showMove(move, Point(x-1,y));
    }
    
    if(checkPosEmpty(Point(x+1,y))){
        helloWorld->mapMove[x+1+y*MAP_COLUMN]->setVisible(true);
        showMove(move, Point(x+1,y));
    }
    
    if(checkPosEmpty(Point(x,y-1))){
        helloWorld->mapMove[x+(y-1)*MAP_COLUMN]->setVisible(true);
        showMove(move, Point(x,y-1));
    }
    
    if(checkPosEmpty(Point(x,y+1))){
        helloWorld->mapMove[x+(y+1)*MAP_COLUMN]->setVisible(true);
        showMove(move, Point(x,y+1));
    }
}

void MapHelper::showMoveAll()
{
    for (int i=0; i<MAP_NUM; i++) {
        helloWorld->mapMove[i]->setVisible(true);
    }
}

void MapHelper::hideMove()
{
    for(int i=0;i<MAP_NUM;i++)
        helloWorld->mapMove[i]->setVisible(false);
}

void MapHelper::showAtkRegion(SkillData *data, Point source_pos, Point pos)
{
    helloWorld->touchState = HelloWorld::eTouchState::SelectUnitForSkillByRegion;

    if (data->m_region == SkillData::eRegion::Region_Cross) {
        showAtkBlock(pos.x, pos.y);
        showAtkBlock(pos.x-1, pos.y);
        showAtkBlock(pos.x+1, pos.y);
        showAtkBlock(pos.x, pos.y-1);
        showAtkBlock(pos.x, pos.y+1);
    }
    else if(data->m_region ==SkillData::eRegion::Region_Cross_2 ) {
        showAtkBlock(pos.x, pos.y);
        showAtkBlock(pos.x-1, pos.y-1);
        showAtkBlock(pos.x+1, pos.y-1);
        showAtkBlock(pos.x-1, pos.y+1);
        showAtkBlock(pos.x+1, pos.y+1);
    }
    else if(data->m_region ==SkillData::eRegion::Region_Line ) {
        //上下
        if (abs(source_pos.x - pos.x) < abs(source_pos.y - pos.y)) {
            showAtkBlock(pos.x, pos.y);
            showAtkBlock(pos.x, pos.y-1);
            showAtkBlock(pos.x, pos.y-2);
            showAtkBlock(pos.x, pos.y+1);
            showAtkBlock(pos.x, pos.y+2);
        }
        //左右
        else {
            showAtkBlock(pos.x, pos.y);
            showAtkBlock(pos.x-1, pos.y);
            showAtkBlock(pos.x-2, pos.y);
            showAtkBlock(pos.x+1, pos.y);
            showAtkBlock(pos.x+2, pos.y);
        }
    }
    else if(data->m_region ==SkillData::eRegion::Region_Line_3 ) {
        if (abs(source_pos.x - pos.x) < abs(source_pos.y - pos.y)) {
            //下
            if (pos.y > source_pos.y) {
                showAtkBlock(pos.x, pos.y);
                showAtkBlock(pos.x, pos.y+1);
                showAtkBlock(pos.x-2, pos.y);
                showAtkBlock(pos.x-2, pos.y+1);
                showAtkBlock(pos.x+2, pos.y);
                showAtkBlock(pos.x+2, pos.y+1);
            }
            //上
            else {
                showAtkBlock(pos.x, pos.y);
                showAtkBlock(pos.x, pos.y-1);
                showAtkBlock(pos.x-2, pos.y);
                showAtkBlock(pos.x-2, pos.y-1);
                showAtkBlock(pos.x+2, pos.y);
                showAtkBlock(pos.x+2, pos.y-1);
            }

        }
        else {
            //右
            if (pos.x > source_pos.x) {
                showAtkBlock(pos.x, pos.y);
                showAtkBlock(pos.x+1, pos.y);
                showAtkBlock(pos.x, pos.y-2);
                showAtkBlock(pos.x+1, pos.y-2);
                showAtkBlock(pos.x, pos.y+2);
                showAtkBlock(pos.x+1, pos.y+2);
            }
            //左
            else {
                showAtkBlock(pos.x, pos.y);
                showAtkBlock(pos.x-1, pos.y);
                showAtkBlock(pos.x, pos.y-2);
                showAtkBlock(pos.x-1, pos.y-2);
                showAtkBlock(pos.x, pos.y+2);
                showAtkBlock(pos.x-1, pos.y+2);
            }
        }
    }
    else if(data->m_region ==SkillData::eRegion::Region_Squre ) {
        showAtkBlock(pos.x, pos.y);
        showAtkBlock(pos.x, pos.y+1);
        showAtkBlock(pos.x+1, pos.y);
        showAtkBlock(pos.x+1, pos.y+1);
    }
    else if(data->m_region ==SkillData::eRegion::Region_Triangle ) {
        if (abs(source_pos.x - pos.x) < abs(source_pos.y - pos.y)) {
            //下
            if (pos.y > source_pos.y) {
                showAtkBlock(pos.x, pos.y);
                showAtkBlock(pos.x-1, pos.y+1);
                showAtkBlock(pos.x+1, pos.y+1);
                showAtkBlock(pos.x, pos.y+2);
                showAtkBlock(pos.x-2, pos.y+2);
                showAtkBlock(pos.x+2, pos.y+2);
            }
            //上
            else {
                showAtkBlock(pos.x, pos.y);
                showAtkBlock(pos.x-1, pos.y-1);
                showAtkBlock(pos.x+1, pos.y-1);
                showAtkBlock(pos.x, pos.y-2);
                showAtkBlock(pos.x-2, pos.y-2);
                showAtkBlock(pos.x+2, pos.y-2);
            }
            
        }
        else {
            //右
            if (pos.x > source_pos.x) {
                showAtkBlock(pos.x, pos.y);
                showAtkBlock(pos.x+1, pos.y-1);
                showAtkBlock(pos.x+1, pos.y+1);
                showAtkBlock(pos.x+2, pos.y);
                showAtkBlock(pos.x+2, pos.y-2);
                showAtkBlock(pos.x+2, pos.y+2);
            }
            //左
            else {
                showAtkBlock(pos.x, pos.y);
                showAtkBlock(pos.x-1, pos.y-1);
                showAtkBlock(pos.x-1, pos.y+1);
                showAtkBlock(pos.x-2, pos.y);
                showAtkBlock(pos.x-2, pos.y-2);
                showAtkBlock(pos.x-2, pos.y+2);
            }
        }
    }
}

void MapHelper::showAtk(SkillData* data, Point pos)
{
    int range_s = data->m_range_s;
    int range_l = data->m_range_l;
    
    for (int i=0; i<MAP_NUM; i++) {
        int x = i%MAP_COLUMN;
        int y = i/MAP_COLUMN;
        
        // 判断目标地点是否在射程范围内
        if (abs(x-pos.x)+abs(y-pos.y) >= range_s && abs(x-pos.x)+abs(y-pos.y) <= range_l &&
            x>=0 && x<MAP_COLUMN && y>=0 && y<MAP_ROW &&
            Point(x,y) != pos)
            helloWorld->mapAtk[i]->setVisible(true);
    }
}

void MapHelper::showAtkAll()
{
    for (int i=0; i<MAP_NUM; i++) {
        helloWorld->mapAtk[i]->setVisible(true);
    }
}

void MapHelper::hideAtk()
{
    for(int i=0;i<MAP_NUM;i++)
        helloWorld->mapAtk[i]->setVisible(false);
}

void MapHelper::showAtkBlock(int x, int y)
{
    if(x < 0 || x > MAP_COLUMN-1|| y > MAP_ROW-1 || y < 0)
        return;
    
    helloWorld->mapAtk[x+y*MAP_COLUMN]->setVisible(true);
}

std::vector<sMapUnit*> MapHelper::checkAtkUnit()
{
    std::vector<sMapUnit*> vec;
    
    for (int i=0; i<MAP_NUM; i++) {
        if (!helloWorld->mapAtk[i]->isVisible())
            continue;
        
        int x = i%MAP_COLUMN;
        int y = i/MAP_COLUMN;
        
        sMapUnit* unit = getUnitByPos(Point(x,y));
        if(!unit)
            continue;
        
        sMapUnit* self = helloWorld->selectedUnit;
        SkillData* data = helloWorld->selectedSkill;
        
        if (data->m_target & SkillData::eTarget::Target_Friend && unit->m_force == self->m_force) {
            vec.push_back(unit);
        }
        else if (data->m_target & SkillData::eTarget::Target_Enermy && unit->m_force != self->m_force) {
            vec.push_back(unit);
        }
    }
    
    return vec;
}

bool MapHelper::checkPosEmpty(Point pos)
{
    if(getUnitByPos(pos))
        return false;
    
    if(pos.x < 0 || pos.x > MAP_COLUMN-1|| pos.y > MAP_ROW-1 || pos.y < 0)
        return false;
    
    return true;
}
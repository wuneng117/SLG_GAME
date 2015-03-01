//
//  SpecialSprite.h
//  quick_md
//
//  Created by ZHOUTAO on 14-10-13.
//
//

#ifndef __quick_md__SpecialSprite__
#define __quick_md__SpecialSprite__

#include "cocos2d.h"

using namespace cocos2d;

class GraySprite : public Sprite
{
public:

//create函数必须重写，与CCSprite保持一致
    static GraySprite* create();
    
    static GraySprite* create(const std::string& pszFileName);
    
    static GraySprite* create(const std::string& pszFileName, const Rect& rect);

    static GraySprite* createWithTexture(Texture2D *pTexture);
    
    static GraySprite* createWithTexture(Texture2D *pTexture, const Rect& rect);

    static GraySprite* createWithSpriteFrame(SpriteFrame *pSpriteFrame);

    static GraySprite* createWithSpriteFrameName(const std::string& pszSpriteFrameName);

    virtual bool initWithTexture(Texture2D *pTexture, const Rect& rect, bool rotated);
};


#endif /* defined(__quick_md__SpecialSprite__) */

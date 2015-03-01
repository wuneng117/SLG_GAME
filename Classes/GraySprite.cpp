//
//  SpecialSprite.cpp
//  quick_md
//
//  Created by ZHOUTAO on 14-10-13.
//
//

#include "GraySprite.h"

const GLchar * fragmentSource =
"                               \n \
#ifdef GL_ES                    \n \
precision mediump float;        \n \
#endif                          \n \
varying vec2 v_texCoord;        \n \
varying vec4 v_fragmentColor;   \n \
void main(void)                 \n \
{                               \n \
// Convert to greyscale using NTSC weightings           \n \
vec4 col = texture2D(CC_Texture0, v_texCoord);            \n \
float grey = dot(col.rgb, vec3(0.299, 0.587, 0.114));   \n \
gl_FragColor = vec4(grey, grey, grey, col.a);           \n \
}";

GraySprite* GraySprite::create()
{
    GraySprite *pSprite = new GraySprite();
    if (pSprite && pSprite->init())
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

GraySprite* GraySprite::create(const std::string& pszFileName)
{
    GraySprite *pobSprite = new GraySprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

GraySprite* GraySprite::create(const std::string& pszFileName, const Rect& rect)
{
    GraySprite *pobSprite = new GraySprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

GraySprite* GraySprite::createWithTexture(Texture2D *pTexture)
{
    GraySprite *pobSprite = new GraySprite();
    if (pobSprite && pobSprite->initWithTexture(pTexture, Rect::ZERO, false))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

GraySprite* GraySprite::createWithTexture(Texture2D *pTexture, const Rect& rect)
{
    GraySprite *pobSprite = new GraySprite();
    if (pobSprite && pobSprite->initWithTexture(pTexture, rect, false))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

GraySprite* GraySprite::createWithSpriteFrame(SpriteFrame *pSpriteFrame)
{
    GraySprite *pobSprite = new GraySprite();
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

GraySprite* GraySprite::createWithSpriteFrameName(const std::string& pszSpriteFrameName)
{
    SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pszSpriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName.c_str());
    CCAssert(pFrame != NULL, msg);
#endif
    
    return createWithSpriteFrame(pFrame);
}

bool GraySprite::initWithTexture(Texture2D *pTexture, const Rect& rect, bool rotated)
{

    if(!Sprite::initWithTexture(pTexture, rect, rotated))
        return false;
    
    GLProgram* pProgram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragmentSource);
    setGLProgram(pProgram);
    setGLProgramState(GLProgramState::create(pProgram));
    

    return true;
}

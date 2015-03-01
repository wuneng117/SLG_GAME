#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "Status.h"
#include "GameClientManager.h"
#include "SkillData.h"
#include "BattleReady_Scene.h"
#include "MapMain_Scene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    //add search
    FileUtils::getInstance()->addSearchPath("res/");
    FileUtils::getInstance()->addSearchPath("fg/");
    
    StatusManager::getInstance()->read();   //读取status
    HeroManager::getInstance()->read(); //读取herodata
    SkillManager::getInstance()->read();    //读取skilldata
    
    g_gameClientManager = new GameClientManager();
    g_gameClientManager->createNewGame();
    
    
    
    
    
    
    
    
    
    
    
    
    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
    
    auto scene = BattleReady_Scene::createScene();

    //auto scene = MapMain_Scene::createScene();
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

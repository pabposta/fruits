#include "AppDelegate.h"
#include "MainMenuScene.h"
#include "Coupon.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Fruits");
        director->setOpenGLView(glview);
    }

#ifdef _WIN32
	// set screen size on windows computers to portrait
	glview->setFrameSize(480, 640);
#endif

	glview->setDesignResolutionSize(480, 640, ResolutionPolicy::EXACT_FIT);
	director->setContentScaleFactor(1.0f);

	// turn off display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60.0f);

	// create the coupons for the demo
	UserDefault::getInstance()->setIntegerForKey(Coupon::TYPE_ONE.c_str(), 2);
	UserDefault::getInstance()->setIntegerForKey(Coupon::TYPE_TWO.c_str(), 2);
	UserDefault::getInstance()->setIntegerForKey(Coupon::TYPE_ONE_USED.c_str(), 1);
	UserDefault::getInstance()->setIntegerForKey(Coupon::TYPE_TWO_USED.c_str(), 0);

	// load sound effects
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("background.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("coupon.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("fruit.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("junk.wav");
	
    // create a scene. it's an autorelease object
    auto scene = MainMenuScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

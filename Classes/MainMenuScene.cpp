#include "MainMenuScene.h"
#include "GameScene.h"
#include "CouponListScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool MainMenuScene::init() {

	if ( !Layer::init() )
    {
        return false;
    }

	auto screenSize = Director::getInstance()->getWinSize();
	glClearColor(1.0, 1.0, 1.0, 1.0);

	auto background = Sprite::create("menu-background.png");
	addChild(background);
	background->setPosition(screenSize.width * 0.5f, screenSize.height * 0.5f);

	auto companyName = Label::createWithTTF("Eco Fruit Market",  "fonts/Marker Felt.ttf", screenSize.height * 0.067f);
	addChild(companyName);
	companyName->setColor(Color3B(0x04, 0xCF, 0x92));
	companyName->setAnchorPoint(Vec2(0.5f, 1.0f));
	companyName->setPosition(screenSize.width * 0.5f, screenSize.height * 0.9f);
	
	auto gameTitle = Label::createWithTTF("Healthy Fun Time", "fonts/Marker Felt.ttf", screenSize.height * 0.067f);
	addChild(gameTitle);
	gameTitle->setColor(Color3B(0x70, 0x0C, 0xB3));
	gameTitle->setAnchorPoint(Vec2(0.5f, 1.0f));
	gameTitle->setPosition(screenSize.width * 0.5f, screenSize.height * 0.82f);

	auto companyLogo = Sprite::create("extended-logo.png");
	addChild(companyLogo);
	companyLogo->setAnchorPoint(Vec2(0.5f, 1.0f));
	companyLogo->setPosition(screenSize.width * 0.5f, screenSize.height * 0.7f);

	auto playLabel = Label::createWithTTF("New Game", "fonts/Marker Felt.ttf", screenSize.height * 0.067f, Size::ZERO, TextHAlignment::CENTER);
	auto playItem = MenuItemLabel::create(playLabel, CC_CALLBACK_1(MainMenuScene::play, this));
	playItem->setColor(Color3B(0x00, 0x00, 0x00));

	auto couponLabel = Label::createWithTTF("My Coupons", "fonts/Marker Felt.ttf", screenSize.height * 0.067f, Size::ZERO, TextHAlignment::CENTER);
	auto couponItem = MenuItemLabel::create(couponLabel, CC_CALLBACK_1(MainMenuScene::coupons, this));
	couponItem->setColor(Color3B(0x56, 0x4F, 0x57));

	auto socialSprite = Sprite::create("social.png");
	auto socialPressedSprite = Sprite::create("social-pressed.png");
	auto socialItem = MenuItemSprite::create(socialSprite, socialPressedSprite, CC_CALLBACK_1(MainMenuScene::social, this));
	
	auto menu = Menu::create(playItem, couponItem, socialItem, nullptr);
	addChild(menu);
	menu->setAnchorPoint(Vec2(0.5f, 1.0f));
	menu->setPosition(screenSize.width * 0.5f, screenSize.height * 0.28f);
	menu->alignItemsVerticallyWithPadding(screenSize.height * 0.06f);
	
	setKeypadEnabled(true);

	return true;
}

void MainMenuScene::play(Ref* sender) {
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void MainMenuScene::coupons(Ref* sender) {
	auto scene = CouponListScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void MainMenuScene::social(Ref* sender) {
#ifdef _WIN32
	ShellExecuteA(nullptr, "open", "http://www.facebook.com", nullptr, nullptr, SW_SHOWNORMAL);
#else
	/*JniMethodInfo minfo;

	if (JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "openURL", "(Ljava/lang/String;)V"))	{
		jstring StringArg1 = minfo.env->NewStringUTF("http://www.facebook.com");
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, StringArg1);
		minfo.env->DeleteLocalRef(StringArg1);
		minfo.env->DeleteLocalRef(minfo.classID);
	}*/
#endif
}

void MainMenuScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		Director::getInstance()->end();
		exit(0);
    }
}

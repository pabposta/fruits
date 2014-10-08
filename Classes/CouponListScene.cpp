#include "CouponListScene.h"
#include "MainMenuScene.h"
#include "Coupon.h"
#include "CouponScene.h"

USING_NS_CC;

Scene* CouponListScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CouponListScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool CouponListScene::init() {

	if (!Layer::init()) {
        return false;
    }

	auto screenSize = Director::getInstance()->getWinSize();

	auto background = Sprite::create("menu-background.png");
	addChild(background);
	background->setPosition(screenSize.width * 0.5f, screenSize.height * 0.5f);

	// my coupons label
	auto myCouponsLabel = Label::createWithTTF("My Coupons",  "fonts/Marker Felt.ttf", screenSize.height * 0.067f);
	addChild(myCouponsLabel);
	myCouponsLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
	myCouponsLabel->setPosition(screenSize.width * 0.05f, screenSize.height * 0.95f);
	myCouponsLabel->setColor(Color3B(0x04, 0xCF, 0x92));

	// logo
	auto logo = Sprite::create("logo.png");
	addChild(logo);
	logo->setAnchorPoint(Vec2(1.0f, 1.0f));
	logo->setPosition(screenSize.width * 0.95f, screenSize.height * 0.95f);

	// back button
	auto backLabel = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", screenSize.height * 0.033f);
	auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(CouponListScene::back, this));
	backItem->setColor(Color3B(0x00, 0x00, 0x00));
	backItem->setAnchorPoint(Vec2(0.0f, 0.0f));
	auto backMenu = Menu::create(backItem, nullptr);
	addChild(backMenu);
	backMenu->setAnchorPoint(Vec2(0.0f, 1.0f));
	backMenu->setPosition(screenSize.width * 0.05f, screenSize.height * 0.8f);

	// show used label and menu
	auto showUsedLabel = Label::createWithTTF("Show used?", "fonts/Marker Felt.ttf", screenSize.height * 0.033f);
	addChild(showUsedLabel);
	showUsedLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
	showUsedLabel->setPosition(screenSize.width * 0.05f, screenSize.height * 0.7f);
	showUsedLabel->setColor(Color3B(0x70, 0x0C, 0xB3));
	auto onLabel = Label::createWithTTF("On", "fonts/Marker Felt.ttf", screenSize.height * 0.033f, Size::ZERO, TextHAlignment::CENTER);
	_onButton = MenuItemLabel::create(onLabel, CC_CALLBACK_1(CouponListScene::showUsed, this));
	_onButton->setColor(Color3B(0x56, 0x4F, 0x57));
	_onButton->setDisabledColor(Color3B(0x04, 0xCF, 0x92));
	_onButton->setAnchorPoint(Vec2(0.5f, 1.0f));
	_onButton->setEnabled(false); // we start with "show used?" enabled, so disable on button
	auto offLabel = Label::createWithTTF("Off", "fonts/Marker Felt.ttf", screenSize.height * 0.033f, Size::ZERO, TextHAlignment::CENTER);
	_offButton = MenuItemLabel::create(offLabel, CC_CALLBACK_1(CouponListScene::hideUsed, this));
	_offButton->setColor(Color3B(0x56, 0x4F, 0x57));
	_offButton->setDisabledColor(Color3B(0x04, 0xCF, 0x92));
	_offButton->setAnchorPoint(Vec2(0.5f, 1.0f));
	auto onOffMenu = Menu::create(_onButton, _offButton, nullptr);
	addChild(onOffMenu);
	onOffMenu->setAnchorPoint(Vec2(0.0f, 1.0f));
	onOffMenu->setPosition(screenSize.width * 0.33f, screenSize.height * 0.70f);
	onOffMenu->alignItemsHorizontally();
	
	// list of coupons
	createCouponList();

	setKeypadEnabled(true);

	return true;
}

void CouponListScene::back(Ref* sender) {
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void CouponListScene::showUsed(Ref* sender) {
	// switch buttons
	_onButton->setEnabled(false);
	_offButton->setEnabled(true);
	// iterate over used coupons and show them
	for (auto coupon : _usedCoupons) {
		coupon->setVisible(true);
	}
}

void CouponListScene::hideUsed(Ref* sender) {
	// switch buttons
	_onButton->setEnabled(true);
	_offButton->setEnabled(false);
	// iterate over used coupons and hide them
	for (auto coupon : _usedCoupons) {
		coupon->setVisible(false);
	}
}

void CouponListScene::createCouponList() {
	// first, read values from user data
	auto typeOne = UserDefault::getInstance()->getIntegerForKey(Coupon::TYPE_ONE.c_str());
	auto typeOneUsed = UserDefault::getInstance()->getIntegerForKey(Coupon::TYPE_ONE_USED.c_str());
	auto typeTwo = UserDefault::getInstance()->getIntegerForKey(Coupon::TYPE_TWO.c_str());
	auto typeTwoUsed = UserDefault::getInstance()->getIntegerForKey(Coupon::TYPE_TWO_USED.c_str());
	
	auto screenSize = Director::getInstance()->getWinSize();

	Vector<MenuItem*> coupons;

	// create the unused ones first
	for (auto i = typeOneUsed; i < typeOne; i++) {
		auto couponItem = createCouponMenuItem(
			screenSize, Coupon::TYPE_ONE_TEXT, false, CC_CALLBACK_1(CouponListScene::unusedCouponOne, this)
		);
		coupons.pushBack(couponItem);
	}
	for (auto i = typeTwoUsed; i < typeTwo; i++) {
		auto couponItem = createCouponMenuItem(
			screenSize, Coupon::TYPE_TWO_TEXT, false, CC_CALLBACK_1(CouponListScene::unusedCouponTwo, this)
		);
		coupons.pushBack(couponItem);
	}

	// then the used ones
	for (auto i = 0; i < typeOneUsed; i++) {
		auto couponItem = createCouponMenuItem(
			screenSize, Coupon::TYPE_ONE_TEXT, true, CC_CALLBACK_1(CouponListScene::usedCouponOne, this)
		);
		coupons.pushBack(couponItem);
		_usedCoupons.pushBack(couponItem);
	}
	for (auto i = 0; i < typeTwoUsed; i++) {
		auto couponItem = createCouponMenuItem(
			screenSize, Coupon::TYPE_TWO_TEXT, true, CC_CALLBACK_1(CouponListScene::usedCouponTwo, this)
		);
		coupons.pushBack(couponItem);
		_usedCoupons.pushBack(couponItem);
	}

	// now create the menu
	auto menu = Menu::createWithArray(coupons);
	addChild(menu);
	menu->setAnchorPoint(Vec2(0.5f, 1.0f));
	menu->setPosition(screenSize.width * 0.5f, screenSize.height * 0.4f);
	menu->alignItemsVerticallyWithPadding(screenSize.height * 0.05f);
}

MenuItem* CouponListScene::createCouponMenuItem(Size& screenSize, const std::string& text, bool used, const ccMenuCallback& callBack) {
	auto couponLabel = Label::createWithTTF(text, "fonts/Marker Felt.ttf", screenSize.height * 0.033f);
	auto couponItem = MenuItemLabel::create(couponLabel, callBack);
	if (used) {
		couponItem->setColor(Color3B(0x56, 0x4F, 0x57));
	}
	else {
		couponItem->setColor(Color3B(0x04, 0xCF, 0x92));
	}
	return couponItem;
}

void CouponListScene::unusedCouponOne(Ref* sender) {
	openCoupon(false, Coupon::TYPE_ONE);
}

void CouponListScene::usedCouponOne(Ref* sender) {
	openCoupon(true, Coupon::TYPE_ONE);
}

void CouponListScene::unusedCouponTwo(Ref* sender) {
	openCoupon(false, Coupon::TYPE_TWO);
}

void CouponListScene::usedCouponTwo(Ref* sender) {
	openCoupon(true, Coupon::TYPE_TWO);
}

void CouponListScene::openCoupon(bool used, const std::string& type) {
	auto scene = CouponScene::createScene(type, used);
	Director::getInstance()->replaceScene(scene);
}

void CouponListScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		back(this);
	}
}

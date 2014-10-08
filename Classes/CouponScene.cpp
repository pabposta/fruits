#include "CouponScene.h"
#include "CouponListScene.h"
#include "Coupon.h"

USING_NS_CC;

Scene* CouponScene::createScene(const std::string& type, bool used)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CouponScene::create(type, used);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

CouponScene* CouponScene::create(const std::string& type, bool used) {
	CouponScene* couponScene = new CouponScene();
	if (couponScene && couponScene->init(type, used)) {
		couponScene->autorelease();
		return couponScene;
	}
	CC_SAFE_DELETE(couponScene);
	return nullptr;
}

bool CouponScene::init(const std::string& type, bool used) {
	if (!Layer::init()) {
        return false;
    }

	_type = type;

	auto screenSize = Director::getInstance()->getWinSize();

	// coupon label
	auto couponLabel = Label::createWithTTF("Coupon", "fonts/Marker Felt.ttf", screenSize.height * 0.067f);
	addChild(couponLabel);
	couponLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
	couponLabel->setPosition(screenSize.width * 0.05f, screenSize.height * 0.95f);
	couponLabel->setColor(Color3B(0x04, 0xCF, 0x92));

	// logo
	auto logo = Sprite::create("logo.png");
	addChild(logo);
	logo->setAnchorPoint(Vec2(1.0f, 1.0f));
	logo->setPosition(screenSize.width * 0.95f, screenSize.height * 0.95f);

	// back button
	auto backLabel = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", screenSize.height * 0.033f);
	auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(CouponScene::back, this));
	backItem->setColor(Color3B(0x00, 0x00, 0x00));
	backItem->setAnchorPoint(Vec2(0.0f, 0.0f));
	auto backMenu = Menu::create(backItem, nullptr);
	addChild(backMenu);
	backMenu->setAnchorPoint(Vec2(0.0f, 1.0f));
	backMenu->setPosition(screenSize.width * 0.05f, screenSize.height * 0.8f);

	// coupon text
	auto text = type.compare(Coupon::TYPE_ONE) == 0 ? Coupon::TYPE_ONE_TEXT : Coupon::TYPE_TWO_TEXT;
	auto textLabel = Label::createWithTTF(text, "fonts/Marker Felt.ttf", screenSize.height * 0.033f);
	addChild(textLabel);
	textLabel->setColor(Color3B(0x56, 0x4F, 0x57));
	textLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
	textLabel->setPosition(screenSize.width * 0.5f, screenSize.height * 0.7f);

	// check
	auto checkSprite = Sprite::create("check-empty.png");
	auto checkSpritePressed = Sprite::create("check-green-pressed.png");
	auto checkSpriteUsed = Sprite::create("check-green.png");
	auto checkItem = MenuItemSprite::create(checkSprite, checkSpritePressed, checkSpriteUsed, CC_CALLBACK_1(CouponScene::check, this));
	auto checkMenu = Menu::create(checkItem, nullptr);
	addChild(checkMenu);
	checkMenu->setAnchorPoint(Vec2(0.5f, 0.5f));
	checkMenu->setPosition(screenSize.width * 0.5f, screenSize.height * 0.47f);
	if (used) {
		checkItem->setEnabled(false);
	}

	// bar code
	auto barcodeSprite = Sprite::create("barcode.png");
	addChild(barcodeSprite);
	barcodeSprite->setAnchorPoint(Vec2(0.5f, 1.0f));
	barcodeSprite->setPosition(screenSize.width * 0.5f, screenSize.height * 0.3f);

	setKeypadEnabled(true);

	return true;
}

void CouponScene::back(Ref* sender) {
	auto scene = CouponListScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void CouponScene::check(Ref* sender) {
	// disable and mark the check button
	((MenuItemSprite*) sender)->setEnabled(false);
	
	// save the new info
	auto key = _type.compare(Coupon::TYPE_ONE) == 0 ? Coupon::TYPE_ONE_USED.c_str() : Coupon::TYPE_TWO_USED.c_str();
	auto used = UserDefault::getInstance()->getIntegerForKey(key);
	used++;
	UserDefault::getInstance()->setIntegerForKey(key, used);	
}

void CouponScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		back(this);
	}
}

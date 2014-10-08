#ifndef __COUPON_LIST_SCENE_H__
#define __COUPON_LIST_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class CouponListScene : public Layer {

public:

	static cocos2d::Scene* createScene();

	bool init();

	CREATE_FUNC(CouponListScene);

	void back(Ref* sender);

	void showUsed(Ref* sender);
	void hideUsed(Ref* sender);

private:
	MenuItemLabel* _onButton;
	MenuItemLabel* _offButton;
	Vector<MenuItem*> _usedCoupons;

	void createCouponList();

	void unusedCouponOne(Ref* sender);
	void usedCouponOne(Ref* sender);
	void unusedCouponTwo(Ref* sender);
	void usedCouponTwo(Ref* sender);
	void openCoupon(bool used, const std::string& type);

	MenuItem* createCouponMenuItem(
		Size& screenSize, const std::string& text, bool used, const ccMenuCallback& callBack
	);

	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif

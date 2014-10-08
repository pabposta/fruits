#ifndef __COUPON_SCENE_H__
#define __COUPON_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class CouponScene : public Layer {

public:

	static Scene* createScene(const std::string& type, bool used);
	static CouponScene* create(const std::string& type, bool used);

	bool init(const std::string& type, bool used);

private:

	std::string _type;

	void back(Ref* sender);
	void check(Ref* sender);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif
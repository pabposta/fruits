#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class MainMenuScene : public Layer {
	
public:

	static cocos2d::Scene* createScene();

	bool init();

	CREATE_FUNC(MainMenuScene);

	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

private:

	void play(Ref* sender);
	void coupons(Ref* sender);
	void social(Ref* sender);
	
};

#endif

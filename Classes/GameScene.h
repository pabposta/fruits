#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "FallingObject.h"
#include "Fruit.h"
#include "Junk.h"

using namespace cocos2d;

class Player;

class GameScene : public Layer {

public:

	~GameScene();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

	// input handlers
	bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void objectHitGround(FallingObject* object);

	void update(float dt) override;
	void gameOver(EventCustom* event);
	void pauseGame();
	void newCoupon(EventCustom* event);
	static float getFloorHeight();

private:

	static const float SPEED;
	static const int MAX_FALLING_OBJECTS;
	static const float DROP_INTERVAL;
	static const float JUNK_PROBABILITY;
	static const int COUPON_INTERVAL;
	static const std::string FRUIT_FILES[];
	static const std::string JUNK_FILES[];
	static const float FLOOR_HEIGHT_FACTOR;
	static float _floorHeight;
	
	Player* _player;
	Vector<FallingObject*> _fallingObjects;
	Vector<FallingObject*> _fruitPool;
	Vector<FallingObject*> _junkPool;
	Menu* _menu;
	Label* _pauseLabel;
	Label* _gameOverLabel;
	Sprite* _pauseButton;
	bool _playing;
	bool _dead;
	Vector<Node*> _pausedTargets;
	Label* _newCouponLabel;
	Action* _newCouponAction;

	float _dropTimer;

	FallingObject* getFromPool(Vector<FallingObject*>& pool);
	void returnToPool(FallingObject* object);

	void reset();

	void play(Ref* sender);
	void mainMenu(Ref* sender);
	void turnAudioOn(Ref* sender);
	void turnAudioOff(Ref* sender);
};

#endif // __HELLOWORLD_SCENE_H__

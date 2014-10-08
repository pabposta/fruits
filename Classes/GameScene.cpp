#include "GameScene.h"
#include "Player.h"
#include "FallingObject.h"
#include "Fruit.h"
#include "Junk.h"
#include "Score.h"
#include "Energy.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

const float GameScene::SPEED = 6.0f; // lower values are faster
const int GameScene::MAX_FALLING_OBJECTS = 12;
const float GameScene::DROP_INTERVAL = 0.5f;
const float GameScene::JUNK_PROBABILITY = 0.3f;
const int GameScene::COUPON_INTERVAL = 1000;
const std::string GameScene::FRUIT_FILES[] = { "apple.png", "banana.png", "cherries.png", "grapes.png", "lemon.png", "prune.png" };
const std::string GameScene::JUNK_FILES[] = { "burger.png", "chicken.png", "donut.png", "lollipop.png" };
const float GameScene::FLOOR_HEIGHT_FACTOR = 0.075f;
float GameScene::_floorHeight = GameScene::FLOOR_HEIGHT_FACTOR;

GameScene::~GameScene() {
	_newCouponAction->release();
	_eventDispatcher->removeCustomEventListeners(Fruit::FRUIT_HIT_GROUND);
	_eventDispatcher->removeCustomEventListeners(Energy::ENERGY_LEVEL_ZERO);
	_eventDispatcher->removeCustomEventListeners(Score::NEW_COUPON);
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto screenSize = Director::getInstance()->getWinSize();
	srand(time(nullptr));

	// background init
	auto background = Sprite::create("game-background.png");
	addChild(background);
	background->setPosition(screenSize.width * 0.5f, screenSize.height * 0.5f);

	// floor init
	auto floor = Sprite::create("floor.png");
	addChild(floor, 2);
	floor->setAnchorPoint(Vec2(0.0f, 0.0f));
	floor->setPosition(0.0f, 0.0f);
	_floorHeight = screenSize.height * FLOOR_HEIGHT_FACTOR;

	// pause button
	_pauseButton = Sprite::create("pause.png");
	_pauseButton->setAnchorPoint(Vec2(0.0f, 1.0f));
	_pauseButton->setPosition(screenSize.width * 0.02f, screenSize.height * 0.98f);
	addChild(_pauseButton, 1);

	// sound on off buttons
	/*auto audioOnSprite = Sprite::create("audio-on.png");
	auto audioOnPressedSprite = Sprite::create("audio-off.png");
	auto audioOffSprite = Sprite::create("audio-off.png");
	auto audioOffPressedSprite = Sprite::create("audio-on.png");
	auto audioOnItem = MenuItemSprite::create(audioOnSprite, audioOnPressedSprite, CC_CALLBACK_1(GameScene::turnAudioOff, this));
	auto audioOffItem = MenuItemSprite::create(audioOffSprite, audioOffPressedSprite, CC_CALLBACK_1(GameScene::turnAudioOn, this));
	auto audioToggleItem = MenuItemToggle::create();
	audioToggleItem->addSubItem(audioOnItem);
	audioToggleItem->addSubItem(audioOffItem);
	auto audioMenu = Menu::create(audioOnItem, nullptr);
	addChild(audioMenu, 1);
	audioMenu->setAnchorPoint(Vec2(0.0f, 1.0f));
	audioMenu->setPosition(screenSize.width * 0.02f, screenSize.height * 0.9f);
	*/

	// energy display init
	auto energy = Energy::create("health.png", "fonts/Marker Felt.ttf", screenSize.height * 0.067f);
	energy->setAnchorPoint(Vec2(0.0f, 1.0f));
	energy->setPosition(Vec2(_pauseButton->getBoundingBox().size.width + screenSize.width * 0.04f, screenSize.height * 0.98f));
	energy->reset();
	addChild(energy, 1);

	// score display init
	auto score = Score::create("fonts/Marker Felt.ttf", screenSize.height * 0.067f, COUPON_INTERVAL);
	score->setAnchorPoint(Vec2(1.0f, 1.0f));
	score->setPosition(Vec2(screenSize.width * 0.98f, screenSize.height * 0.98f));
	score->reset();
	addChild(score, 1);

	// player init
    _player = Player::create("player.png", score, energy);
	_player->setAnchorPoint(Vec2(0.5f, 1.0f));
	_player->moveTo(Vec2(screenSize.width * 0.5f, _player->getHeight() + _floorHeight));
	addChild(_player, 1);

	// fruit pool init
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 6; i++) {
			auto fruit = Fruit::create(FRUIT_FILES[i], this);
			fruit->setAnchorPoint(Vec2(0.5f, 0.0f));
			fruit->setVisible(false);
			_fruitPool.pushBack(fruit);
			addChild(fruit);
		}
	}

	// junk pool init
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 4; i++) {
			auto junk = Junk::create(JUNK_FILES[i], this);
			junk->setAnchorPoint(Vec2(0.5f, 0.0f));
			junk->setVisible(false);
			_junkPool.pushBack(junk);
			addChild(junk);
		}
	}

	// game over and pause menu
	auto labelPlay = Label::createWithTTF("Play", "fonts/Marker Felt.ttf", screenSize.height * 0.067f, Size::ZERO, TextHAlignment::CENTER);
	auto playItem = MenuItemLabel::create(labelPlay, CC_CALLBACK_1(GameScene::play, this));
	playItem->setColor(Color3B(0xFF, 0xFF, 0xFF));
	
	auto labelMainMenu = Label::createWithTTF("Main Menu", "fonts/Marker Felt.ttf", screenSize.height * 0.067f, Size::ZERO, TextHAlignment::CENTER);
	auto mainMenuItem = MenuItemLabel::create(labelMainMenu, CC_CALLBACK_1(GameScene::mainMenu, this));
	mainMenuItem->setColor(Color3B(0xFF, 0xFF, 0xFF));
		
	_menu = Menu::create(playItem, mainMenuItem, nullptr);
	addChild(_menu, 1);
	_menu->setVisible(false);
	_menu->setAnchorPoint(Vec2(0.5f, 1.0f));
	_menu->setPosition(screenSize.width * 0.5f, screenSize.height * 0.5f);
	_menu->alignItemsVertically();

	// game over and pause labels
	_pauseLabel = Label::createWithTTF("Pause", "fonts/Marker Felt.ttf", screenSize.height * 0.067f);
	addChild(_pauseLabel, 1);
	_pauseLabel->setColor(Color3B(0, 0, 0));
	_pauseLabel->setAnchorPoint(Vec2(0.5f, 0.0f));
	_pauseLabel->setPosition(screenSize.width * 0.5f, screenSize.height * 0.6f);
	_pauseLabel->setVisible(false);

	_gameOverLabel = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", screenSize.height * 0.067f);
	addChild(_gameOverLabel, 1);
	_gameOverLabel->setColor(Color3B(0, 0, 0));
	_gameOverLabel->setAnchorPoint(Vec2(0.5f, 0.0f));
	_gameOverLabel->setPosition(screenSize.width * 0.5f, screenSize.height * 0.6f);
	_gameOverLabel->setVisible(false);

	// new coupon label
	_newCouponLabel = Label::createWithTTF("New Coupon", "fonts/Marker Felt.ttf", screenSize.height * 0.067f);
	addChild(_newCouponLabel, 1);
	_newCouponLabel->setColor(Color3B(0x70, 0x0C, 0xB3));
	_newCouponLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	_newCouponLabel->setPosition(screenSize.width * 0.5f, screenSize.height * 0.5f);
	_newCouponLabel->setVisible(false);
	// fade action for new coupon label
	auto zoomInAction = ScaleBy::create(1.0f, 1.5f);
	auto fadeOutAction = FadeOut::create(1.0f);
	_newCouponAction = Spawn::createWithTwoActions(zoomInAction, fadeOutAction);
	_newCouponAction->retain();

	// register Touch Events
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	setKeypadEnabled(true);
	
	// listen to the player dying
	_eventDispatcher->addCustomEventListener(Energy::ENERGY_LEVEL_ZERO, CC_CALLBACK_1(GameScene::gameOver, this));

	// listen to new coupon
	_eventDispatcher->addCustomEventListener(Score::NEW_COUPON, CC_CALLBACK_1(GameScene::newCoupon, this));

	// init timer to immediately drop a fruit
	_dropTimer = DROP_INTERVAL;

	// state variables
	_playing = true;
	_dead = false;

	//create main loop
    this->schedule(schedule_selector(GameScene::update));

	// play music
	SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);

    return true;
}

// new touches and moves are treated the same way
bool GameScene::onTouchBegan(Touch* touch, Event* event) {
	if (touch) {
		Point tap = touch->getLocation();
		if (_pauseButton->getBoundingBox().containsPoint(tap)) {
			if (_playing) {
				pauseGame();
			}
			else {
				play(this);
			}
		}
	}
	return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event) {
	if (_playing) {
		_player->moveTo(Vec2(touch->getLocation().x, _player->getHeight() + _floorHeight));
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		stopAllActions();
		mainMenu(this);
    }
	else if (keyCode == EventKeyboard::KeyCode::KEY_P || keyCode == EventKeyboard::KeyCode::KEY_PAUSE) {
		if (_playing) {
			pauseGame();
		}
		else {
			play(this);
		}
	}
}

FallingObject* GameScene::getFromPool(Vector<FallingObject*>& pool) {
	auto object = pool.at(0);
	pool.erase(0);
	return object;
}

void GameScene::returnToPool(FallingObject* object) {
	if (object->getObjectType() == FallingObject::FRUIT) {
		_fruitPool.pushBack((Fruit*) object);
	}
	else {
		_junkPool.pushBack((Junk*) object);
	}
}

void GameScene::objectHitGround(FallingObject* object) {
	object->hitGround();
	_fallingObjects.eraseObject(object, false);
	returnToPool(object);
}

void GameScene::update(float dt) {
	// only do something if a game is active
	if (!_playing) {
		return;
	}

	// check if new fruits need to be created. this is done when the timer elapses
	if (_dropTimer >= DROP_INTERVAL) {
		if (_fallingObjects.size() < MAX_FALLING_OBJECTS) {
			// decide whether to drop fruit or junk
			FallingObject* object;
			if (rand() < RAND_MAX * JUNK_PROBABILITY) {
				// add junk. get it from pool
				object = getFromPool(_junkPool);
			}
			else {
				// get fruit from pool
				object = getFromPool(_fruitPool);
			}
			// reset it to the starting state
			object->reset(this, SPEED);
			// add it to list of falling objects
			_fallingObjects.pushBack(object);
			// reset the timer
			_dropTimer = 0.0f;
		}
	}
	else {
		// increase the timer
		_dropTimer += dt;
	}

	// collision detection with player
	auto it = _fallingObjects.begin();
	auto collisionHeight = _player->getHeight() * 0.9f + _floorHeight;
	while (it != _fallingObjects.end()) {
		if ((*it)->getPositionY() <= collisionHeight) {
			float objectX = (*it)->getPositionX();
			float playerX = _player->getPositionX();
			float playerHalfWidth = _player->getHalfWidth();
			if (objectX >= playerX - playerHalfWidth && objectX <= playerX + playerHalfWidth) {
				// we have a collision
				(*it)->collideWithPlayer(_player);
				returnToPool(*it);
				it = _fallingObjects.erase(it);
				continue;
			}
		}
		++it;
	}
}

void GameScene::gameOver(EventCustom* event) {
	_gameOverLabel->setVisible(true);
	_menu->setVisible(true);
	_playing = false;
	_dead = true;
	_pausedTargets = _actionManager->pauseAllRunningActions();
}

void GameScene::play(Ref* sender) {
	if (_dead) {
		reset();
		_gameOverLabel->setVisible(false);
	}
	else {
		_pauseLabel->setVisible(false);
		_actionManager->resumeTargets(_pausedTargets);
	}
	_menu->setVisible(false);
	_playing = true;	
}

void GameScene::mainMenu(Ref* sender) {
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void GameScene::reset() {
	_player->getScore()->reset();
	_player->getEnergy()->reset();
	auto screenSize = Director::getInstance()->getWinSize();
	_player->moveTo(Vec2(screenSize.width * 0.5f, _player->getHeight() + _floorHeight));
	// empty falling objects vector and return items to pool
	while (!_fallingObjects.empty()) {
		auto object = _fallingObjects.back();
		returnToPool(object);
		_fallingObjects.popBack();
	}
	// tell all objects to prepare for a new round
	for (auto object : _fruitPool) {
		object->prepareForNewRound();
	}
	for (auto object : _junkPool) {
		object->prepareForNewRound();
	}
	_dead = false;
}

void GameScene::pauseGame() {
	_pauseLabel->setVisible(true);
	_menu->setVisible(true);
	_playing = false;
	_pausedTargets = _actionManager->pauseAllRunningActions();
}

void GameScene::newCoupon(EventCustom* event) {
	// animate new coupon label. but first, reset it
	_newCouponLabel->stopAllActions();
	_newCouponLabel->setScale(1.0f);
	_newCouponLabel->setOpacity(255);
	_newCouponLabel->setVisible(true);
	_newCouponLabel->runAction(_newCouponAction);
	// play sound
	SimpleAudioEngine::getInstance()->playEffect("coupon.wav");
}

void GameScene::turnAudioOn(Ref* sender) {
	SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
	SimpleAudioEngine::getInstance()->resumeAllEffects();
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	CCLOG("audio on");
}

void GameScene::turnAudioOff(Ref* sender) {
	SimpleAudioEngine::getInstance()->stopAllEffects();
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
	CCLOG("audio off");
}

float GameScene::getFloorHeight() {
	return _floorHeight;
}

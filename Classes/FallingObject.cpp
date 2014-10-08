#include "FallingObject.h"
#include "GameScene.h"

USING_NS_CC;

FallingObject::~FallingObject() {
	_action->release();
}

bool FallingObject::init(Layer* parent, ParticleSystem* system, ObjectType type) {
	setObjectType(type);
	
	_system = system;
	parent->addChild(_system);
	_system->setAnchorPoint(getAnchorPoint());
	_system->stopSystem();
	auto fadeOut = FadeOut::create(1.0f);
	_action = Sequence::create(
		fadeOut,
		CallFunc::create([=] () {
			_system->stopSystem();
			_system->setVisible(false);
		}),
		nullptr
	);
	_action->retain();

	return true;
}

void FallingObject::reset(GameScene* parent, float speed) {
	// stop everything the object was doing
	stopAllActions();

	// choose a random x coordinate for the falling object. it will fall straight down, from the top of the screen to the bottom (i.e. the floor)
	auto screenSize = Director::getInstance()->getWinSize();
	float x = rand() % (int) (screenSize.width * 0.9f) + screenSize.width * 0.05f;
	setPosition(x, screenSize.height);
	_speed = speed;

	// create the falling action
	FiniteTimeAction* sequence = Sequence::create(
		MoveTo::create(_speed, Vec2(x, GameScene::getFloorHeight())),
		CallFunc::create(CC_CALLBACK_0(GameScene::objectHitGround, parent, this)),
		nullptr
	);
	runAction(sequence);

	// make the object visible again
	setVisible(true);
}

void FallingObject::prepareForNewRound() {
	die();
	stopParticleSystem();
}

void FallingObject::hitGround() {
	die();
}

void FallingObject::die() {
	// stop everything the object was doing
	stopAllActions();
	// and make the object invisible
	setVisible(false);
}

void FallingObject::runParticleSystem() {
	_system->stopAllActions();
	_system->setPosition(getPosition());
	_system->setVisible(true);
	_system->setOpacity(255);
	_system->resetSystem();
	_system->runAction(_action);
}

void FallingObject::stopParticleSystem() {
	_system->stopAllActions();
	_system->setVisible(false);
	_system->stopSystem();
}

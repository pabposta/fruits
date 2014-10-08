#include "Fruit.h"
#include "Score.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

const std::string Fruit::FRUIT_HIT_GROUND = "fruitHitGround";

Fruit* Fruit::create(const std::string& filename, Layer* parent) {
	Fruit* fruit = new Fruit();
	if (fruit && fruit->initWithFile(filename) && fruit->init(parent)) {
		fruit->autorelease();
		return fruit;
	}
	CC_SAFE_DELETE(fruit);
	return nullptr;
}

bool Fruit::init(Layer* parent) {
	return FallingObject::init(parent, ParticleExplosion::create(), FRUIT);
}

void Fruit::collideWithPlayer(Player* player) {
	die();
	player->getScore()->addPoints(100);
}

void Fruit::hitGround() {
	runParticleSystem();
	SimpleAudioEngine::getInstance()->playEffect("fruit.wav");
	FallingObject::hitGround();
	_eventDispatcher->dispatchCustomEvent(FRUIT_HIT_GROUND);
}

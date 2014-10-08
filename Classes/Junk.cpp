#include "Junk.h"
#include "Energy.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Junk* Junk::create(const std::string& filename, Layer* parent) {
	Junk* junk = new Junk();
	if (junk && junk->initWithFile(filename) && junk->init(parent)) {
		junk->autorelease();
		return junk;
	}
	CC_SAFE_DELETE(junk);
	return nullptr;
}

bool Junk::init(Layer* parent) {
	auto system = ParticleFire::create();
	system->setStartColor(Color4F(0.8f, 0.6f, 0.0f, 0.5f));
	return FallingObject::init(parent, system, JUNK);
}

void Junk::collideWithPlayer(Player* player) {
	runParticleSystem();
	SimpleAudioEngine::getInstance()->playEffect("junk.wav");
	die();
	// decrease player energy
	player->getEnergy()->decrease(20);
}

#ifndef __FALLINGOBJECT_H__
#define __FALLINGOBJECT_H__

#include "cocos2d.h"
#include "Player.h"

using namespace cocos2d;

class GameScene;

class FallingObject: public Sprite {

public:

	enum ObjectType {
		FRUIT,
		JUNK
	};

	~FallingObject();

	virtual void collideWithPlayer(Player* player) = 0;

	virtual void hitGround();

	void reset(GameScene* parent, float speed);
	void die();

	CC_SYNTHESIZE(float, _speed, Speed);

	CC_SYNTHESIZE(ObjectType, _objectType, ObjectType);

	void prepareForNewRound();

protected:

	ParticleSystem* _system;
	Action* _action;

	bool init(Layer* parent, ParticleSystem* system, ObjectType type);
	void runParticleSystem();
	void stopParticleSystem();
};

#endif
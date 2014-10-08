#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

class Score;
class Energy;

class Player : public Sprite {

public:
	static Player* create(const std::string& filename, Score* score, Energy* energy);

	void moveTo(Vec2 destination);
	float getHeight(void);
	float getHalfWidth(void);
	Score* getScore();
	Energy* getEnergy();
	void fruitHitGround(EventCustom* event);

private:
	float _height;
	float _halfWidth;
	Score* _score;
	Energy* _energy;
};

#endif
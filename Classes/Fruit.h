#ifndef __FRUIT_H__
#define __FRUIT_H__

#include "FallingObject.h"

class Fruit: public FallingObject {

public:
	
	static const std::string FRUIT_HIT_GROUND;

	static Fruit* create(const std::string& filename, Layer* parent);

	void collideWithPlayer(Player* player) override;
	void hitGround() override;

private:

	bool init(Layer* parent);

};

#endif
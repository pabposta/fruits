#ifndef __JUNK_H__
#define __JUNK_H__

#include "FallingObject.h"

class Junk: public FallingObject {

public:
	
	static Junk* create(const std::string& filename, Layer* parent);

	void collideWithPlayer(Player* player) override;

private:
	
	bool init(Layer* parent);
};

#endif
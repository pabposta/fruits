#ifndef __ENERGY_H__
#define __ENERGY_H__

#include "cocos2d.h"

using namespace cocos2d;

class Energy: public Node {

public:
	static const std::string ENERGY_LEVEL_ZERO;

	static Energy* create(const std::string& filename, const std::string& font, float fontSize);

	void reset();
	void decrease(int percent);
	void refreshDisplay();

private:
	int _level;
	Sprite* _icon;
	Label* _levelLabel;

};

#endif
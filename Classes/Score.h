#ifndef __SCORE_H__
#define __SCORE_H__

#include "cocos2d.h"

using namespace cocos2d;

class Score: public Node {

public:
	static const std::string NEW_COUPON;

	static Score* create(const std::string& font, float fontSize, int interval);

	void reset();
	void addPoints(int points);
	void refreshDisplay();

private:
	int _points;
	int _interval;
	int _missing;
	Label* _pointLabel;
	Label* _missingLabel;
	
};

#endif

#include "Score.h"
#include "Coupon.h"

USING_NS_CC;

const std::string Score::NEW_COUPON = "NewCoupon";

Score* Score::create(const std::string& font, float fontSize, int interval) {
	Score* score = new Score();
	if (score) {
		score->_pointLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", fontSize);
		score->addChild(score->_pointLabel);
		score->_pointLabel->setColor(Color3B(0x70, 0x0C, 0xB3));
		score->_pointLabel->setAnchorPoint(Vec2(1.0f, 1.0f));
		score->_pointLabel->setPosition(Vec2());
				
		score->_missingLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", fontSize * 0.5f);
		score->addChild(score->_missingLabel);
		score->_missingLabel->setColor(Color3B(0x56, 0x4F, 0x57));
		score->_missingLabel->setAnchorPoint(Vec2(1.0f, 1.0f));
		score->_missingLabel->setPosition(Vec2(0.0f, -fontSize * 1.1f));

		score->_interval = interval;
		score->reset();
		score->autorelease();
		return score;
	}
	CC_SAFE_DELETE(score);
	return nullptr;
}

void Score::reset() {
	_points = 0;
	_missing = _interval;
	refreshDisplay();
}

void Score::addPoints(int points) {
	_points += points;
	_missing -= points;
	while (_missing <= 0) {
		_missing += _interval;
		_eventDispatcher->dispatchCustomEvent(NEW_COUPON);
	}
	refreshDisplay();
}

void Score::refreshDisplay() {
	auto value = String::createWithFormat("%i", _points);
	_pointLabel->setString(value->getCString());
	value = String::createWithFormat("Next coupon: %i", _missing);
	_missingLabel->setString(value->getCString());
}

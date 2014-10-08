#include "Energy.h"

USING_NS_CC;

const std::string Energy::ENERGY_LEVEL_ZERO = "EnergyLevelZero";

Energy* Energy::create(const std::string& filename, const std::string& font, float fontSize) {
	Energy* energy = new Energy();
	if (energy) {
		energy->_icon = Sprite::create(filename);
		energy->addChild(energy->_icon);
		energy->_icon->setAnchorPoint(Vec2(0.0f, 1.0f));
		energy->_icon->setPosition(Vec2());
				
		energy->_levelLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", fontSize);
		energy->addChild(energy->_levelLabel);
		energy->_levelLabel->setColor(Color3B(0xDD, 0x88, 0x00));
		energy->_levelLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
		energy->_levelLabel->setPosition(Vec2(energy->_icon->getBoundingBox().size.width + Director::getInstance()->getWinSize().width * 0.02f, 0.0f));

		energy->reset();
		energy->autorelease();
		return energy;
	}
	CC_SAFE_DELETE(energy);
	return nullptr;
}

void Energy::reset() {
	_level = 100;
	refreshDisplay();
}

void Energy::decrease(int percent) {
	_level -= percent;
	if (_level <= 0) {
		_eventDispatcher->dispatchCustomEvent(ENERGY_LEVEL_ZERO);
		_level = 0;
	}
	refreshDisplay();
}

void Energy::refreshDisplay() {
	auto value = String::createWithFormat("%i", _level);
	_levelLabel->setString(value->getCString());
}

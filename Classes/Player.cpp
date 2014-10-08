#include "Player.h"
#include "Score.h"
#include "Energy.h"
#include "Fruit.h"

USING_NS_CC;

Player* Player::create(const std::string& filename, Score* score, Energy* energy) {
	Player* player = new Player();
	if (player && player->initWithFile(filename)) {
		player->_height = player->getBoundingBox().size.height;
		player->_halfWidth = player->getBoundingBox().size.width * 0.5f;
		player->_score = score;
		player->_energy = energy;
		player->autorelease();

		auto listener = EventListenerCustom::create(Fruit::FRUIT_HIT_GROUND, CC_CALLBACK_1(Player::fruitHitGround, player));
		Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

void Player::moveTo(Vec2 destination) {
	setPosition(destination);
}

float Player::getHeight(void) {
	return _height;
}

float Player::getHalfWidth(void) {
	return _halfWidth;
}

Score* Player::getScore() {
	return _score;
}

Energy* Player::getEnergy() {
	return _energy;
}

void Player::fruitHitGround(EventCustom* event) {
	_energy->decrease(10);
}

#include "SlotScene.h"
#include "SimpleAudioEngine.h"
#include "VisibleRect.h"

USING_NS_CC;

Scene* SlotScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SlotScene::create();
    scene->addChild(layer);
    return scene;
}

bool SlotScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	_zOrder = 0;
	_status = SLOTS_STATUS::STOPED;

	widthScale = Director::getInstance()->getVisibleSize().width / DEFAULT_WIDTH;
	heightScale = Director::getInstance()->getVisibleSize().height / DEFAULT_HEIGHT;

	initCenterBackground();

	initSlotMachine();

	initUpBottomBackground();

	initButton();

    return true;
}
void SlotScene::initCenterBackground()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* bg_center = Sprite::create("imeges/background/bg_center.png");
	bg_center->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + (27 * heightScale)));
	bg_center->setScaleX(widthScale);
	bg_center->setScaleY(heightScale);
	this->addChild(bg_center, _zOrder++);
}
void SlotScene::initUpBottomBackground()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* bg_up = Sprite::create("imeges/background/bg_up.png");
	bg_up->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - ((bg_up->getContentSize().height * heightScale) / 2)));
	bg_up->setScaleX(widthScale);
	bg_up->setScaleY(heightScale);
	this->addChild(bg_up, _zOrder++);

	Sprite* bg_bottom = Sprite::create("imeges/background/bg_bottom.png");
	bg_bottom->setPosition(Vec2(visibleSize.width / 2, ((bg_bottom->getContentSize().height * heightScale) / 2)));
	bg_bottom->setScaleX(widthScale);
	bg_bottom->setScaleY(heightScale);
	this->addChild(bg_bottom, _zOrder++);
}
void SlotScene::initButton()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	_spinLabel = Label::createWithTTF("Spin", "fonts/Marker Felt.ttf", 46 * widthScale);
	auto spinButton = MenuItemLabel::create(_spinLabel, CC_CALLBACK_1(SlotScene::spinCallback, this));
	spinButton->setPosition(Vec2(visibleSize.width / 2, 70 * heightScale));

	auto menu = Menu::create(spinButton, NULL);
	menu->setPosition(Vec2(ZERO, ZERO));
	this->addChild(menu, _zOrder++);
}

int SlotScene::getNextZOrder()
{
	return _zOrder++;
}

void SlotScene::initSlotMachine()
{
	machine.push_back(std::vector<Sprite*>());
	machine.push_back(std::vector<Sprite*>());
	machine.push_back(std::vector<Sprite*>());
	machine.push_back(std::vector<Sprite*>());
	machine.push_back(std::vector<Sprite*>());
	for (int i = 0; i < machine.size(); i++)
	{
		initSlotBarByNumber(i + 1, machine[i]);
	}
}
void SlotScene::initSlotBarByNumber(int number, std::vector<Sprite*> &bar)
{
	for (int i = 0; i < 6; i++)
	{
		String* filename = String::createWithFormat("imeges/items/%d.png", i + 1);
		Sprite *item = Sprite::create(filename->getCString());
		item->setPosition(HORIZONTAL_START_POSITION * widthScale + (number * (HORIZONTAL_MARGIN * widthScale)),
			VERTICAL_START_POSITION * heightScale + (i * (VERTICAL_MARGIN * heightScale)));
		item->setTag(number * 10 + (i));
		item->setScaleX(widthScale);
		item->setScaleY(heightScale);
		this->addChild(item, this->getNextZOrder());
		bar.push_back(item);
	}
}

void SlotScene::setDefaulSlotMachinePosition()
{
	for (int i = 0; i < machine.size(); i++)
	{
		setDefaultSlotBarPosition(machine[i]);
	}
}
void SlotScene::setDefaultSlotBarPosition(std::vector<Sprite*> &bar)
{
	for (int i = 0; i < bar.size(); i++)
	{
		bar[i]->setPositionY(VERTICAL_START_POSITION * heightScale + (i * (VERTICAL_MARGIN * heightScale)));
	}
}

void SlotScene::runSlotMachine()
{
	for (int i = 0; i < machine.size(); i++)
	{
		runSlotBarByNumber(i + 1, machine[i]);
	}
}
void SlotScene::runSlotBarByNumber(int number, std::vector<Sprite*> &bar)
{
	for (int i = 0; i < bar.size(); i++)
	{
		float adv = VisibleRect::bottom().y;
		auto defaultMove =		MoveBy::create(DEFAULT_MOVE_DURATION,			Vec2(ZERO, ZERO - (VERTICAL_MARGIN * heightScale)));
		auto toDownAfterStart = MoveBy::create(BAR_DURATION / bar.size() * i,	Vec2(ZERO, ZERO - (i * (VERTICAL_MARGIN * heightScale))));
		auto toDownPos =		MoveBy::create(BAR_DURATION,					Vec2(ZERO, ZERO - (BAR_HEIGHT * heightScale)));
		auto toDefPos =			MoveBy::create(ZERO,							Vec2(ZERO, ZERO + (DEFAULT_POSITION * heightScale) + (i * (VERTICAL_MARGIN * heightScale))));	
		auto toUpPos =			MoveBy::create(ZERO,							Vec2(ZERO, ZERO + (BAR_HEIGHT * heightScale)));

		auto start = EaseElasticIn::create(defaultMove->clone(), START_FINISH_BAR_DURATION);
		auto finish = EaseElasticOut::create(defaultMove->clone(), START_FINISH_BAR_DURATION);

		auto stopActionsCallback = CallFunc::create(CC_CALLBACK_0(SlotScene::actionsStoped, this));

		Sequence* sequence;

		switch (number)
		{
		case BAR::FIRST_BAR:
			sequence = Sequence::create(
				start, toDownAfterStart,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toDefPos, finish, nullptr);

			bar[i]->runAction(sequence);
			break;
		case BAR::SECOND_BAR:
			sequence = Sequence::create(
				start, toDownAfterStart,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toDefPos, finish, nullptr);

			bar[i]->runAction(sequence);
			break;

		case BAR::THIRD_BAR:
			sequence = Sequence::create(
				start, toDownAfterStart,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toDefPos, finish, nullptr);

			bar[i]->runAction(sequence);
			break;
		case BAR::FOURTH_BAR:
			sequence = Sequence::create(
				start, toDownAfterStart,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toDefPos, finish, nullptr);

			bar[i]->runAction(sequence);
			break;

		case BAR::FIFTH_BAR:
			sequence = Sequence::create(
				start, toDownAfterStart,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toUpPos, toDownPos,
				toDefPos, finish, 
				stopActionsCallback, nullptr);

			bar[i]->runAction(sequence);
			break;

		default:
			break;
		}
	}
}

void SlotScene::stopSlotMachine()
{
	for (int i = 0; i < machine.size(); i++)
	{
		stopSlotBar(machine[i]);
	}
}
void SlotScene::stopSlotBar(std::vector<Sprite*> &bar)
{
	for (int i = 0; i < bar.size(); i++)
	{
		bar[i]->stopAllActions();
	}
}

void SlotScene::shuffleSlotMachine(float dt)
{
	unschedule(CC_SCHEDULE_SELECTOR(SlotScene::shuffleSlotMachine));
	for (int i = 0; i < machine.size(); i++)
	{
		shuffleSlotBar(machine[i]);
	}
}
void SlotScene::shuffleSlotBar(std::vector<Sprite*> &bar)
{
	for (int i = 0; i < 4 /*bar.size()*/; i++)
	{
		int randomIndex = rand() % 6;

		Vec2 tmpPos = bar[i]->getPosition();
		bar[i]->setPosition(bar[randomIndex]->getPosition());
		bar[randomIndex]->setPosition(tmpPos);

		std::swap(bar[i], bar[randomIndex]);
	}
}

std::vector<int> SlotScene::getWinItems()
{
	std::vector<int> coconut;
	std::vector<int> cherry;
	std::vector<int> mishok;
	std::vector<int> buterrfly;
	std::vector<int> seven;
	std::vector<int> crystall;

	for (int i = 0; i < machine.size(); i++)
	{
		switch (machine[i][CENTER_LINE_ITEM]->getTag() % 10)
		{
		case COCONUT:
			coconut.push_back(machine[i][CENTER_LINE_ITEM]->getTag() / 10);
			break;
		case CHERRY:
			cherry.push_back(machine[i][CENTER_LINE_ITEM]->getTag() / 10);
			break;
		case MISHOK:
			mishok.push_back(machine[i][CENTER_LINE_ITEM]->getTag() / 10);
			break;
		case BUTERRFLY:
			buterrfly.push_back(machine[i][CENTER_LINE_ITEM]->getTag() / 10);
			break;
		case SEVEN:
			seven.push_back(machine[i][CENTER_LINE_ITEM]->getTag() / 10);
			break;
		case CRISTALL:
			crystall.push_back(machine[i][CENTER_LINE_ITEM]->getTag() / 10);
			break;
		default:
			break;
		}
		
	}
	
	if (coconut.size() >= 3)
	{
		return coconut;
	}
	else if (cherry.size() >= 3)
	{
		return cherry;
	}
	else if (mishok.size() >= 3)
	{
		return mishok;
	}
	else if (buterrfly.size() >= 3)
	{
		return buterrfly;
	}
	else if (seven.size() >= 3)
	{
		return seven;
	}
	else if (crystall.size() >= 3)
	{
		return crystall;
	}

	return std::vector<int>();
}
void SlotScene::animateWinItems(std::vector<int> items)
{
	auto scaleOn = EaseElasticOut::create(ScaleTo::create(.25f, 1.2f), 2.f);
	auto scaleOff = ScaleTo::create(.25f, 1.f);

	Sequence* sequence = Sequence::create(scaleOn, scaleOff, nullptr);

	for (int i = 0; i < items.size(); i++)
	{
		int index = items[i];
		machine[index - 1][CENTER_LINE_ITEM]->runAction(RepeatForever::create(sequence->clone()));
	}
}
void SlotScene::stopAnimateWinItem(std::vector<int> items)
{
	for (int i = 0; i < items.size(); i++)
	{
		int index = items[i];
		machine[index - 1][CENTER_LINE_ITEM]->stopAllActions();
	}
}

void SlotScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void SlotScene::spinCallback(cocos2d::Ref* pSender)
{
	if (_status == SLOTS_STATUS::STOPED)
	{
		schedule(CC_SCHEDULE_SELECTOR(SlotScene::shuffleSlotMachine), 0.9f);
		_status = SLOTS_STATUS::RUNNING;
		_spinLabel->setString("Stop");
		stopAnimateWinItem(getWinItems());
		runSlotMachine();
	}
	else if (_status == SLOTS_STATUS::RUNNING)
	{
		_status = SLOTS_STATUS::STOPED;
		_spinLabel->setString("Spin");
		stopSlotMachine();
		setDefaulSlotMachinePosition();
		animateWinItems(getWinItems());
	}
}
void SlotScene::actionsStoped()
{
	_status = SLOTS_STATUS::STOPED;
	_spinLabel->setString("Spin");
	animateWinItems(getWinItems());
}
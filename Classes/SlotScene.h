#ifndef __SLOT_SCENE_H__
#define __SLOT_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include "defines.h"
#include "enums.h"

USING_NS_CC;

class SlotScene : public cocos2d::Layer
{
private: 
	int _zOrder;
	float widthScale;
	float heightScale;
	std::vector<std::vector<Sprite*>> machine;
	SLOTS_STATUS _status;
	Label* _spinLabel;

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
	void spinCallback(cocos2d::Ref* pSender);
	void actionsStoped();

	void initButton();
	void initCenterBackground();
	void initUpBottomBackground();

	int getNextZOrder();
	void initSlotMachine();
	void initSlotBarByNumber(int number, std::vector<Sprite*> &bar);

	void setDefaulSlotMachinePosition();
	void setDefaultSlotBarPosition(std::vector<Sprite*> &bar);

	void runSlotMachine();
	void runSlotBarByNumber(int number, std::vector<Sprite*> &bar);

	void stopSlotMachine();
	void stopSlotBar(std::vector<Sprite*> &bar);

	void shuffleSlotMachine(float dt);
	void shuffleSlotBar(std::vector<Sprite*> &bar);

	std::vector<int> getWinItems();
	void animateWinItems(std::vector<int> items);
	void stopAnimateWinItem(std::vector<int> items);
    
    CREATE_FUNC(SlotScene);
};

#endif // __SLOT_SCENE_H__

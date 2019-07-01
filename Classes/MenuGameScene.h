#ifndef __MENUGAME_SCENE_H__
#define __MENUGAME_SCENE_H__
#include "ui/CocosGUI.h"
#include "cocos2d.h"
USING_NS_CC;
class MenuGame : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void update(float dt);
	cocos2d::ui::LoadingBar* load;
	int percent = 0;
	cocos2d::Label* label;
    // implement the "static create()" method manually
    CREATE_FUNC(MenuGame);
	
private:
	int LoadingStep;
};

#endif // __MENUGAME_SCENE_H__

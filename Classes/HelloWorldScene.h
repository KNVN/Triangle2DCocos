#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "ui/CocosGUI.h"
using namespace std;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PluginChartboost/PluginChartboost.h"
#endif
class HelloWorld : public cocos2d::Scene, public b2ContactListener, public sdkbox::ChartboostListener
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void SpawnBrick();
	void ReplaceBrick();
//	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);
	void update(float dt);
	void EndGame();
	void fireworkEffect(cocos2d::Sprite* fw);
	void loadSprite();
	void Star(int number);
	void ShowBanner();
	void HideBanner();
	//chartboost ad
	void onChartboostCached(const std::string& name);
	bool onChartboostShouldDisplay(const std::string& name);
	void onChartboostDisplay(const std::string& name);
	void onChartboostDismiss(const std::string& name);
	void onChartboostClose(const std::string& name);
	void onChartboostClick(const std::string& name);
	void onChartboostReward(const std::string& name, int reward);
	void onChartboostFailedToLoad(const std::string& name, sdkbox::CB_LoadError e);
	void onChartboostFailToRecordClick(const std::string& name, sdkbox::CB_ClickError e);
	void onChartboostConfirmation();
	void onChartboostCompleteStore();
	//
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	cocos2d::Vector<cocos2d::Sprite*>*_blackblock;
	cocos2d::Vector<cocos2d::Sprite*>*_brick;
	cocos2d::Vector<cocos2d::Sprite*>*_greenblock;
	cocos2d::Vector<cocos2d::Sprite*>*_DeadBlock;
	cocos2d::Vector<cocos2d::Sprite*>*_chest;
	cocos2d::RepeatForever* fast;
	cocos2d::RepeatForever* normal;
	cocos2d::Sprite* trian1;
	cocos2d::Sprite* trian2;
	cocos2d::Sprite* trian3;
	cocos2d::Sprite* bg1;
	cocos2d::Sprite* bg2;
	cocos2d::Sprite* bg3;
	cocos2d::Sprite* bg4;
	cocos2d::Sprite* firework;
	cocos2d::Sprite* fireworks;
	cocos2d::Sprite* table;
	cocos2d::Sprite* health_border;
	cocos2d::ui::LoadingBar* health;
	cocos2d::ui::Button* play;
	cocos2d::ui::Button* reload;
	cocos2d::Label* label;
	float percent = 100;
	int At_blackblockNext = 0;
	int At_greenblockNext = 0;
	int At_brickNext = 0;
	int At_chestNext = 0;
	int At_deadBlockNext = 0;
	int velocity = 0;
	int score = 0;
	unsigned int soundId;
	bool needDestroy = false;
	bool needContact = false;
	bool soundStart = true;
	bool needChest = false;
	bool needDeadBlock = false;
	bool maxhealth = false;
	b2Body *trianbody; 

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	b2World *world;
};

#endif // __HELLOWORLD_SCENE_H__

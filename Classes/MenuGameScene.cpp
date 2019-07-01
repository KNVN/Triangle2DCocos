#include "MenuGameScene.h"
#include "HelloWorldScene.h"
#include "audio/include/AudioEngine.h"
USING_NS_CC;
using namespace std;
using namespace cocos2d::experimental;
Scene* MenuGame::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MenuGame::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuGame::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	load = cocos2d::ui::LoadingBar::create("loading.png");
	load->setScaleX(visibleSize.width*0.7 / load->getContentSize().width);
	load->setScaleY(visibleSize.height*0.05 / load->getContentSize().height);
	load->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	load->setDirection(cocos2d::ui::LoadingBar::Direction::LEFT);
	this->addChild(load, 1);
	auto percenttemp = String::createWithFormat("%i %", percent);
	label = Label::createWithTTF(percenttemp->getCString(), "fonts/Marker Felt.ttf", visibleSize.height*0.12);
	label->setColor(Color3B::YELLOW);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.7 - label->getContentSize().height / 2));
	this->addChild(label, 2);
	this->scheduleUpdate();
	
		return true;
}

void MenuGame::update(float dt)
{
	switch (LoadingStep)
	{
	case 0:
		load->setPercent(6);
		break;
	case 1:
		AudioEngine::preload("over.mp3");
		percent = 5;
		load->setPercent(percent);
		break;
	case 2:
		AudioEngine::preload("jump.mp3");
		percent = 10;
		load->setPercent(percent);
		break;
	case 3:
		AudioEngine::preload("fast.mp3");
		AudioEngine::preload("select.mp3");
		percent = 20; load->setPercent(percent);
		break;
	case 4:
		AudioEngine::preload("score.mp3");
		AudioEngine::preload("fireworks.mp3");
		percent = 25;
		load->setPercent(percent);
		break;
	}
	LoadingStep++;
	percent++;
	load->setPercent(percent);
	auto percenttemp = String::createWithFormat("%i%%", percent);
	label->setString(percenttemp->getCString());
	if (percent >= 100)
	{
		auto GameScene = HelloWorld::createScene();
		Director::getInstance()->replaceScene(GameScene);
	}
}

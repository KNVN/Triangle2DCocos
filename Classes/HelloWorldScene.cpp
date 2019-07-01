#include "HelloWorldScene.h"
#include "audio/include/AudioEngine.h"
//#include "GLES-Render.h"
#define PTM_RATIO 32.0
USING_NS_CC;
using namespace std;
using namespace cocos2d::experimental;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "pluginadmob/PluginAdMob.h"
#include "PluginChartboost/PluginChartboost.h"
using namespace sdkbox;
#endif
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
//auto audio = CocosDenshion::SimpleAudioEngine::getInstance();//
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
	//ads
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
	PluginChartboost::setListener(this);
	PluginChartboost::cache(sdkbox::CB_Location_Default);
	PluginAdMob::cache("home");
	PluginAdMob::cache("gameover");
#endif

	// create Box2d world

	b2Vec2 gravity(0.0f, -12.0f);
	world = new b2World(gravity);
	world->SetAllowSleeping(true);
	world->SetContactListener(this);
	//add some backgrounds
	bg1 = Sprite::create("universe-01.png");
	bg1->setScaleX(visibleSize.width / bg1->getContentSize().width);
	bg1->setScaleY(visibleSize.height / bg1->getContentSize().height);
	bg1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg1, -2);
	//
	bg2 = Sprite::create("yellow-background.png");
	bg2->setScaleX(visibleSize.width / bg2->getContentSize().width);
	bg2->setScaleY(visibleSize.height / bg2->getContentSize().height);
	bg2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	bg2->setOpacity(0);
	this->addChild(bg2, -2);
	//
	bg3 = Sprite::create("green-background.png");
	bg3->setScaleX(visibleSize.width / bg3->getContentSize().width);
	bg3->setScaleY(visibleSize.height / bg3->getContentSize().height);
	bg3->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	bg3->setOpacity(0);
	this->addChild(bg3, -2);
	//
	bg4 = Sprite::create("white-background.png");
	bg4->setScaleX(visibleSize.width / bg4->getContentSize().width);
	bg4->setScaleY(visibleSize.height / bg4->getContentSize().height);
	bg4->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	bg4->setOpacity(0);
	this->addChild(bg4, -2);
	//add some fireworks(milestones)
	firework = Sprite::create("firework.png");
	firework->setScale(visibleSize.width*0.002 / firework->getContentSize().width);

	this->addChild(firework, 1);
	fireworks = Sprite::create("three-fireworks.png");
	fireworks->setScale(visibleSize.height*0.002 / fireworks->getContentSize().height);
	this->addChild(fireworks, 1);
	//add table
	table = Sprite::create("table.png");
	table->setScale(visibleSize.width*0.7 / table->getContentSize().width);
	table->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	table->setOpacity(0);
	this->addChild(table, 3);
	//add reload
	reload = cocos2d::ui::Button::create("restart.png");
	reload->setScale(visibleSize.width*0.2 / reload->getContentSize().width);
	reload->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - table->getBoundingBox().size.height / 2 - visibleSize.height*0.02 - reload->getBoundingBox().size.height / 2));
	reload->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
														   AudioEngine::play2d("select.mp3");
														   auto ReplaceScene = HelloWorld::createScene();
														   Director::getInstance()->replaceScene(ReplaceScene);
														   break;
		}
		default:
			break;
		}
	});
	this->addChild(reload, 3);
	reload->setOpacity(0);
	reload->setTouchEnabled(false);


	//create main character
	trian1 = cocos2d::Sprite::create("triangle-1.png");
	this->addChild(trian1, 0);
	trian1->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height*0.7+origin.y));
	trian1->setScale(visibleSize.width*0.08 / trian1->getContentSize().width);
	trian1->setTag(0);
	trian2 = cocos2d::Sprite::create("triangle-2.png");
	this->addChild(trian2, 0);
	trian2->setScale(visibleSize.width*0.08 / trian2->getContentSize().width);
	trian2->setOpacity(0);
	trian2->setTag(0);
	trian3 = cocos2d::Sprite::create("triangle-3.png");
	this->addChild(trian3, 0);
	trian3->setScale(visibleSize.width*0.08 / trian3->getContentSize().width);
	trian3->setOpacity(0);
	trian3->setTag(0);
	//
	normal = RepeatForever::create(RotateBy::create(1.0f, 360.0f));
	normal->setTag(2);
	fast = RepeatForever::create(RotateBy::create(0.3f, 360.0f));
	trian1->runAction(normal);
	// create box-shaped body for triangle
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(trian1->getPositionX() / PTM_RATIO, trian1->getPositionY() / PTM_RATIO);
	bodyDef.userData = trian1;
	b2CircleShape circleShape;
	circleShape.m_radius = trian1->getBoundingBox().size.width/2 / PTM_RATIO;
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &circleShape;
	myFixtureDef.restitution = 0.25f;
	myFixtureDef.friction = 2.0f;

	trianbody = world->CreateBody(&bodyDef);
	trianbody->CreateFixture(&myFixtureDef);
	//create triangle's health
	health = cocos2d::ui::LoadingBar::create("health-bar.png");
	health->setDirection(cocos2d::ui::LoadingBar::Direction::LEFT);
	health->setPercent(percent);
	health->setScaleX(visibleSize.width*0.25 / health->getContentSize().width);
	health->setScaleY(visibleSize.height*0.02 / health->getContentSize().height);
	health->setPosition(Vec2(health->getBoundingBox().size.width / 2, visibleSize.height - health->getBoundingBox().size.height/2));
	this->addChild(health,1);
	 health_border = Sprite::create("health-box.png");
	health_border->setScaleX(visibleSize.width*0.25 / health_border->getContentSize().width);
	health_border->setScaleY(visibleSize.height*0.02 / health_border->getContentSize().height);
	health_border->setPosition(Vec2(health_border->getBoundingBox().size.width / 2, visibleSize.height - health_border->getBoundingBox().size.height/2));
	this->addChild(health_border, 1);
	//add score lable
	//score
	auto scoretemp = String::createWithFormat("%i", score);
	//label = cocos2d::Label::createWithBMFont("fonts/Marker Felt.fnt", scoretemp->getCString());
	label = Label::createWithTTF(scoretemp->getCString(),"fonts/Marker Felt.ttf", visibleSize.height*0.12);
	label->setColor(Color3B::WHITE);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - visibleSize.height*0.02 - label->getContentSize().height / 2));
	this->addChild(label, 2);
	//add play button
	play = cocos2d::ui::Button::create("play-button.png");//storm button
	play->setScale(visibleSize.width*0.6 / play->getContentSize().width);
	play->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height/2));
	play->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
														   play->setTouchEnabled(false);
														   play->setOpacity(0);
														   AudioEngine::play2d("select.mp3");
														   this->SpawnBrick();
														   this->runAction(RepeatForever::create(Sequence::create(DelayTime::create(4.0f), CallFunc::create(CC_CALLBACK_0(HelloWorld::SpawnBrick, this)), NULL)));
														   this->runAction(RepeatForever::create(Sequence::create(DelayTime::create(30.0f), CallFunc::create([&]{needChest = true; }), NULL)));
														   this->runAction(RepeatForever::create(Sequence::create(DelayTime::create(7.5f), CallFunc::create([&]{if (score >= 200){ needDeadBlock = true; } }), NULL)));
														   this->runAction(Repeat::create(Sequence::create(DelayTime::create(1.8f), CallFunc::create(CC_CALLBACK_0(HelloWorld::loadSprite, this)), NULL), 50));
														   this->scheduleUpdate();
														   this->HideBanner();
														   break;
		}
		default:
			break;
		}
	});
	this->addChild(play, 2);
	// debug draw
	/*uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	flags += b2Draw::e_aabbBit;
	auto debugDraw = new GLESDebugDraw(32);
	debugDraw->SetFlags(flags);
	world->SetDebugDraw(debugDraw);
	world->DrawDebugData();*/
	_DeadBlock = new Vector<Sprite*>(50);
    _blackblock = new Vector<Sprite*>(75);
	_greenblock = new Vector<Sprite*>(75);
	_brick = new Vector<Sprite*>(75);
	
	for (int i = 0; i < 25; ++i) {
		auto blackblock = Sprite::create("black-block.png");
		auto greenblock = Sprite::create("green-block.png");
		auto brick = Sprite::create("brick.png");
		//add child
		this->addChild(blackblock, 0);
		this->addChild(greenblock, 0);
		this->addChild(brick, 0);
		//opacity
		blackblock->setOpacity(0);
		greenblock->setOpacity(0);
		brick->setOpacity(0);
		//push back vector
		_blackblock->pushBack(blackblock);
		_greenblock->pushBack(greenblock);
		_brick->pushBack(brick);
	}
/*	_DeadBlock = new Vector<Sprite*>(40);
	for (int i = 0; i < 40; ++i) {
		auto deadBlock = Sprite::create("deadblock.png");
		//add child
		this->addChild(deadBlock, 0);
		//opacity
		deadBlock->setOpacity(0);
		//push back vector
		_DeadBlock->pushBack(deadBlock);
	}*/
	_chest = new Vector<Sprite*>(8);
	for (int i = 0; i < 8; ++i) {
		auto chest = Sprite::create("chest.png");
		this->addChild(chest, 0);
		//opacity
		chest->setOpacity(0);
		//push back vector
		_chest->pushBack(chest);
	}
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "home.png",
                                           "home.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

 if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
		closeItem->setScale(visibleSize.width*0.1 / closeItem->getContentSize().width);
        float x = origin.x + visibleSize.width - closeItem->getBoundingBox().size.width/2;
        float y = origin.y + closeItem->getBoundingBox().size.height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    //  ADD EVENT LISTENER

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};

	listener->onTouchMoved = [=](Touch* touch, Event* event){
		//trian->stopActionByTag(1);
		Sprite* trian = (Sprite *)trianbody->GetUserData();
		if (trian->getActionByTag(2) != NULL){
			trian->stopActionByTag(2);
			fast = RepeatForever::create(RotateBy::create(0.3f, 360.0f));
			fast->setTag(1);
			trian->runAction(fast);
			if (soundStart){ //run sound first time
				soundId = AudioEngine::play2d("fast.mp3", true);
				soundStart = false;
			}
			AudioEngine::resume(soundId);
		}
		trianbody->SetLinearVelocity(b2Vec2(0, -visibleSize.height*0.3/ PTM_RATIO));
		needContact = true;
	};

	listener->onTouchEnded = [=](Touch* touch, Event* event){
		needContact = false;
		//trian->stopActionByTag(2);
		Sprite* trian = (Sprite *)trianbody->GetUserData();
		if (trian->getActionByTag(1) != NULL){
			trian->stopActionByTag(1);
			normal = RepeatForever::create(RotateBy::create(1.0f, 360.0f));
			normal->setTag(2);
			trian->runAction(normal);
			AudioEngine::pause(soundId);
		}
		AudioEngine::play2d("jump.mp3");
		trianbody->SetLinearVelocity(b2Vec2(0, 0));
		if (trianbody->GetPosition().x*PTM_RATIO <= touch->getLocation().x)
		{
			trianbody->ApplyLinearImpulse(b2Vec2(visibleSize.width*0.15 / PTM_RATIO, visibleSize.height*0.25 / PTM_RATIO), trianbody->GetWorldCenter(), true);
		}
		if (trianbody->GetPosition().x*PTM_RATIO > touch->getLocation().x){
			trianbody->ApplyLinearImpulse(b2Vec2(-visibleSize.width*0.15 / PTM_RATIO, visibleSize.height*0.25 / PTM_RATIO), trianbody->GetWorldCenter(), true);
		}
		this->scheduleUpdate();
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//////////////
	return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
	AudioEngine::play2d("select.mp3");
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
/*void HelloWorld::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	cocos2d::Node::draw(renderer, transform, flags);

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	world->DrawDebugData();

	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}*/
void HelloWorld::SpawnBrick()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int black_num = rand() % 10;
	int green_num = rand() % (10 - black_num);
	int brick_num = 10 - black_num - green_num;
	int num = 0;
	for (int i = 0; i < black_num; ++i){
		auto blackblock = (Sprite*)_blackblock->at(At_blackblockNext);
		blackblock->setTag(1);
		At_blackblockNext++;
		if (At_blackblockNext >= _blackblock->size())
			At_blackblockNext = 0;

		blackblock->setScale(visibleSize.width*0.1 / blackblock->getContentSize().width);
		blackblock->setPosition(Vec2(blackblock->getBoundingBox().size.width*num + blackblock->getBoundingBox().size.width/2, visibleSize.height*-1.5));
		b2BodyDef bodyDef;
		bodyDef.type = b2_kinematicBody;
		bodyDef.position.Set(blackblock->getPositionX() / PTM_RATIO, blackblock->getPositionY() / PTM_RATIO);
		bodyDef.userData = blackblock;
		auto blackblock_body = world->CreateBody(&bodyDef);
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(blackblock->getBoundingBox().size.width / 2 / PTM_RATIO, blackblock->getBoundingBox().size.height/2 / PTM_RATIO);
		b2FixtureDef FixtureDef;
		FixtureDef.shape = &polygonShape;
		FixtureDef.filter.groupIndex = 2;
		FixtureDef.friction = 1.0f;
		blackblock_body->CreateFixture(&FixtureDef);
		blackblock->setOpacity(255);
		blackblock_body->SetLinearVelocity(b2Vec2(0, (visibleSize.height*0.15+velocity) / PTM_RATIO));
		//blackblock->runAction(MoveBy::create(5.0f, Vec2(0, visibleSize.width * 2)));
		num++;
	}
	for (int i = 0; i < green_num; ++i){
		auto greenblock = (Sprite*)_greenblock->at(At_greenblockNext);
		greenblock->setTag(2);
		At_greenblockNext++;
		if (At_greenblockNext >= _greenblock->size())
			At_greenblockNext = 0;

		greenblock->setScale(visibleSize.width*0.1 / greenblock->getContentSize().width);
		greenblock->setPosition(Vec2(greenblock->getBoundingBox().size.width*num + greenblock->getBoundingBox().size.width / 2, visibleSize.height*-1.5));
		b2BodyDef bodyDef;
		bodyDef.type = b2_kinematicBody;
		bodyDef.position.Set(greenblock->getPositionX() / PTM_RATIO, greenblock->getPositionY() / PTM_RATIO);
		bodyDef.userData = greenblock;
		auto greenblock_body = world->CreateBody(&bodyDef);
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(greenblock->getBoundingBox().size.width / 2 / PTM_RATIO, greenblock->getBoundingBox().size.height / 2 / PTM_RATIO);
		b2FixtureDef FixtureDef;
		FixtureDef.shape = &polygonShape;
		FixtureDef.friction = 1.0f;
		greenblock_body->CreateFixture(&FixtureDef);
		greenblock->setOpacity(255);
		greenblock_body->SetLinearVelocity(b2Vec2(0, (visibleSize.height*0.15+velocity) / PTM_RATIO));
		num++;
		//greenblock->runAction(MoveBy::create(5.0f, Vec2(0, visibleSize.width * 2)));
	}
	 for (int i = 0; i < brick_num; ++i){
		auto brick = (Sprite*)_brick->at(At_brickNext);
		brick->setTag(3);
		At_brickNext++;
		if (At_brickNext >= _brick->size())
			At_brickNext = 0;

		brick->setScale(visibleSize.width*0.1 / brick->getContentSize().width);
		brick->setPosition(Vec2(brick->getBoundingBox().size.width*num + brick->getBoundingBox().size.width / 2, visibleSize.height*-1.5));
		b2BodyDef bodyDef;
		bodyDef.type = b2_kinematicBody;
		bodyDef.position.Set(brick->getPositionX() / PTM_RATIO, brick->getPositionY() / PTM_RATIO);
		bodyDef.userData = brick;
		auto brick_body = world->CreateBody(&bodyDef);
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(brick->getBoundingBox().size.width / 2 / PTM_RATIO, brick->getBoundingBox().size.height / 2 / PTM_RATIO);
		b2FixtureDef FixtureDef;
		FixtureDef.shape = &polygonShape;
		FixtureDef.friction = 1.0f;
		brick_body->CreateFixture(&FixtureDef);
		brick->setOpacity(255);
		brick_body->SetLinearVelocity(b2Vec2(0, (visibleSize.height*0.15 + velocity) / PTM_RATIO));
		num++;
		//brick->runAction(MoveBy::create(5.0f, Vec2(0, visibleSize.width * 2)));
	} 
	 
	 int Columns = rand() % 6;
	 for(int i = 0; i < Columns; ++i){
		 int AtBrick = rand() % 10;
		 int Row = rand() % 4;
		 for (int i = 0; i < Row; ++i){
			 auto brick = (Sprite*)_brick->at(At_brickNext);
			 brick->setTag(3);
			 At_brickNext++;
			 if (At_brickNext >= _brick->size())
				 At_brickNext = 0;

			 brick->setScale(visibleSize.width*0.1 / brick->getContentSize().width);
			 brick->setPosition(Vec2(brick->getBoundingBox().size.width*AtBrick + brick->getBoundingBox().size.width / 2, visibleSize.height*-1.5 + brick->getBoundingBox().size.height*i));
			 b2BodyDef bodyDef;
			 bodyDef.type = b2_kinematicBody;
			 bodyDef.position.Set(brick->getPositionX() / PTM_RATIO, brick->getPositionY() / PTM_RATIO);
			 bodyDef.userData = brick;
			 auto brick_body = world->CreateBody(&bodyDef);
			 b2PolygonShape polygonShape;
			 polygonShape.SetAsBox(brick->getBoundingBox().size.width / 2 / PTM_RATIO, brick->getBoundingBox().size.height / 2 / PTM_RATIO);
			 b2FixtureDef FixtureDef;
			 FixtureDef.shape = &polygonShape;
			 FixtureDef.friction = 1.0f;
			 brick_body->CreateFixture(&FixtureDef);
			 brick->setOpacity(255);
			 brick_body->SetLinearVelocity(b2Vec2(0, (visibleSize.height*0.15 + velocity) / PTM_RATIO));
			 //brick->runAction(MoveBy::create(5.0f, Vec2(0, visibleSize.width * 2)));
		 }
	 }
	 //chest
	 if (needChest){
		 int AtBrick = rand() % 10;
		 int Row = rand() % 8;
		 auto chest = (Sprite*)_chest->at(At_chestNext);
		 chest->setTag(-1);
		 At_chestNext++;
		 if (At_chestNext >= _chest->size())
			 At_chestNext = 0;

		 chest->setScale(visibleSize.width*0.09 / chest->getContentSize().width);
		 chest->setPosition(Vec2(chest->getBoundingBox().size.width*AtBrick + chest->getBoundingBox().size.width / 2, visibleSize.height*-1.5 + chest->getBoundingBox().size.height*Row));
		 b2BodyDef bodyDef;
		 bodyDef.type = b2_kinematicBody;
		 bodyDef.position.Set(chest->getPositionX() / PTM_RATIO, chest->getPositionY() / PTM_RATIO);
		 bodyDef.userData = chest;
		 auto chest_body = world->CreateBody(&bodyDef);
		 b2PolygonShape polygonShape;
		 polygonShape.SetAsBox(chest->getBoundingBox().size.width / 2 / PTM_RATIO, chest->getBoundingBox().size.height / 2 / PTM_RATIO);
		 b2FixtureDef FixtureDef;
		 FixtureDef.shape = &polygonShape;
		 FixtureDef.friction = 1.0f;
		 chest_body->CreateFixture(&FixtureDef);
		 chest->setOpacity(255);
		 chest_body->SetLinearVelocity(b2Vec2(0, (visibleSize.height*0.15 + velocity) / PTM_RATIO));
		 needChest = false;
	 }
	 if (needDeadBlock){
		 int AtBrick = rand() % 10;
		 int Row = rand() % 6;
		 auto deadblock = (Sprite*)_DeadBlock->at(At_deadBlockNext);
		 deadblock->setTag(-3);
		 At_deadBlockNext++;
		 if (At_deadBlockNext >= _DeadBlock->size())
			 At_deadBlockNext = 0;

		 deadblock->setScale(visibleSize.width*0.1 / deadblock->getContentSize().width);
		 deadblock->setPosition(Vec2(deadblock->getBoundingBox().size.width*AtBrick + deadblock->getBoundingBox().size.width / 2, visibleSize.height*-1.5 + deadblock->getBoundingBox().size.height*Row));
		 b2BodyDef bodyDef;
		 bodyDef.type = b2_kinematicBody;
		 bodyDef.position.Set(deadblock->getPositionX() / PTM_RATIO, deadblock->getPositionY() / PTM_RATIO);
		 bodyDef.userData = deadblock;
		 auto deadblock_body = world->CreateBody(&bodyDef);
		 b2CircleShape circleShape;
		 circleShape.m_radius = deadblock->getBoundingBox().size.width / 2 / PTM_RATIO;
		 b2FixtureDef FixtureDef;
		 FixtureDef.shape = &circleShape;
		 FixtureDef.friction = 1.0f;
		 deadblock_body->CreateFixture(&FixtureDef);
		 deadblock->setOpacity(255);
		 deadblock_body->SetLinearVelocity(b2Vec2(0, (visibleSize.height*0.15 + velocity) / PTM_RATIO));
		 needDeadBlock = false;
	 }
	 velocity = velocity+7;
}
void HelloWorld::ReplaceBrick(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (score >= 400){
		for (auto b = world->GetBodyList(); b; b = b->GetNext())
		{

			if (b->GetUserData() != NULL)
			{
				// sprites follow bodies
				Sprite *sprite = (Sprite *)b->GetUserData();
				int tag = sprite->getTag();
				if (tag != 0 && tag != -1 && tag != -2)
				{//replace data
					sprite->setOpacity(0);
					int type = rand() % 4;
					switch (type)
					{
					case 0:
					{
							  auto blackblock = (Sprite*)_blackblock->at(At_blackblockNext);
							  blackblock->setTag(1);
							  At_blackblockNext++;
							  if (At_blackblockNext >= _blackblock->size())
								  At_blackblockNext = 0;

							  blackblock->setScale(visibleSize.width*0.1 / blackblock->getContentSize().width);
							  blackblock->setOpacity(255);
							  b->SetUserData(blackblock);
							  break;
					}
					case 1:
					{
							  auto greenblock = (Sprite*)_greenblock->at(At_greenblockNext);
							  greenblock->setTag(2);
							  At_greenblockNext++;
							  if (At_greenblockNext >= _greenblock->size())
								  At_greenblockNext = 0;

							  greenblock->setScale(visibleSize.width*0.1 / greenblock->getContentSize().width);
							  greenblock->setOpacity(255);
							  b->SetUserData(greenblock);
							  break;
					}
					case 2:
					{
							  auto brick = (Sprite*)_brick->at(At_brickNext);
							  brick->setTag(3);
							  At_brickNext++;
							  if (At_brickNext >= _brick->size())
								  At_brickNext = 0;

							  brick->setScale(visibleSize.width*0.1 / brick->getContentSize().width);
							  brick->setOpacity(255);
							  b->SetUserData(brick);
							  break;
					}
					case 3:
					{
							  auto deadblock = (Sprite*)_DeadBlock->at(At_deadBlockNext);
							  deadblock->setTag(-3);
							  At_deadBlockNext++;
							  if (At_deadBlockNext >= _DeadBlock->size())
								  At_deadBlockNext = 0;

							  deadblock->setScale(visibleSize.width*0.1 / deadblock->getContentSize().width);
							  deadblock->setOpacity(255);
							  b->SetUserData(deadblock);
							  break;
					}
					default:
						break;
					}
				}
			}
		}
	}
}
void HelloWorld::update(float dt)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float timeStep = 1 / 20.0f;
	int velocityIterations = 24;
	int positionIterations = 12;
	world->Step(timeStep, velocityIterations, positionIterations);

	//set the bodies'postion
	for (auto b = world->GetBodyList(); b; b = b->GetNext())
	{
		
		if (b->GetUserData() != NULL)
		{
			// sprites follow bodies
			Sprite *sprite = (Sprite *)b->GetUserData();
			sprite->setPosition(Vec2(b->GetPosition().x*PTM_RATIO, b->GetPosition().y*PTM_RATIO));
		}
	}
	//handle bodies
	for (auto b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL)
		{
			Sprite *sprite = (Sprite *)b->GetUserData();
			int tag = sprite->getTag();
			//destroy bodies over screen
			if ((float)sprite->getPositionY() >= (float)visibleSize.height * 1.2)
			{
				if (tag != 0)
				{
					world->DestroyBody(b);
					sprite->stopAllActions();
					sprite->setOpacity(0);
					break;
				}
				else{
					this->EndGame();
				}
			}
			//destroy collision bodies
			if (tag == 4 || tag == 5 || tag == 6 || tag == -2 )
			{
				if (needDestroy)//in Action?
				{
					if (needContact)
					{// on Click?
						sprite->stopAllActions();
						world->DestroyBody(b);
						sprite->setOpacity(0);
						if (tag == 5)
						{
							percent = percent + 30;
							auto fadeIn = FadeIn::create(0.5f);
							auto fadeOut = FadeOut::create(0.5f);
							health_border->runAction(Sequence::create(fadeOut, fadeIn, NULL));
							score += 10;
						}
						else if (tag == 4){ score += 30; }
						else if (tag == -2){
							int LukyNumber = rand() % 4;
							switch (LukyNumber)
							{
							case 1:
							{// scale triangle
									  auto scale1 = ScaleBy::create(0.5f, 4.0f);
									  auto scale2 = ScaleBy::create(0.5f, 0.25f);
									  auto scale3 = ScaleBy::create(0.5f, 1.2f);
									  ((Sprite*)trianbody->GetUserData())->runAction(Sequence::create(scale1, scale2, scale3, NULL));
									
									  break;
							}
							case 0:
							{ // 100% health for 10 secs
									  maxhealth = true;
									  this->runAction(Sequence::create(DelayTime::create(10.0f), CallFunc::create([&]{ maxhealth = false; }), NULL));
								  break;
							}
							case 2:// replace all current blocks
								this->ReplaceBrick();
								break;
							case 3:// change triangle
							{
									   int type = rand() % 3;
									   Sprite* trian = (Sprite *)trianbody->GetUserData();
									   switch (type)
									   {
									   case 0:
									   {
												 trian->setOpacity(0);
												 trian->stopAllActions();
												 trian1->setOpacity(255);
												 trian1->setScale(trian->getBoundingBox().size.width / trian1->getContentSize().width);
												 trianbody->SetUserData(trian1);
												 if (needContact)//action
												 {
													 fast = RepeatForever::create(RotateBy::create(0.3f, 360.0f));
													 fast->setTag(1);
													 trian1->runAction(fast);
												 }
												 else{
													 normal = RepeatForever::create(RotateBy::create(1.0f, 360.0f));
													 normal->setTag(2);
													 trian1->runAction(normal);
												 }
												 break;
									   }
									   case 1:
									   {
												 trian->setOpacity(0);
												 trian->stopAllActions();
												 trian2->setOpacity(255);
												 trian2->setScale(trian->getBoundingBox().size.width / trian2->getContentSize().width);
												 trianbody->SetUserData(trian2);
												 if (needContact)//action
												 {
													 fast = RepeatForever::create(RotateBy::create(0.3f, 360.0f));
													 fast->setTag(1);
													 trian2->runAction(fast);
												 }
												 else{
													 normal = RepeatForever::create(RotateBy::create(1.0f, 360.0f));
													 normal->setTag(2);
													 trian2->runAction(normal);
												 }
												 break;
									   }
									   case 2:
									   {
												 trian->setOpacity(0);
												 trian->stopAllActions();
												 trian3->setOpacity(255);
												 trian3->setScale(trian->getBoundingBox().size.width / trian3->getContentSize().width);
												 trianbody->SetUserData(trian3);
												 if (needContact)//action
												 {
													 fast = RepeatForever::create(RotateBy::create(0.3f, 360.0f));
													 fast->setTag(1);
													 trian3->runAction(fast);
												 }
												 else{
													 normal = RepeatForever::create(RotateBy::create(1.0f, 360.0f));
													 normal->setTag(2);
													 trian3->runAction(normal);
												 }
												 break;
									   }
									   default:
										   break;
									   }
									  break;
							}
							default:
								break;
							}
						}
						else{ score += 20; }
						
						AudioEngine::play2d("score.mp3");
						break;
					}
					else{
						sprite->setTag(3);
						sprite->stopAllActions();
					    }
				 }
			}
			if (tag == 0)
			{ // end game when triangle's over screen: right, left, bottom
				if ((float)sprite->getPositionX() >= (float)visibleSize.width * 1.1 || (float)sprite->getPositionX() <= -(float)visibleSize.width * 0.1 || (float)sprite->getPositionY() <= -(float)visibleSize.height * 0.2)
				{
					this->EndGame();
				}
			}
		}
	}
	world->ClearForces();
	//decrease heath percent
	if (health->getPercent() >= 100 || maxhealth)
	{
		percent = 100;
	}
	if (health->getPercent() <= 0)
	{
		this->EndGame();
	}
	if (needContact){//on click
		percent = percent - 0.5;
        health->setPercent(percent);
	}
	else{
		percent = percent - 0.06;
		health->setPercent(percent);
	}
	// update score
	if (score < 10){ score++; }
	auto scoretemp = String::createWithFormat("%i", score);
	label->setString(scoretemp->getCString());
	// score effects
	switch (score)
	{
	case 100:
	{
				this->fireworkEffect(firework);
				score += 10;
				break;
	}
	case 200:
	{
				this->fireworkEffect(firework);
				score += 10;
				break;
	}
	case 300:
	{
				this->fireworkEffect(firework);
				score += 10;
				break;
	}
	case 400:
	{
				this->fireworkEffect(firework);
				score += 10;
				break;
	}
	case 500:
	case 510:
	{
				this->fireworkEffect(fireworks);
				bg1->setOpacity(0);
				bg2->setOpacity(255);
				score += 20;
				break;
	}
	case 600:
	{
				this->fireworkEffect(firework);
				score += 10;
				break;
	}
	case 700:
	{
				this->fireworkEffect(fireworks);
				score += 10;
				break;
	}
	case 800:
	{
				this->fireworkEffect(firework);
				score += 10;
				break;
	}
	case 900:
	{
				this->fireworkEffect(firework);
				score += 10;
				break;
	}
	case 1100:
	case 1110:
	{
				this->fireworkEffect(fireworks);
				bg2->setOpacity(0);
				bg3->setOpacity(255);
				score += 20;
				break;
	}
	case 1200:
	{
		this->fireworkEffect(firework);
		score += 10;
		this->ShowBanner();
		this->runAction(Sequence::create(DelayTime::create(30.0f), CallFunc::create(CC_CALLBACK_0(HelloWorld::HideBanner, this)), NULL));
		break;
	}
	case 1300:
	{
				 this->fireworkEffect(fireworks);
				 score += 10;
				 break;
	}
	case 1400:
	{
				 this->fireworkEffect(firework);
				 score += 10;
				 break;
	}
	case 1500:
	{
				 this->fireworkEffect(fireworks);
				 this->fireworkEffect(firework);
				 score += 10;
				 break;
	}
	case 1800:
	case 1810:
	{
				 this->fireworkEffect(fireworks);
				 bg3->setOpacity(0);
				 bg4->setOpacity(255);
				 score += 20;
				 this->ShowBanner();
				 this->runAction(Sequence::create(DelayTime::create(30.0f), CallFunc::create(CC_CALLBACK_0(HelloWorld::HideBanner, this)), NULL));
				 break;
	}
	case 1900:
	case 2000:
	case 2200:
	{
				 this->fireworkEffect(fireworks);
				 this->fireworkEffect(firework);
				 score += 10;
				 break;
	}
	case 2500:
	case 2510:
	{
				 this->fireworkEffect(fireworks);
				 bg4->setOpacity(0);
				 bg1->setOpacity(255);
				 score += 20;
				 this->ShowBanner();
				 this->runAction(Sequence::create(DelayTime::create(30.0f), CallFunc::create(CC_CALLBACK_0(HelloWorld::HideBanner, this)), NULL));
				 break;
	}
	case 3000:
	case 3010:
	{
				 this->fireworkEffect(fireworks);
				 bg1->setOpacity(0);
				 bg3->setOpacity(255);
				 score += 20;
				 this->ShowBanner();
				 this->runAction(Sequence::create(DelayTime::create(30.0f), CallFunc::create(CC_CALLBACK_0(HelloWorld::HideBanner, this)), NULL));
				 break;
	}
	default:
		break;
	}
}
void HelloWorld::BeginContact(b2Contact* contact){
	//CCLOG("CONTACTTTTTTTT");
	//onclick
	if (needContact){
		void* Object1 = contact->GetFixtureA()->GetBody()->GetUserData();
		void* Object2 = contact->GetFixtureB()->GetBody()->GetUserData();
		if (Object1 != NULL && Object2 != NULL){
			auto A = static_cast<Sprite*>(Object1);
			auto B = static_cast<Sprite*>(Object2);
			if (A->getTag() == 1 || A->getTag() == 2 || A->getTag() == 3 || A->getTag() == -1)
			{
				b2Body* body2 = contact->GetFixtureA()->GetBody();
				this->unscheduleUpdate();
				//a bug about destroying brick's bodies
				needDestroy = false;
				int tag = A->getTag();
				switch (tag)
				{
				case -1:
					A->setTag(-2);//chest
					break;
				case 1:
					A->setTag(4);
					break;
				case 2:
					A->setTag(5);
					break;
				case 3:
					A->setTag(6);
					break;
				default:
					break;
				}
				//action for bricks
				if (tag != -1 && tag != 1)
				{
					auto shake = Repeat::create(Sequence::create(MoveBy::create(0.05f, Vec2(0, -A->getBoundingBox().size.height*0.25)), MoveBy::create(0.05f, Vec2(0, A->getBoundingBox().size.height*0.25)), NULL), 8);
					//   auto Scale = Sequence::create(shake, ScaleTo::create(0.1f, 0.001), NULL);
					auto seq = Sequence::create(shake, CallFunc::create([&]{needDestroy = true; this->scheduleUpdate(); }), NULL);
					A->runAction(seq);
				}
				else if (tag == 1){
					auto shake = Repeat::create(Sequence::create(MoveBy::create(0.05f, Vec2(0, -A->getBoundingBox().size.height*0.25)), MoveBy::create(0.05f, Vec2(0, A->getBoundingBox().size.height*0.25)), NULL), 12);
					//   auto Scale = Sequence::create(shake, ScaleTo::create(0.1f, 0.001), NULL);
					auto seq = Sequence::create(shake, CallFunc::create([&]{needDestroy = true; this->scheduleUpdate(); }), NULL);
					A->runAction(seq);
				}
				else{
					needDestroy = true;
					this->scheduleUpdate();
				    }
			}//end if tag
		}//end null
	}//end if(needContact)
	void* Object1 = contact->GetFixtureA()->GetBody()->GetUserData();
	void* Object2 = contact->GetFixtureB()->GetBody()->GetUserData();
	if (Object1 != NULL && Object2 != NULL)
	{
		auto A = static_cast<Sprite*>(Object1);
		auto B = static_cast<Sprite*>(Object2);
		if (A->getTag() == -3 || B->getTag() == -3)
		{
			this->EndGame();
		}

	}// handle deadblock
}
void HelloWorld::EndContact(b2Contact* contact){}
void HelloWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){}
void HelloWorld::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){}
void HelloWorld::EndGame()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	int adType = rand() % 3;
	switch (adType)
	{
	case 0:
		this->ShowBanner();
		break;
	case 1:
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
			  PluginAdMob::show("gameover");
#endif
			  break;
	}
	case 2:
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
			  PluginChartboost::show(sdkbox::CB_Location_Default);
#endif
			  break;
	}
	default:
		break;
	}
	AudioEngine::play2d("over.mp3");
	AudioEngine::stop(soundId);
	table->setOpacity(255);
	reload->setOpacity(255);
	reload->setTouchEnabled(true);
		//
    auto text = String::createWithFormat("RECORD");
	auto TextScoreLB = Label::createWithTTF(text->getCString(), "fonts/Marker Felt.ttf", visibleSize.height*0.08);
	TextScoreLB->setColor(Color3B::GRAY);
	TextScoreLB->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.65));
	this->addChild(TextScoreLB, 4);
	
	//
	auto userScore = cocos2d::UserDefault::sharedUserDefault();
	const char *HIGH_SCORE_KEY = "key";
	int HighScore = userScore->getIntegerForKey(HIGH_SCORE_KEY);
	if (score > HighScore)
	{
		HighScore = score;
	}
	userScore->setIntegerForKey(HIGH_SCORE_KEY, HighScore);
	userScore->flush();
	auto Hscore = String::createWithFormat("%i", HighScore);
	auto HLB = Label::createWithTTF(Hscore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height*0.08);
	HLB->setColor(Color3B::RED);
	HLB->setPosition(Vec2(visibleSize.width / 2, visibleSize.height/2));
	this->addChild(HLB, 4);

	
	if (score < 200)
	{
		this->Star(0);
	}
	else if (score < 600){
		this->Star(1);
	}
	else if (score < 1000){
		this->Star(2);
	}
	else if (score < 1600){
		this->Star(3);
	}
	else if (score < 2500){
		this->Star(4);
	}
	else if (score < 3000){
		this->Star(5);
	}
	else{
		this->Star(6);
	}
	this->pauseSchedulerAndActions();
}
void HelloWorld::fireworkEffect(cocos2d::Sprite* fw){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float pst = (float)((int)rand() % 10) / 10;
	fw->setPosition(Vec2(visibleSize.width*pst, visibleSize.height*0.9));
	auto scale1 = ScaleBy::create(1.0f, 100.0f);
	auto scale2 = ScaleBy::create(1.5f, 0.01f);
	fw->runAction(Sequence::create(scale1, DelayTime::create(1.0f), scale2, NULL));
	AudioEngine::play2d("fireworks.mp3");
}
void HelloWorld::loadSprite(){
	auto blackblock = Sprite::create("black-block.png");
	auto greenblock = Sprite::create("green-block.png");
	auto brick = Sprite::create("brick.png");
	auto deadblock = Sprite::create("deadblock.png");
	//add child
	this->addChild(blackblock, 0);
	this->addChild(greenblock, 0);
	this->addChild(brick, 0);
	this->addChild(deadblock, 0);
	//opacity
	blackblock->setOpacity(0);
	greenblock->setOpacity(0);
	brick->setOpacity(0);
	deadblock->setOpacity(0);
	//push back vector
	_blackblock->pushBack(blackblock);
	_greenblock->pushBack(greenblock);
	_brick->pushBack(brick);
	_DeadBlock->pushBack(deadblock);
}
void HelloWorld::Star(int number){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	for (int i = 1; i <= number; i++){
		auto star = Sprite::create("star.png");
		star->setScale(visibleSize.width*0.01 / star->getContentSize().height);
		star->setPosition(Vec2(visibleSize.width*0.12*i + visibleSize.width*0.1, visibleSize.height / 2 - visibleSize.height*0.15));
		this->addChild(star, 4);
		star->runAction(ScaleBy::create(3.0f, 10.0f));
	}
}
//admob ad
void HelloWorld::ShowBanner()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
		PluginAdMob::show("home");
#endif
}
void HelloWorld::HideBanner()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
	if (PluginAdMob::isAvailable("home"))
	{
		PluginAdMob::hide("home");
	}
#endif
}
//chartboost ad
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
void HelloWorld::onChartboostCached(const std::string& name)
{
	CCLOG("onChartboostCached: %s", name.c_str());
}

bool HelloWorld::onChartboostShouldDisplay(const std::string& name)
{
	CCLOG("onChartboostShouldDisplay: %s", name.c_str());
	return true;
}

void HelloWorld::onChartboostDisplay(const std::string& name)
{
	CCLOG("onChartboostDisplay: %s", name.c_str());
}

void HelloWorld::onChartboostDismiss(const std::string& name)
{
	CCLOG("onChartboostDismiss: %s", name.c_str());
}

void HelloWorld::onChartboostClose(const std::string& name)
{
	CCLOG("onChartboostClose: %s", name.c_str());
}

void HelloWorld::onChartboostClick(const std::string& name)
{
	CCLOG("onChartboostClick: %s", name.c_str());
}

template < typename T > std::string to_string(const T& n)
{
	std::ostringstream stm;
	stm << n;
	return stm.str();
}

void HelloWorld::onChartboostReward(const std::string& name, int reward)
{
	CCLOG("onChartboostReward: %s, %d", name.c_str(), reward);
}

void HelloWorld::onChartboostFailedToLoad(const std::string& name, sdkbox::CB_LoadError e)
{
	CCLOG("onChartboostFailedToLoad: %s, %d", name.c_str(), static_cast<int>(e));
}

void HelloWorld::onChartboostFailToRecordClick(const std::string& name, sdkbox::CB_ClickError e)
{
	CCLOG("onChartboostFailToRecordClick: %s, %d", name.c_str(), static_cast<int>(e));
}

void HelloWorld::onChartboostConfirmation()
{
	CCLOG("onChartboostConfirmation");
}

void HelloWorld::onChartboostCompleteStore()
{
	CCLOG("onChartboostCompleteStore");
}
#endif
#include "AppDelegate.h"
#include "MenuGameScene.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "pluginadmob/PluginAdMob.h"
#include "PluginChartboost/PluginChartboost.h"
using namespace sdkbox;
#endif
 #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

#define PORTRAIT 0
#define SCREEN_WIDTH_SD     396
#define SCREEN_HEIGHT_SD    627
#define SCREEN_WIDTH_HD     792
#define SCREEN_HEIGHT_HD    1254
#define SCREEN_WIDTH_RETINA 1584
#define SCREEN_HEIGHT_RETINA 2508

#define GAME_WIDTH_SD       360
#define GAME_HEIGHT_SD      528
#define GAME_WIDTH_HD       704
#define GAME_HEIGHT_HD      1056
#define GAME_WIDTH_RETINA   1408
#define GAME_HEIGHT_RETINA  2112

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	//ads
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	PluginAdMob::init();
	PluginChartboost::init();
	
	class IMListener : public sdkbox::AdMobListener {
	public:
		virtual void adViewDidReceiveAd(const std::string &name) {
			log("admob adViewDidReceiveAd %s: ", name.c_str());
			//if (showText) showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
		}
		virtual void adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg) {
			log("admob adViewDidFailToReceiveAdWithError %s: ", msg.c_str());
			//if (showText) showText(StringUtils::format("%s name=%s, msg=%s", __FUNCTION__, name.c_str(), msg.c_str()));
		}
		virtual void adViewWillPresentScreen(const std::string &name) {
			log("admob adViewWillPresentScreen %s: ", name.c_str());
			//if (showText) showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
		}
		virtual void adViewDidDismissScreen(const std::string &name) {
			log("admob adViewDidDismissScreen %s: ", name.c_str());
			//if (showText) showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
		}
		virtual void adViewWillDismissScreen(const std::string &name) {
			//if (showText) showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));

			//if (name == "gameover") {
			//sdkbox::PluginAdMob::cache(kHomeBanner);
			//sdkbox::PluginAdMob::cache(kGameOverAd);
			//}
			log("admob adViewWillDismissScreen %s: ", name.c_str());
		}
		virtual void adViewWillLeaveApplication(const std::string &name) {
			//if (showText) showText(StringUtils::format("%s name=%s", __FUNCTION__, name.c_str()));
			log("admob adViewWillLeaveApplication %s: ", name.c_str());
		}
	};
	PluginAdMob::setListener(new IMListener());
#endif
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("TrianGle", cocos2d::Rect(0, 0, 320, 480));
#else
       glview = GLViewImpl::create("Triangle");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

	auto screenSize = glview->getFrameSize();
	auto designSize = cocos2d::Size(SCREEN_WIDTH_RETINA, SCREEN_HEIGHT_RETINA);
	auto gameSize = cocos2d::Size(GAME_WIDTH_RETINA, GAME_HEIGHT_RETINA);

	std::vector<std::string> searchPaths;

	if (screenSize.width <= SCREEN_WIDTH_SD) {
		searchPaths.push_back("SD");
		if (PORTRAIT) director->setContentScaleFactor(SCREEN_WIDTH_SD / designSize.width);
		else director->setContentScaleFactor(SCREEN_HEIGHT_SD / designSize.height);
		cocos2d::log("Set SD Design Res");
	}
	else if (screenSize.width <= SCREEN_WIDTH_HD) {
		searchPaths.push_back("HD");
		if (PORTRAIT) director->setContentScaleFactor(SCREEN_WIDTH_HD / designSize.width);
		else director->setContentScaleFactor(SCREEN_HEIGHT_HD / designSize.height);
		cocos2d::log("Set HD Design Res");
	}
	else {
		searchPaths.push_back("RETINA");
		if (PORTRAIT) director->setContentScaleFactor(SCREEN_WIDTH_RETINA / designSize.width);
		else director->setContentScaleFactor(SCREEN_HEIGHT_RETINA / designSize.height);
		cocos2d::log("Set RETINA Design Res");
	}

	cocos2d::FileUtils::getInstance()->setSearchPaths(searchPaths);
	if (PORTRAIT) glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT);
	else glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT);

    register_all_packages();

    // create a scene. it's an autorelease object
	auto scene = MenuGame::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}

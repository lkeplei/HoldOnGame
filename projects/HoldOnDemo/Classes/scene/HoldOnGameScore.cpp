//
//  HoldOnGameScore.cpp
//  HoldOnDemo
//
//  Created by ken on 14/7/4.
//
//

#include "HoldOnGameScore.h"
#include "HoldOnGame.h"
#include "HoldOnHome.h"
#include "KenGameUtils.h"
#include "HoldOnModel.h"

CCScene* HoldOnGameScore::scene()
{
    CCScene * scene = NULL;
    do
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        
        // 'layer' is an autorelease object
        HoldOnGameScore *layer = HoldOnGameScore::create();
        CC_BREAK_IF(! layer);
        
        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);
    
    // return the scene
    return scene;
}

bool HoldOnGameScore::init(){
    bool bRet = false;
    do
    {
        CC_BREAK_IF(! CCLayer::init());
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        //create bg
        this->addChild(KenGameUtils::createSprite("app_bg.png", ccp(320, winSize.height / 2)), -1);
        this->addChild(KenGameUtils::createSprite("score_bg.png", ccp(320, winSize.height - 421)));
        
        //create menu
        CCMenuItemImage* item1 = KenGameUtils::createMenuItemImage("score_start_game.png", "score_start_game_sec.png", this,
                                                                   menu_selector(HoldOnGameScore::menuStartGame),
                                                                   ccp(197, winSize.height - 560));
        CCMenuItemImage* item2 = KenGameUtils::createMenuItemImage("score_game_center.png", "score_game_center_sec.png", this,
                                                                   menu_selector(HoldOnGameScore::menuGameCenter),
                                                                   ccp(320, winSize.height - 560));
        CCMenuItemImage* item3 = KenGameUtils::createMenuItemImage("score_back_home.png", "score_back_home_sec.png", this,
                                                                   menu_selector(HoldOnGameScore::menuJumpToHome),
                                                                   ccp(443, winSize.height - 560));
        CCMenu* pMenu = CCMenu::create(item1, item2, item3, NULL);
        pMenu->setPosition(CCPointZero);
        this->addChild(pMenu, 1);
        
        char levelString[4];
        sprintf(levelString, "%d", HoldOnModel::shareModel()->getGameLevel());
        this->addChild(KenGameUtils::createLabelAtlas(levelString, ccp(236, winSize.height - 294), true));
        char timeString[8];
        sprintf(timeString, "%.2f", HoldOnModel::shareModel()->getGameTime());
        this->addChild(KenGameUtils::createLabelAtlas(timeString, ccp(434, winSize.height - 294), true));
        
        gameScore = KenGameUtils::createLabelAtlas("0", ccp(320, winSize.height - 404));
        this->addChild(gameScore);
        
        currentScore = 0;
        
        this->scheduleUpdate();     //开启计时刷新
        
        bRet = true;
    } while (0);
    
    return bRet;
}

#pragma mark - parent method
void HoldOnGameScore::update(float delta){
    if (currentScore < HoldOnModel::shareModel()->getGameScore()) {
        currentScore += 100;
        
        char scoreString[12];
        sprintf(scoreString, "%d", currentScore);
        gameScore->setString(scoreString);
        
        if (currentScore >= HoldOnModel::shareModel()->getGameScore()) {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            CCSprite* scoreSprite = KenGameUtils::createSprite("score_high.png", ccp(478, winSize.height - 371));
            this->addChild(scoreSprite);
            
//            CCScaleTo* scaleTo = CCScaleTo::create(0, 1.3);
//            scoreSprite->runAction(scaleTo);
//            CCScaleTo* actionScaleTo = CCScaleTo::create(1, 0.8);
//            scoreSprite->runAction(actionScaleTo);
        }
    }
}

#pragma mark - menu call back
void HoldOnGameScore::menuStartGame(CCObject* pSender){
    CCLOG("menuStartGame");
    CCDirector::sharedDirector()->replaceScene(HoldOnGame::scene());
}

void HoldOnGameScore::menuGameCenter(CCObject* pSender){
    CCLOG("menuGameCenter");
    HoldOnModel::shareModel()->showGameCenterLoader();
}

void HoldOnGameScore::menuJumpToHome(CCObject* pSender){
    currentScore = 0;
    CCDirector::sharedDirector()->replaceScene(HoldOnHome::scene());
}
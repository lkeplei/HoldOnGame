//
//  HoldOnGameScore.cpp
//  HoldOnDemo
//
//  Created by ken on 14/7/4.
//
//

#include "HoldOnGameScore.h"
#include "HoldOnGame.h"
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
        
        bRet = true;
    } while (0);
    
    return bRet;
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
    CCDirector::sharedDirector()->popScene();
}
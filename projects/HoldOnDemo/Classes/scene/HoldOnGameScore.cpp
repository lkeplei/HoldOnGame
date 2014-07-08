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

void HoldOnGameScore::initScene(){
    CCSize winSize = gameLayer->getContentSize();
    //create bg
    gameLayer->addChild(KenGameUtils::createSprite("app_bg.png", ccp(320, winSize.height / 2)), -1);
    gameLayer->addChild(KenGameUtils::createSprite("score_bg.png", ccp(320, winSize.height - 421)));
    
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
    gameLayer->addChild(pMenu, 1);
    
    char levelString[4];
    sprintf(levelString, "%d", HoldOnModel::shareModel()->getGameLevel());
    gameLayer->addChild(KenGameUtils::createLabelAtlas(levelString, ccp(236, winSize.height - 294), true));
    char timeString[8];
    sprintf(timeString, "%.2f", HoldOnModel::shareModel()->getGameTime());
    gameLayer->addChild(KenGameUtils::createLabelAtlas(timeString, ccp(434, winSize.height - 294), true));
    
    gameScore = KenGameUtils::createLabelAtlas("0", ccp(320, winSize.height - 404));
    gameLayer->addChild(gameScore);
    
    currentScore = 0;
    
    this->scheduleUpdate();     //开启计时刷新
    
    effectId = HoldOnModel::shareModel()->playEffect(KEffectTypeNumberRoll, true);    //数字滚动音效播放
}

#pragma mark - parent method
void HoldOnGameScore::showHighScore(){
    HoldOnModel::shareModel()->stopEffect(effectId);    //数字音效结束播放
    HoldOnModel::shareModel()->playEffect(KEffectTypeNewRecorder);      //新记录音效
    
    CCSize winSize = gameLayer->getContentSize();
    CCSprite* scoreSprite = KenGameUtils::createSprite("score_high.png", ccp(478, winSize.height - 371));
    gameLayer->addChild(scoreSprite);
    
//            CCScaleTo* scaleTo = CCScaleTo::create(0, 1.3);
//            scoreSprite->runAction(scaleTo);
//            CCScaleTo* actionScaleTo = CCScaleTo::create(1, 0.8);
//            scoreSprite->runAction(actionScaleTo);
}

void HoldOnGameScore::update(float delta){
    if (currentScore < HoldOnModel::shareModel()->getGameScore()) {
        currentScore += 100;
        
        char scoreString[12];
        sprintf(scoreString, "%d", currentScore);
        gameScore->setString(scoreString);
        
        if (currentScore >= HoldOnModel::shareModel()->getGameScore()) {
            showHighScore();
        }
    }
}

#pragma mark - menu call back
void HoldOnGameScore::menuStartGame(CCObject* pSender){
    HoldOnModel::shareModel()->playEffect(KEffectTypeAnJian);      //按键音效
    
    CCDirector::sharedDirector()->replaceScene(HoldOnGame::scene());
}

void HoldOnGameScore::menuGameCenter(CCObject* pSender){
    HoldOnModel::shareModel()->playEffect(KEffectTypeAnJian);      //按键音效
    
    HoldOnModel::shareModel()->showGameCenterLoader();
}

void HoldOnGameScore::menuJumpToHome(CCObject* pSender){
    HoldOnModel::shareModel()->playEffect(KEffectTypeAnJian);      //按键音效
    
    currentScore = 0;
    CCDirector::sharedDirector()->replaceScene(HoldOnHome::scene());
}
//
//  HoldOnModel.cpp
//  HoldOnDemo
//
//  Created by apple on 14/7/4.
//
//

#include "HoldOnModel.h"
#include "HoldOnConfig.h"
#include "SimpleAudioEngine.h"

#import "GameKitHelper.h"

static HoldOnModel* s_ShareModel = NULL;

HoldOnModel* HoldOnModel::shareModel(){
    if (!s_ShareModel) {
        s_ShareModel = new HoldOnModel();
    }
    
    return s_ShareModel;
}

HoldOnModel::HoldOnModel()
: gameScore(0)
, gameTime(0)
, gameLevel(1)

, levelTime(0)
, scoreTime(0)
{
    
};

HoldOnModel::~HoldOnModel(){
    
}

void HoldOnModel::showGameCenterLoader(){
    //显示排行榜
    [[GameKitHelper sharedGameKitHelper] showLeaderboard];
}

bool HoldOnModel::isLanguageEnglish(){
    NSUserDefaults* defs = [NSUserDefaults standardUserDefaults];
    NSArray* languages = [defs objectForKey:@"AppleLanguages"];
    NSString* preferredLang = [languages objectAtIndex:0];
    if ([preferredLang compare:@"zh-Hans"] == NSOrderedSame) {
        return false;
    } else {
        return true;
    }
}

float HoldOnModel::getBodyVelocity(HoldOnBodyType type){
    float velocity = KInitialVelocity;
    
    switch (type) {
        case KBodyTypeRectangleVer:{
            velocity +=  KInitialVelocity * (gameLevel - 1) * 0.15;
        }
            break;
        case KBodyTypeRectangleHor:{
            velocity +=  KInitialVelocity * (gameLevel - 1) * 0.15;
        }
            break;
        case KBodyTypeTriangle:{
            velocity +=  KInitialVelocity * (gameLevel - 1) * 0.2;
        }
            break;
        case KBodyTypeSquare:{
            velocity +=  KInitialVelocity * (gameLevel - 1) * 0.1;
        }
            break;
        default:
            break;
    }
    
    return velocity;
}

#pragma mark - music
void HoldOnModel::playEffect(HoldOnEffectType type){
    switch (type) {
        case KEffectTypeAnJian:{
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_an_jian.mp3");
        }
            break;
        case KEffectTypeCollision:{
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_collision.mp3");
        }
            break;
        case KEffectTypeGameOver:{
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_game_over.mp3");
        }
            break;
        case KEffectTypeNewRecorder:{
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_new_recorder.mp3");
        }
            break;
        case KEffectTypeNumberRoll:{
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_number_roll.mp3");
        }
            break;
        case KEffectTypeTime:{
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_time.mp3");
        }
            break;
        case KEffectTypeUpgrade:{
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_upgrade.mp3");
        }
            break;
        case KEffectTypeBackground:{
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("effect_background.mp3", true);
        }
            break;
        default:
            break;
    }
}

void HoldOnModel::closeEffect(){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void HoldOnModel::openEffect(){
    playEffect(KEffectTypeBackground);
}

#pragma mark - score
void HoldOnModel::resetLevelScore(){
    gameLevel = 1;
    gameScore = 0;
    gameTime = 0;
    
    levelTime = 0;
    scoreTime = 0;
}

void HoldOnModel::updateGameTime(float delta){
    levelTime += delta;
    scoreTime += delta;
    gameTime += delta;
    
    if (levelTime >= 10) {
        gameScore += gameLevel * 100;
        gameLevel++;
        levelTime -= 10;
        scoreTime -= 1;
    } else if (scoreTime >= 1){
        gameScore += gameLevel * 100;
        scoreTime -= 1;
    }
}
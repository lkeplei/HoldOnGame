//
//  HoldOnModel.cpp
//  HoldOnDemo
//
//  Created by apple on 14/7/4.
//
//

#include "HoldOnModel.h"
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
, gameLevel(1)
{
    
};

HoldOnModel::~HoldOnModel(){
    
}

void HoldOnModel::showGameCenterLoader(){
    //显示排行榜
    [[GameKitHelper sharedGameKitHelper] showLeaderboard];
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
}

void HoldOnModel::upgrade(){
    this->countScore();
    gameLevel++;
}

uint32_t HoldOnModel::getGameLevel(){
    return gameLevel;
}

void HoldOnModel::countScore(){
    gameScore += gameLevel * 100;
}

double HoldOnModel::getGameScore(){
    return gameScore;
}
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

#import "KENDataManager.h"
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
    if ([KENDataManager getDataByKey:KUserDefaultSetOpenVoice] == nil) {
        [KENDataManager setDataByKey:[NSNumber numberWithBool:YES] forkey:KUserDefaultSetOpenVoice];
    }
};

HoldOnModel::~HoldOnModel(){
    
}

void HoldOnModel::showGameCenterLoader(){
    //显示排行榜
    [[GameKitHelper sharedGameKitHelper] showLeaderboard];
}

void HoldOnModel::reportScore(){
    //显示排行榜
    [[GameKitHelper sharedGameKitHelper] reportScore:gameScore];
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
unsigned int HoldOnModel::playEffect(HoldOnEffectType type, bool bLoop){
    unsigned int resId = -1;
    
    if (getEffectStatus()) {
        switch (type) {
            case KEffectTypeAnJian:{
                resId = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_an_jian.mp3", bLoop);
            }
                break;
            case KEffectTypeCollision:{
                resId = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_collision.mp3", bLoop);
            }
                break;
            case KEffectTypeGameOver:{
                resId = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_game_over.mp3", bLoop);
            }
                break;
            case KEffectTypeNewRecorder:{
                resId = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_new_recorder.mp3", bLoop);
            }
                break;
            case KEffectTypeNumberRoll:{
                resId = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_number_roll.mp3", bLoop);
            }
                break;
            case KEffectTypeTime:{
                resId = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_time.mp3", bLoop);
            }
                break;
            case KEffectTypeUpgrade:{
                resId = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect_upgrade.mp3", bLoop);
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
    
    return resId;
}

void HoldOnModel::stopEffect(unsigned int nSoundId){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(nSoundId);
}

void HoldOnModel::closeEffect(){
    [KENDataManager setDataByKey:[NSNumber numberWithBool:NO] forkey:KUserDefaultSetOpenVoice];
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void HoldOnModel::openEffect(){
    [KENDataManager setDataByKey:[NSNumber numberWithBool:YES] forkey:KUserDefaultSetOpenVoice];
    
    playEffect(KEffectTypeBackground);
}

bool HoldOnModel::getEffectStatus(){
    return [[KENDataManager getDataByKey:KUserDefaultSetOpenVoice] boolValue];
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
        
        playEffect(KEffectTypeUpgrade);     //难度升级音效
    } else if (scoreTime >= 1){
        gameScore += gameLevel * 100;
        scoreTime -= 1;
    }
}
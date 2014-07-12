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
#import "MobClick.h"
#import "RootViewController.h"

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

, playGameTimes(0)
{

};

HoldOnModel::~HoldOnModel(){
    
}

void HoldOnModel::initData(){
    //初始数据
    if ([KENDataManager getDataByKey:KUserDefaultSetOpenVoice] == nil) {
        [KENDataManager setDataByKey:[NSNumber numberWithBool:YES] forkey:KUserDefaultSetOpenVoice];
    }
    
    if ([KENDataManager getDataByKey:KUserDefaultSetBestScore] == nil) {
        [KENDataManager setDataByKey:[NSNumber numberWithInt:0] forkey:KUserDefaultSetBestScore];
    }
    
    //初始友盟统计
//    [MobClick setCrashReportEnabled:NO]; // 如果不需要捕捉异常，注释掉此行
//    [MobClick setLogEnabled:YES];  // 打开友盟sdk调试，注意Release发布时需要注释掉此行,减少io消耗
    [MobClick setAppVersion:XcodeAppVersion]; //参数为NSString * 类型,自定义app版本信息，如果不设置，默认从CFBundleVersion里取
    
    if (IsPad) {
        [MobClick startWithAppkey:@"53bcd08356240bf086045566"];
    } else {
        [MobClick startWithAppkey:@"53bcd00d56240b0c1b01f0c4"];
    }
    
    //初始全屏广告
    [SysDelegate.viewController initFullMogo];
}

void HoldOnModel::showGameCenterLoader(){
    //显示排行榜
    [[GameKitHelper sharedGameKitHelper] showLeaderboard];
}

void HoldOnModel::reportScore(){
    int bestScore = [[KENDataManager getDataByKey:KUserDefaultSetBestScore] intValue];
    if (gameScore > bestScore) {
        bestScore = gameScore;
        [KENDataManager setDataByKey:[NSNumber numberWithInt:gameScore] forkey:KUserDefaultSetBestScore];
        
        [[GameKitHelper sharedGameKitHelper] whetherHighestScores:gameScore];
    } else {
        [[GameKitHelper sharedGameKitHelper] setIsNewRecord:NO];
    }
    
    [[GameKitHelper sharedGameKitHelper] reportScore:bestScore];
    
    playGameTimes++;
}

bool HoldOnModel::getIsNewRecord(){
    return [[GameKitHelper sharedGameKitHelper] isNewRecord];
}

bool HoldOnModel::isPad(){
    return IsPad;
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

#pragma mark - about Ad
void HoldOnModel::resetAd(){
    [SysDelegate.viewController resetAd];
}

void HoldOnModel::removeAd(){
    [SysDelegate.viewController removeAd];
}

void HoldOnModel::showFullAd(){
    if (playGameTimes % 3 == 1) {
        [SysDelegate.viewController showFullAd];
    }
}

void HoldOnModel::cancelFullAd(){
    [SysDelegate.viewController cancelFullAd];
}

void HoldOnModel::clearAllAd(){
    [SysDelegate.viewController clearAllAd];
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

void HoldOnModel::stopBackgroundMusic(){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
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
        gameLevel++;
        levelTime -= 10;
        scoreTime -= 1;
        
        playEffect(KEffectTypeUpgrade);     //难度升级音效
    } else if (scoreTime >= 1){
        scoreTime -= 1;
    }
}

void HoldOnModel::countGameScore(){
    for (int i = 1; i < gameLevel; i++) {
        gameScore += 10 * 100 * i;
    }

    float time = (gameTime - (gameLevel - 1) * 10);
    gameScore += gameLevel * 100 * time;
}
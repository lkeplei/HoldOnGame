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

HoldOnModel::HoldOnModel(){
    
};

HoldOnModel::~HoldOnModel(){
    
}

void HoldOnModel::showGameCenterLoader(){
    //显示排行榜
    [[GameKitHelper sharedGameKitHelper] showLeaderboard];
}

#pragma mark - music
unsigned int HoldOnModel::playEffect(const char* pszFilePath, bool bLoop){
    return CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(pszFilePath, bLoop);
}

void HoldOnModel::playBackgroundMusic(const char* pszFilePath, bool bLoop){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(pszFilePath, bLoop);
}

void HoldOnModel::stopEffect(unsigned int nSoundId){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(nSoundId);
}

void HoldOnModel::stopAllEffects(){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void HoldOnModel::stopBackgroundMusic(bool bReleaseData){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(bReleaseData);
}
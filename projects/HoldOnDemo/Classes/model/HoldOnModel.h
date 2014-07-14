//
//  HoldOnModel.h
//  HoldOnDemo
//
//  Created by ken on 14/7/4.
//
//

#ifndef  _HOLDONMODEL_H_
#define  _HOLDONMODEL_H_

#include "cocos2d.h"
#include "HoldOnModelDef.h"

class  HoldOnModel
{
public:
    HoldOnModel();
    virtual ~HoldOnModel();
    
    static HoldOnModel* shareModel(void);
    
public:
    void checkCurrentNetStatus();
    void setCurrentNetStatus(uint32_t status);
    
    void reportScore();
    void showGameCenterLoader();
    
    void initData();
    bool isLanguageEnglish();
    bool isPad();
    float getBodyVelocity(HoldOnBodyType type);
    
    //music         参考：http://www.2cto.com/kf/201306/217740.html
    unsigned int playEffect(HoldOnEffectType type, bool bLoop=false);
    void stopEffect(unsigned int nSoundId);
    void stopBackgroundMusic();
    void closeEffect();
    void openEffect();
    
    //about Ad
    void resetAd();
    void removeAd();
    void showFullAd();
    void cancelFullAd();
    void clearAllAd();
    
    void resetLevelScore();
    void updateGameTime(float delta);
    void countGameScore();
    uint16_t getGameLevel(){return gameLevel;}
    uint32_t getGameScore(){return gameScore;}
    double getGameTime(){return gameTime;}
    bool getIsNewRecord();
    bool getEffectStatus();
    
private:
    uint16_t gameLevel;
    double gameTime;
    uint32_t gameScore;
    uint16_t playGameTimes;
    
    float levelTime;
    float scoreTime;
    
    HoldOnNetType netType;
};

#endif // _HOLDONMODEL_H_
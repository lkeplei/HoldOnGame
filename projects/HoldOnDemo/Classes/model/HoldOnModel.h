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
    
    float levelTime;
    float scoreTime;
};

#endif // _HOLDONMODEL_H_
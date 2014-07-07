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
    void showGameCenterLoader();
    
    bool isLanguageEnglish();
    float getBodyVelocity(HoldOnBodyType type);
    
    //music         参考：http://www.2cto.com/kf/201306/217740.html
    void playEffect(HoldOnEffectType type);
    void closeEffect();
    void openEffect();
    
    void resetLevelScore();
    void updateGameTime(float delta);
    uint16_t getGameLevel(){return gameLevel;}
    uint32_t getGameScore(){return gameScore;}
    double getGameTime(){return gameTime;}
    
private:
    uint16_t gameLevel;
    double gameTime;
    uint32_t gameScore;
    
    float levelTime;
    float scoreTime;
};

#endif // _HOLDONMODEL_H_
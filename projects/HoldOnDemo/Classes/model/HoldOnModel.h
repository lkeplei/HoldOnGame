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
    
    //music         参考：http://www.2cto.com/kf/201306/217740.html
    void playEffect(HoldOnEffectType type);
    void closeEffect();
    void openEffect();
    
    void resetLevelScore();
    void upgrade();
    uint32_t getGameLevel();
    void countScore();
    double getGameScore();
    
private:
    uint32_t gameLevel;
    double gameScore;
};

#endif // _HOLDONMODEL_H_
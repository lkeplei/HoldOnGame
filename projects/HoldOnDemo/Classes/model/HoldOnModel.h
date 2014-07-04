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

class  HoldOnModel
{
public:
    HoldOnModel();
    virtual ~HoldOnModel();
    
    static HoldOnModel* shareModel(void);
    
public:
    void showGameCenterLoader();
    
    //music         参考：http://www.2cto.com/kf/201306/217740.html
    unsigned int playEffect(const char* pszFilePath, bool bLoop = false);
    void playBackgroundMusic(const char* pszFilePath, bool bLoop = false);
    void stopEffect(unsigned int nSoundId);
    void stopAllEffects();
    void stopBackgroundMusic(bool bReleaseData = false);
};

#endif // _HOLDONMODEL_H_
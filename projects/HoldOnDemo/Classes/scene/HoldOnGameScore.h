//
//  HoldOnGameScore.h
//  HoldOnDemo
//
//  Created by ken on 14/7/4.
//
//

#ifndef _HOLDONGAMESCORE_SCENE_H_
#define _HOLDONGAMESCORE_SCENE_H_

#include "HoldOnSceneBase.h"

class HoldOnGameScore : public HoldOnBase
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual void initScene();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();
    virtual void update(float delta);
    
    CREATE_FUNC(HoldOnGameScore);
    
public:
    // a selector callback
    void menuStartGame(CCObject* pSender);
    void menuGameCenter(CCObject* pSender);
    void menuJumpToHome(CCObject* pSender);
    
private:
    void showHighScore();
    
    CCLabelAtlas* gameScore;
    uint32_t currentScore;
    uint16_t offsetScore;
    unsigned int effectId;
};

#endif  // _HOLDONGAMESCORE_SCENE_H_
//
//  HoldOnGameScore.h
//  HoldOnDemo
//
//  Created by ken on 14/7/4.
//
//

#ifndef _HOLDONGAMESCORE_SCENE_H_
#define _HOLDONGAMESCORE_SCENE_H_

#include "cocos2d.h"

using namespace cocos2d;

class HoldOnGameScore : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();
    
    CREATE_FUNC(HoldOnGameScore);
    
public:
    // a selector callback
    void menuStartGame(CCObject* pSender);
    void menuGameCenter(CCObject* pSender);
    void menuJumpToHome(CCObject* pSender);
};

#endif  // _HOLDONGAMESCORE_SCENE_H_
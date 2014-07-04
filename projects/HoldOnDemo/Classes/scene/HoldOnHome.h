//
//  HoldOnHome.h
//  HoldOnDemo
//
//  Created by ken on 14/7/3.
//
//

#ifndef __HOLDONHOME_SCENE_H__
#define __HOLDONHOME_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class HoldOnHome : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();

    CREATE_FUNC(HoldOnHome);
    
public:
    // a selector callback
    void menuStartGame(CCObject* pSender);
    void menuGameCenter(CCObject* pSender);
    void menuAboutUs(CCObject* pSender);
    void menuOpenSound(CCObject* pSender);
    void menuCloseSound(CCObject* pSender);
};

#endif // __HOLDONHOME_SCENE_H__
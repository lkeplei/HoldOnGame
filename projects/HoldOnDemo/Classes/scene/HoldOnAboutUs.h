//
//  HoldOnAboutUs.h
//  HoldOnDemo
//
//  Created by ken on 14/7/3.
//
//

#ifndef __HOLDONABOUTUS_SCENE_H__
#define __HOLDONABOUTUS_SCENE_H__

#include "HoldOnSceneBase.h"

class HoldOnAboutUs : public HoldOnBase
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual void initScene();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();
    
    CREATE_FUNC(HoldOnAboutUs);
    
public:
    // a selector callback
    void menuBack(CCObject* pSender);
    void menuDanDan(CCObject* pSender);
    
private:
    CCSprite* dandanSprite;
    bool showDanDan;
};

#endif // __HOLDONABOUTUS_SCENE_H__
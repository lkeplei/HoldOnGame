//
//  HoldOnSceneBase.h
//  HoldOnDemo
//
//  Created by ken on 14/7/8.
//
//

#ifndef __HOLDONSCENE_BASE_H__
#define __HOLDONSCENE_BASE_H__

#include "cocos2d.h"

using namespace cocos2d;

class HoldOnBase : public CCLayer
{
public:
    virtual bool init();
    
    CREATE_FUNC(HoldOnBase);
    
public:
    virtual void initScene(){}
    
    CCLayer* gameLayer;
};

#endif // __HOLDONSCENE_BASE_H__
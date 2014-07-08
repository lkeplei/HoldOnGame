//
//  HoldOnSceneBase.cpp
//  HoldOnDemo
//
//  Created by ken on 14/7/8.
//
//

#include "HoldOnSceneBase.h"
#include "HoldOnConfig.h"
#include "HoldOnModel.h"

bool HoldOnBase::init(){
    bool bRet = false;
    do
    {
        CC_BREAK_IF(! CCLayer::init());
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        gameLayer = CCLayer::create();
        gameLayer->setPosition(ccp((winSize.width - 640) / 2, (winSize.height - 960) / 2));
        gameLayer->setContentSize(CCSize(640, 960));
        this->addChild(gameLayer, -1);
        
        if (HoldOnModel::shareModel()->isPad()) {
            gameLayer->setScale(winSize.height / 960);
        }
        
        initScene();
        
        bRet = true;
    } while (0);
    
    return bRet;
}
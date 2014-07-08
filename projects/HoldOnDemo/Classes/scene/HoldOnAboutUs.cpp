//
//  HoldOnAboutUs.cpp
//  HoldOnDemo
//
//  Created by ken on 14/7/3.
//
//

#include "HoldOnAboutUs.h"
#include "KenGameUtils.h"

CCScene* HoldOnAboutUs::scene()
{
    CCScene * scene = NULL;
    do
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        
        // 'layer' is an autorelease object
        HoldOnAboutUs *layer = HoldOnAboutUs::create();
        CC_BREAK_IF(! layer);
        
        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);
    
    // return the scene
    return scene;
}

void HoldOnAboutUs::initScene(){
    CCSize winSize = gameLayer->getContentSize();
    //create bg
    gameLayer->addChild(KenGameUtils::createSprite("app_bg.png", ccp(320, winSize.height / 2)), -1);
    
    gameLayer->addChild(KenGameUtils::createSprite("about_bg.png", ccp(320, winSize.height / 2)));
    gameLayer->addChild(KenGameUtils::createSprite("about_dandan.png", ccp(320, winSize.height - 81)));
    
    //create menu
    CCMenuItemImage* item1 = KenGameUtils::createMenuItemImage("app_back.png", "app_back_sec.png", this,
                                                               menu_selector(HoldOnAboutUs::menuBack),
                                                               ccp(50, winSize.height - 81));
    CCMenu* pMenu = CCMenu::create(item1, NULL);
    pMenu->setPosition(CCPointZero);
    gameLayer->addChild(pMenu, 1);
}

#pragma mark - menu call back
void HoldOnAboutUs::menuBack(CCObject* pSender){
    CCLOG("menu back");
    CCDirector::sharedDirector()->popScene();
}
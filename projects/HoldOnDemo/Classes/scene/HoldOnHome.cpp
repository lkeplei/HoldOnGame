//
//  HoldOnHome.cpp
//  HoldOnDemo
//
//  Created by ken on 14/7/3.
//
//

#include "HoldOnHome.h"
#include "HoldOnAboutUs.h"
#include "HoldOnGame.h"
#include "KenGameUtils.h"
#include "HoldOnModel.h"

CCScene* HoldOnHome::scene()
{
    CCScene * scene = NULL;
    do
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        
        // 'layer' is an autorelease object
        HoldOnHome *layer = HoldOnHome::create();
        CC_BREAK_IF(! layer);
        
        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);
    
    // return the scene
    return scene;
}

void HoldOnHome::initScene(){
    CCSize layerSize = gameLayer->getContentSize();
    gameLayer->addChild(KenGameUtils::createSprite("app_bg.png", ccp(320, layerSize.height / 2)), -1);
    
    if (HoldOnModel::shareModel()->isLanguageEnglish()) {
        gameLayer->addChild(KenGameUtils::createSprite("home_name_en.png", ccp(320, layerSize.height - 250)));
    } else {
        gameLayer->addChild(KenGameUtils::createSprite("home_name_ch.png", ccp(320, layerSize.height - 250)));
    }
    
    //create menu
    CCMenuItemImage* item1 = KenGameUtils::createMenuItemImage("home_start_game.png", "home_start_game_sec.png", this,
                                                               menu_selector(HoldOnHome::menuStartGame),
                                                               ccp(320, layerSize.height - 480));
    CCMenuItemImage* item2 = KenGameUtils::createMenuItemImage("home_game_center.png", "home_game_center_sec.png", this,
                                                               menu_selector(HoldOnHome::menuGameCenter),
                                                               ccp(320, layerSize.height - 696));
    CCMenuItemImage* item3 = KenGameUtils::createMenuItemImage("home_about_us.png", "home_about_us_sec.png", this,
                                                               menu_selector(HoldOnHome::menuAboutUs),
                                                               ccp(474, layerSize.height - 892));
    CCMenuItemImage* item4 = KenGameUtils::createMenuItemImage("home_sound_close.png", "home_sound_close_sec.png", this,
                                                               menu_selector(HoldOnHome::menuCloseSound),
                                                               ccp(572, layerSize.height - 892));
    item4->setVisible(!HoldOnModel::shareModel()->getEffectStatus());
    item4->setTag(2004);
    CCMenuItemImage* item5 = KenGameUtils::createMenuItemImage("home_sound_open.png", "home_sound_open_sec.png", this,
                                                               menu_selector(HoldOnHome::menuOpenSound),
                                                               ccp(572, layerSize.height - 892));
    item5->setVisible(HoldOnModel::shareModel()->getEffectStatus());
    item5->setTag(2005);
    
    CCMenu* pMenu = CCMenu::create(item1, item2, item3, item4, item5, NULL);
    pMenu->setPosition(CCPointZero);
    gameLayer->addChild(pMenu, 1, 1000);
}

#pragma mark - menu call back
void HoldOnHome::menuStartGame(CCObject* pSender){
    HoldOnModel::shareModel()->playEffect(KEffectTypeAnJian);      //按键音效
    
    CCDirector::sharedDirector()->replaceScene(HoldOnGame::scene());
}

void HoldOnHome::menuGameCenter(CCObject* pSender){
    HoldOnModel::shareModel()->playEffect(KEffectTypeAnJian);      //按键音效
    
    HoldOnModel::shareModel()->showGameCenterLoader();
}

void HoldOnHome::menuAboutUs(CCObject* pSender){
    HoldOnModel::shareModel()->playEffect(KEffectTypeAnJian);      //按键音效
    
    CCDirector::sharedDirector()->pushScene(HoldOnAboutUs::scene());
}

void HoldOnHome::menuOpenSound(CCObject* pSender){
    HoldOnModel::shareModel()->playEffect(KEffectTypeAnJian);      //按键音效
    
    CCMenu* menu = (CCMenu*)gameLayer->getChildByTag(1000);
    CCMenuItemImage* item = (CCMenuItemImage*)menu->getChildByTag(2004);
    item->setVisible(true);
    item = (CCMenuItemImage*)menu->getChildByTag(2005);
    item->setVisible(false);
    
    HoldOnModel::shareModel()->closeEffect();
}

void HoldOnHome::menuCloseSound(CCObject* pSender){
    HoldOnModel::shareModel()->playEffect(KEffectTypeAnJian);      //按键音效
    
    CCMenu* menu = (CCMenu*)gameLayer->getChildByTag(1000);
    CCMenuItemImage* item = (CCMenuItemImage*)menu->getChildByTag(2004);
    item->setVisible(false);
    item = (CCMenuItemImage*)menu->getChildByTag(2005);
    item->setVisible(true);
    
    HoldOnModel::shareModel()->openEffect();
}
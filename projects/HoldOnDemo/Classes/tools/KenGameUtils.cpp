//
//  KenGameUtils.cpp
//  HoldOnDemo
//
//  Created by ken on 14/7/3.
//
//

#include "KenGameUtils.h"

CCSprite* KenGameUtils::createSprite(const char* name, const CCPoint &position, float scaleX, float scaleY){
    CCSprite* sprite = CCSprite::create(name);
    sprite->setPosition(position);
    
    if (scaleX != 1) {
        sprite->setScaleY(scaleX);
    }
    
    if (scaleY != 1) {
        sprite->setScaleY(scaleY);
    }
   
    return sprite;
}

CCLabelTTF* KenGameUtils::createLabel(const char* string, const char* fontName, float fontSize, const CCPoint &position){
    CCLabelTTF* label = CCLabelTTF::create(string, fontName, fontSize);
    label->setPosition(position);
    return label;
}

CCLabelAtlas* KenGameUtils::createLabelAtlas(const char* string, const CCPoint& postion, bool small){
    CCLabelAtlas* atlas = NULL;
    if (small) {
        atlas = CCLabelAtlas::create(string, "app_font_small.png", 24, 34, '0');
    } else {
        atlas = CCLabelAtlas::create(string, "app_font_big.png", 54, 74, '0');
    }
    atlas->setPosition(postion);
    atlas->setAnchorPoint(ccp(0.5, 0.5));
    return atlas;
}

CCMenuItemImage* KenGameUtils::createMenuItemImage(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector, const CCPoint &position){
    CCMenuItemImage* menuItem = CCMenuItemImage::create(normalImage, selectedImage, target, selector);
    
    menuItem->setPosition(position);
    return menuItem;
}

int KenGameUtils::randomBetweenInt(int max, int min){
    return arc4random() % max + min;
}
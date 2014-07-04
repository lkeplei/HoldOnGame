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
//    sprite->setAnchorPoint(ccp(0,0));
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

CCMenuItemImage* KenGameUtils::createMenuItemImage(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector, const CCPoint &position){
    CCMenuItemImage* menuItem = CCMenuItemImage::create(normalImage, selectedImage, target, selector);
    
    menuItem->setPosition(position);
    return menuItem;
}
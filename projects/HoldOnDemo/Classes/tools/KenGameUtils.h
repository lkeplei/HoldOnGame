//
//  KenGameUtils.h
//  HoldOnDemo
//
//  Created by ken on 14/7/3.
//
//

#ifndef __KENGAMEUTILS_H__
#define __KENGAMEUTILS_H__

#include "cocos2d.h"

using namespace cocos2d;

class KenGameUtils
{
public:
    static CCSprite* createSprite(const char* name, const CCPoint &position, float scaleX=1, float scaleY=1);
    static CCLabelTTF* createLabel(const char* string, const char* fontName, float fontSize, const CCPoint &position);
    static CCLabelAtlas* createLabelAtlas(const char* string, const CCPoint& postion, bool small=false);
    static CCMenuItemImage* createMenuItemImage(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector, const CCPoint &position);
    static int randomBetweenInt(int max, int min);
};

#endif // __KENGAMEUTILS_H__
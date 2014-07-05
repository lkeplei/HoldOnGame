//
//  HoldOnGame.h
//  HoldOnDemo
//
//  Created by ken on 14/7/4.
//
//

#ifndef __HOLDONGAME_SCENE_H__
#define __HOLDONGAME_SCENE_H__

#include "cocos2d.h"
#include "GLES-Render.h"
#include "Box2D/Box2D.h"

using namespace cocos2d;

class HoldOnGame : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();
    
    CREATE_FUNC(HoldOnGame);
    
public:
    void timerAnimation();
    
    // a selector callback
    void menuBack(CCObject* pSender);

    virtual void update(float delta);
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
    
    //for debug
    virtual void draw();
private:
    void startGame();
    void gameOver();
    void createB2world();
    void createGameElement();
    
    void updateScoreLevel(float delta);
    void updateBody(float delta);
    
    b2World* gameWorld;
    CCSprite* playerBall;
};

#endif // __HOLDONGAME_SCENE_H__
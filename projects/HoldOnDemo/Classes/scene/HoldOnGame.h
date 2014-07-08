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

class HoldOnGame
: public CCLayer
, public b2ContactListener
{
public:
    typedef enum {
        KGameStatusNull = 0,
        KGameStatusGaming,
        KGameStatusOver,
    } GameStatus;
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();
    
    CREATE_FUNC(HoldOnGame);
    
public:
    void timerAnimation();
    void gameOver();
    
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
    
    //for contact
    /// Called when two fixtures begin to touch.
    virtual void BeginContact(b2Contact* contact);
    /// Called when two fixtures cease to touch.
    virtual void EndContact(b2Contact* contact);
    
private:
    void startGame();
    void createB2world();
    void createGameElement();
    
    void checkCollision();
    void updateScoreLevel(float delta);
    void updateBody(float delta);
    
    CCLabelAtlas* gameLevel;
    CCLabelAtlas* gameScore;
    b2World* gameWorld;
    CCSprite* playerBall;

    GameStatus currentGameStatus;
};

#endif // __HOLDONGAME_SCENE_H__
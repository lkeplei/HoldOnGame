//
//  HoldOnGame.cpp
//  HoldOnDemo
//
//  Created by ken on 14/7/4.
//
//

#include "HoldOnGame.h"
#include "HoldOnHome.h"
#include "KenGameUtils.h"
#include "HoldOnConfig.h"

CCScene* HoldOnGame::scene(){
    CCScene * scene = NULL;
    do{
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        
        // 'layer' is an autorelease object
        HoldOnGame *layer = HoldOnGame::create();
        CC_BREAK_IF(! layer);
        
        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);
    
    // return the scene
    return scene;
}

bool HoldOnGame::init(){
    bool bRet = false;
    do{
        CC_BREAK_IF(! CCLayer::init());
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        //create bg
        this->addChild(KenGameUtils::createSprite("app_bg.png", ccp(320, winSize.height / 2)), -1);
        this->addChild(KenGameUtils::createSprite("game_message.png", ccp(320, winSize.height - 81)));
        
        //create menu
        CCMenuItemImage* item1 = KenGameUtils::createMenuItemImage("app_back.png", "app_back_sec.png", this,
                                                                   menu_selector(HoldOnGame::menuBack),
                                                                   ccp(50, winSize.height - 81));
        CCMenu* pMenu = CCMenu::create(item1, NULL);
        pMenu->setPosition(CCPointZero);
        this->addChild(pMenu, 1);
        
        this->createGameElement();
        this->createB2world();
        
        bRet = true;
    } while (0);
    
    return bRet;
}

#pragma mark - private method
void HoldOnGame::createB2world(){
    this->scheduleUpdate();
    this->setTouchEnabled(true);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //define the world
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    gameWorld = new b2World(gravity);
    // Do we want to let bodies sleep?
    gameWorld->SetAllowSleeping(true);
    gameWorld->SetContinuousPhysics(true);
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
    
    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = gameWorld->CreateBody(&groundBodyDef);
    // Define the ground box shape.
    b2EdgeShape groundBox;
    
    b2FixtureDef birdFixtureDef;
    birdFixtureDef.density = 0;
    birdFixtureDef.filter.categoryBits = 0x0001;
    birdFixtureDef.filter.maskBits = 0x0003;
    
    // bottom
    groundBox.Set(b2Vec2(0, 0), b2Vec2(winSize.width / PTM_RATIO, 0));
    birdFixtureDef.shape = &groundBox;
    groundBody->CreateFixture(&birdFixtureDef);
    // top
    groundBox.Set(b2Vec2(0, (winSize.height - 120) / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO, (winSize.height - 120) / PTM_RATIO));
    birdFixtureDef.shape = &groundBox;
    groundBody->CreateFixture(&birdFixtureDef);
    // left
    groundBox.Set(b2Vec2(0, winSize.height / PTM_RATIO), b2Vec2(0, 0));
    birdFixtureDef.shape = &groundBox;
    groundBody->CreateFixture(&birdFixtureDef);
    // right
    groundBox.Set(b2Vec2(winSize.width / PTM_RATIO, winSize.height / PTM_RATIO), b2Vec2(winSize.width / PTM_RATIO,0));
    birdFixtureDef.shape = &groundBox;
    groundBody->CreateFixture(&birdFixtureDef);
    
    //debug
    GLESDebugDraw* m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    gameWorld->SetDebugDraw(m_debugDraw);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;
    m_debugDraw->SetFlags(flags);
}

void HoldOnGame::createGameElement(){
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    playerBall = KenGameUtils::createSprite("game_circle.png", ccp(320, winSize.height - 550));
    this->addChild(playerBall);
    this->addChild(KenGameUtils::createSprite("game_rectangle_ vertical.png", ccp(50, winSize.height - 270)), playerBall->getZOrder(), 1001);
    this->addChild(KenGameUtils::createSprite("game_rectangle_horizontal.png", ccp(510, winSize.height - 190)), playerBall->getZOrder(), 1002);
    this->addChild(KenGameUtils::createSprite("game_triangle.png", ccp(110, winSize.height - 861)), playerBall->getZOrder(), 1003);
    this->addChild(KenGameUtils::createSprite("game_square.png", ccp(535, winSize.height - 855)), playerBall->getZOrder(), 1004);
}

void HoldOnGame::startGame(){
    for (int i = 1001; i <= 1004; i++) {
        CCSprite* sprite = (CCSprite*)this->getChildByTag(i);
        if (sprite) {
            //add box2d body
            b2CircleShape birdShape;
            birdShape.m_radius = sprite->getContentSize().width / PTM_RATIO / 2;
            //CCLOG("%f",birdShape.m_radius);
            b2FixtureDef birdFixtureDef;
            birdFixtureDef.shape = &birdShape;
            birdFixtureDef.density = 0.001;
            birdFixtureDef.friction = 0;
            birdFixtureDef.restitution = 1;

            birdFixtureDef.filter.categoryBits = 0x0002;
            birdFixtureDef.filter.maskBits = 0x0001;
            
            b2BodyDef birdBodyDef;
            birdBodyDef.type = b2_dynamicBody;
            birdBodyDef.position.Set(sprite->getPositionX() / PTM_RATIO, sprite->getPositionY() / PTM_RATIO);
            b2Body* birdBody = gameWorld->CreateBody(&birdBodyDef);
            birdBody->SetUserData(sprite);
            birdBody->CreateFixture(&birdFixtureDef);
            //    birdBody->ApplyForce(b2Vec2(-5.5f * distance * cos, -5.5f * distance * sin), birdBody->GetWorldCenter());
            birdBody->SetLinearVelocity(b2Vec2(10, 10));
            birdBody->SetLinearDamping(0);
        }
    }
}

#pragma mark - parent method
void HoldOnGame::update(float delta){
    static float testNumber = 0;
    
    if (testNumber > 3 && testNumber < 9) {
        testNumber = 10;
        this->startGame();
    } else {
        testNumber += delta;
    }
    
    CCLOG("update delta = %f", delta);
    int32 velocityIterations = 8;
    int32 positionIterations = 8;
    // Instruct the world to perform a single step of simulation.
    gameWorld->Step(delta, velocityIterations, positionIterations);
    for (b2Body* b = gameWorld->GetBodyList(); b; b = b->GetNext()){
        CCSprite *myActor = (CCSprite*)b->GetUserData();
        if (myActor){
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            myActor->setPosition(CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
            myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
        }
    }
}

#pragma mark - touch
//移动操作
bool HoldOnGame::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void HoldOnGame::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint touchLocation = this->convertTouchToNodeSpace(pTouch);
//    CCPoint oldTouchLocation = pTouch->getPreviousLocationInView();
//    
//    oldTouchLocation = CCDirector::sharedDirector()->convertToGL(oldTouchLocation);
//    oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
//    
//    CCPoint translation = ccpSub(touchLocation, oldTouchLocation);
    playerBall->setPosition(touchLocation);
}

void HoldOnGame::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

void HoldOnGame::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

void HoldOnGame::registerWithTouchDispatcher(){
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

#pragma mark - menu call back
void HoldOnGame::menuBack(CCObject* pSender){
    CCLOG("menu back");
    CCDirector::sharedDirector()->replaceScene(HoldOnHome::scene());
}

#pragma mark - draw
void HoldOnGame::draw(){
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    glLineWidth(3.0f);
    gameWorld->DrawDebugData();
    
    kmGLPopMatrix();
}






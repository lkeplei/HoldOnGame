//
//  HoldOnGame.cpp
//  HoldOnDemo
//
//  Created by ken on 14/7/4.
//
//

#include "HoldOnGame.h"
#include "HoldOnHome.h"
#include "HoldOnGameScore.h"
#include "HoldOnModel.h"
#include "KenGameUtils.h"
#include "HoldOnConfig.h"

#define KGameTimerTag        2001

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
        
        currentGameStatus = KGameStatusNull;
        
        this->createGameElement();
        this->createB2world();
        this->timerAnimation();
        
        bRet = true;
    } while (0);
    
    return bRet;
}

#pragma mark - private method
void HoldOnGame::timerAnimation(){
    static int step = 1;
    CCLog("timerAnimation step = %d", step);
    
    CCSprite* timer = (CCSprite*)this->getChildByTag(KGameTimerTag);
    if (timer) {
        timer->removeFromParentAndCleanup(true);
        timer = NULL;
    }
    
    if (step == 5) {
        step = 1;
        this->startGame();
    } else {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        switch (step) {
            case 1:
                timer = KenGameUtils::createSprite("game_time_3.png", ccp(320, winSize.height - 386));
                break;
            case 2:
                timer = KenGameUtils::createSprite("game_time_2.png", ccp(320, winSize.height - 386));
                break;
            case 3:
                timer = KenGameUtils::createSprite("game_time_1.png", ccp(320, winSize.height - 386));
                break;
            case 4:
                timer = KenGameUtils::createSprite("game_go.png", ccp(320, winSize.height - 386));
                break;
            default:
                break;
        }
        this->addChild(timer, timer->getZOrder(), KGameTimerTag);
        
        CCCallFuncN* callbakc = CCCallFuncN::create(this, callfuncN_selector(HoldOnGame::timerAnimation));
        CCScaleTo* actionScaleTo = CCScaleTo::create(1, 1.3);
        CCSequence* actionSeq = CCSequence::create(actionScaleTo, callbakc, NULL);
        timer->runAction(actionSeq);
        
        step++;
    }
}

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
    
    HoldOnModel::shareModel()->resetLevelScore();

    char levelString[4];
    sprintf(levelString, "%d", HoldOnModel::shareModel()->getGameLevel());
    gameLevel = KenGameUtils::createLabelAtlas(levelString, ccp(268, winSize.height - 80), true);
    this->addChild(gameLevel);
    char timeString[8];
    sprintf(timeString, "%.2f", HoldOnModel::shareModel()->getGameTime());
    gameScore = KenGameUtils::createLabelAtlas(timeString, ccp(493, winSize.height - 80), true);
    this->addChild(gameScore);
    
    playerBall = KenGameUtils::createSprite("game_circle.png", ccp(320, winSize.height - 550));
    
    this->addChild(KenGameUtils::createSprite("game_rectangle_ vertical.png", ccp(50, winSize.height - 270)),
                   playerBall->getZOrder(), KBodyTypeRectangleVer);
    this->addChild(KenGameUtils::createSprite("game_rectangle_horizontal.png", ccp(510, winSize.height - 190)),
                   playerBall->getZOrder(), KBodyTypeRectangleHor);
    this->addChild(KenGameUtils::createSprite("game_triangle.png", ccp(110, winSize.height - 861)), playerBall->getZOrder(), KBodyTypeTriangle);
    this->addChild(KenGameUtils::createSprite("game_square.png", ccp(535, winSize.height - 855)), playerBall->getZOrder(), KBodyTypeSquare);
    
    this->addChild(playerBall);
}

void HoldOnGame::startGame(){
    currentGameStatus = KGameStatusGaming;
    HoldOnModel::shareModel()->resetLevelScore();
    
    for (int i = KBodyTypeRectangleVer; i <= KBodyTypeSquare; i++) {
        CCSprite* sprite = (CCSprite*)this->getChildByTag(i);
        if (sprite) {
            //add box2d body
            b2CircleShape birdShape;
            birdShape.m_radius = sprite->getContentSize().width / PTM_RATIO / 2;
            //CCLOG("%f",birdShape.m_radius);
            b2FixtureDef birdFixtureDef;
            birdFixtureDef.shape = &birdShape;
            birdFixtureDef.density = 0.001;             //密度
            birdFixtureDef.friction = 0;                //摩擦力
            birdFixtureDef.restitution = 1;             //弹力

            birdFixtureDef.filter.categoryBits = 0x0002;
            birdFixtureDef.filter.maskBits = 0x0001;
            
            b2BodyDef birdBodyDef;
            birdBodyDef.type = b2_dynamicBody;
            birdBodyDef.position.Set(sprite->getPositionX() / PTM_RATIO, sprite->getPositionY() / PTM_RATIO);
            b2Body* birdBody = gameWorld->CreateBody(&birdBodyDef);
            birdBody->SetUserData(sprite);
            birdBody->CreateFixture(&birdFixtureDef);
//            birdBody->ApplyForce(b2Vec2(-5.5f * distance * cos, -5.5f * distance * sin), birdBody->GetWorldCenter());
            float velocity = HoldOnModel::shareModel()->getBodyVelocity((HoldOnBodyType)i);
            birdBody->SetLinearVelocity(b2Vec2(velocity, velocity));
            birdBody->SetLinearDamping(0);
        }
    }
}

void HoldOnGame::gameOver(){
    currentGameStatus = KGameStatusOver;
    static int step = 0;
    switch (step) {
        case 0:{
            CCLog("gameOver() step = %d", step);
            CCCallFuncN* callbakc = CCCallFuncN::create(this, callfuncN_selector(HoldOnGame::gameOver));
            CCSequence* actionSeq = CCSequence::create(CCBlink::create(2, 4), callbakc, NULL);
            playerBall->runAction(actionSeq);
            
            step++;
        }
            break;
        case 1:{
            CCLog("gameOver() step = %d", step);
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            this->addChild(KenGameUtils::createSprite("game_over.png", ccp(320, winSize.height - 458)));
            
            playerBall->runAction(CCFadeOut::create(2));
            for (int i = KBodyTypeRectangleVer; i <= KBodyTypeSquare; i++) {
                CCSprite* sprite = (CCSprite*)this->getChildByTag(i);
                if (sprite) {
                    if (i == KBodyTypeSquare) {
                        CCCallFuncN* callbakc = CCCallFuncN::create(this, callfuncN_selector(HoldOnGame::gameOver));
                        CCSequence* actionSeq = CCSequence::create(CCFadeOut::create(2), callbakc, NULL);
                        sprite->runAction(actionSeq);
                    } else {
                        sprite->runAction(CCFadeOut::create(2));
                    }
                }
            }
            
            step++;
        }
            break;
        case 2:{
            CCLog("gameOver() step = %d", step);
            CCDirector::sharedDirector()->replaceScene(HoldOnGameScore::scene());
            
            step = 0;
        }
            break;
        default:
            step = 0;
            break;
    }
}

#pragma mark - parent method
void HoldOnGame::update(float delta){
    if (currentGameStatus == KGameStatusGaming) {
        this->updateScoreLevel(delta);
        this->updateBody(delta);
        this->checkCollision();
    }
}

void HoldOnGame::updateScoreLevel(float delta){
    HoldOnModel::shareModel()->updateGameTime(delta);
    
    char levelString[4];
    sprintf(levelString, "%d", HoldOnModel::shareModel()->getGameLevel());
    gameLevel->setString(levelString);

    char scoreString[8];
    sprintf(scoreString, "%.2f", HoldOnModel::shareModel()->getGameTime());
    gameScore->setString(scoreString);
}

void HoldOnGame::updateBody(float delta){
    int32 velocityIterations = 8;
    int32 positionIterations = 8;
    // Instruct the world to perform a single step of simulation.
    gameWorld->Step(delta, velocityIterations, positionIterations);
    for (b2Body* b = gameWorld->GetBodyList(); b; b = b->GetNext()){
        if (b) {
            CCSprite *myActor = (CCSprite*)b->GetUserData();
            if (myActor){
                //Synchronize the AtlasSprites position and rotation with the corresponding body
                myActor->setPosition(CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
                myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
                
                //set velocity
                float velocity = HoldOnModel::shareModel()->getBodyVelocity((HoldOnBodyType)myActor->getTag());
                b->SetLinearVelocity(b2Vec2(b->GetLinearVelocity().x > 0 ? velocity : -velocity, b->GetLinearVelocity().y > 0 ? velocity : -velocity));
            }
        }
    }
}

void HoldOnGame::checkCollision(){
    //小球与障碍物
    CCRect ballRect = playerBall->boundingBox();
    for (int i = KBodyTypeRectangleVer; i <= KBodyTypeSquare; i++) {
        CCSprite* sprite = (CCSprite*)this->getChildByTag(i);
        if (ballRect.intersectsRect(sprite->boundingBox())) {
            this->gameOver();
            break;
        }
    }
    
    
//    // 子弹跟敌机
//    CCRect bulletRec = bullet->boundingBox();
//    
//    CCObject* foeObj;
//    CCARRAY_FOREACH(this->getFoePlanes(), foeObj){
//        CCFoePlane *foePlane = (CCFoePlane *)foeObj;
//        if (bulletRec.intersectsRect(foePlane->boundingBox())  ) {
//            this->resetBullet();
//            this->fowPlaneHitAnimation(foePlane);
//            foePlane->hp = foePlane->hp - (isBigBullet?2:1);
//            if (foePlane->hp<=0) {
//                //CCLog("##### move out animation:   %d   for hp(%d)",foePlane->__id,foePlane->hp);
//                this->fowPlaneBlowupAnimation(foePlane);
//                //CCLog("##### move out begin:   %d   for hp(%d)",foePlane->__id,foePlane->hp);
//                this->getFoePlanes()->removeObject(foePlane);
//                //CCLog("##### move out end:   %d   for hp(%d)",foePlane->__id,foePlane->hp);
//            }
//        }
//    }
//    
//    // 飞机跟打飞机
//    CCRect playerRec = player->boundingBox();
//    playerRec.origin.x += 25;
//    playerRec.size.width -= 50;
//    playerRec.origin.y -= 10;
//    playerRec.size.height -= 10;
//    
//    CCObject *foeObj3;
//    CCARRAY_FOREACH(this->getFoePlanes(), foeObj3){
//        CCFoePlane *foePlane = (CCFoePlane *)foeObj3;
//        if (playerRec.intersectsRect(foePlane->boundingBox()) ) {
//            CCLog("@@@@@ shit,i was killed  by:   %d",foePlane->__id);
//            
//            this->playerBlowupAnimation();
//            this->fowPlaneBlowupAnimation(foePlane);// 同归于尽
//            this->getFoePlanes()->removeObject(foePlane);
//            this->gameOver();
//        }
//    }
//    
//    // 飞机跟道具
//    if (isVisible) {
//        CCRect playerRec1 = player->boundingBox();
//        CCRect propRec = this->getProp()->getProp()->boundingBox();
//        if (playerRec1.intersectsRect(propRec)) {
//            
//            this->getProp()->getProp()->stopAllActions();
//            this->getProp()->getProp()->removeFromParent();
//            isVisible = false;
//            
//            if (this->getProp()->type == propsTypeBullet) {
//                CCLog("========= 大力丸子");
//                isBigBullet = true;
//                isChangeBullet = true;
//            }else if (this->getProp()->type == propsTypeBomb) {
//                CCLog("========= 意念一直线，敌人死光光");
//                CCObject *foeObj4;
//                CCARRAY_FOREACH(this->getFoePlanes(), foeObj4){
//                    CCFoePlane *foePlane = (CCFoePlane *)foeObj4;
//                    this->fowPlaneBlowupAnimation(foePlane);
//                }
//                this->getFoePlanes()->removeAllObjects();
//            }
//        }
//    }
}

#pragma mark - touch
//移动操作
bool HoldOnGame::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void HoldOnGame::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (currentGameStatus == KGameStatusGaming) {
        CCPoint touchLocation = this->convertTouchToNodeSpace(pTouch);
//        CCPoint oldTouchLocation = pTouch->getPreviousLocationInView();
//
//        oldTouchLocation = CCDirector::sharedDirector()->convertToGL(oldTouchLocation);
//        oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
//
//        CCPoint translation = ccpSub(touchLocation, oldTouchLocation);
        playerBall->setPosition(touchLocation);
    }
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
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

void HoldOnGame::initScene(){
    CCSize winSize = gameLayer->getContentSize();
    //create bg
    gameLayer->addChild(KenGameUtils::createSprite("app_bg.png", ccp(320, winSize.height / 2)), -1);
    gameLayer->addChild(KenGameUtils::createSprite("game_message.png", ccp(320, winSize.height - 81)));
    
    //create menu
    CCMenuItemImage* item1 = KenGameUtils::createMenuItemImage("app_back.png", "app_back_sec.png", this,
                                                               menu_selector(HoldOnGame::menuBack),
                                                               ccp(50, winSize.height - 81));
    CCMenu* pMenu = CCMenu::create(item1, NULL);
    pMenu->setPosition(CCPointZero);
    gameLayer->addChild(pMenu, 1);
    
    currentGameStatus = KGameStatusNull;
    
    this->createGameElement();
    this->createB2world();
    this->timerAnimation();
}

#pragma mark - private method
void HoldOnGame::timerAnimation(){
    static int step = 1;
    CCLog("timerAnimation step = %d", step);
    
    CCSprite* timer = (CCSprite*)gameLayer->getChildByTag(KGameTimerTag);
    if (timer) {
        timer->removeFromParentAndCleanup(true);
        timer = NULL;
    }
    
    if (step == 5) {
        step = 1;
        this->startGame();
    } else {
        CCSize winSize = gameLayer->getContentSize();
        switch (step) {
            case 1:{
                HoldOnModel::shareModel()->playEffect(KEffectTypeTime);      //计时音效
                
                timer = KenGameUtils::createSprite("game_time_3.png", ccp(320, winSize.height - 386));
            }
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
        gameLayer->addChild(timer, timer->getZOrder(), KGameTimerTag);
        
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
    CCSize contentSize = gameLayer->getContentSize();
    
    //define the world
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    gameWorld = new b2World(gravity);
    // Do we want to let bodies sleep?
    gameWorld->SetAllowSleeping(true);
    gameWorld->SetContinuousPhysics(true);
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, (winSize.height - contentSize.height) / 2 / PTM_RATIO); // bottom-left corner
    
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
    groundBox.Set(b2Vec2(0, 0), b2Vec2(contentSize.width / PTM_RATIO, 0));
    birdFixtureDef.shape = &groundBox;
    groundBody->CreateFixture(&birdFixtureDef);
    // top
    groundBox.Set(b2Vec2(0, (contentSize.height - 140) / PTM_RATIO), b2Vec2(contentSize.width / PTM_RATIO, (contentSize.height - 140) / PTM_RATIO));
    birdFixtureDef.shape = &groundBox;
    groundBody->CreateFixture(&birdFixtureDef);
    // left
    groundBox.Set(b2Vec2(0, contentSize.height / PTM_RATIO), b2Vec2(0, 0));
    birdFixtureDef.shape = &groundBox;
    groundBody->CreateFixture(&birdFixtureDef);
    // right
    groundBox.Set(b2Vec2(contentSize.width / PTM_RATIO, contentSize.height / PTM_RATIO), b2Vec2(contentSize.width / PTM_RATIO,0));
    birdFixtureDef.shape = &groundBox;
    groundBody->CreateFixture(&birdFixtureDef);
    
    gameWorld->SetContactListener(this);    //设置刚体碰撞代理
    
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
    CCSize winSize = gameLayer->getContentSize();
    
    HoldOnModel::shareModel()->resetLevelScore();

    char levelString[4];
    sprintf(levelString, "%d", HoldOnModel::shareModel()->getGameLevel());
    gameLevel = KenGameUtils::createLabelAtlas(levelString, ccp(268, winSize.height - 80), true);
    gameLayer->addChild(gameLevel);
    char timeString[8];
    sprintf(timeString, "%.2f", HoldOnModel::shareModel()->getGameTime());
    gameScore = KenGameUtils::createLabelAtlas(timeString, ccp(493, winSize.height - 80), true);
    gameLayer->addChild(gameScore);
    
    playerBall = KenGameUtils::createSprite("game_circle.png", ccp(320, winSize.height - 550));
    
    gameLayer->addChild(KenGameUtils::createSprite("game_rectangle_ vertical.png", ccp(50, winSize.height - 270)),
                   playerBall->getZOrder(), KBodyTypeRectangleVer);
    gameLayer->addChild(KenGameUtils::createSprite("game_rectangle_horizontal.png", ccp(510, winSize.height - 190)),
                   playerBall->getZOrder(), KBodyTypeRectangleHor);
    gameLayer->addChild(KenGameUtils::createSprite("game_triangle.png", ccp(110, winSize.height - 861)), playerBall->getZOrder(), KBodyTypeTriangle);
    gameLayer->addChild(KenGameUtils::createSprite("game_square.png", ccp(535, winSize.height - 855)), playerBall->getZOrder(), KBodyTypeSquare);
    
    gameLayer->addChild(playerBall);
}

void HoldOnGame::startGame(){
    HoldOnModel::shareModel()->playEffect(KEffectTypeBackground);   //播放背景音乐
    
    currentGameStatus = KGameStatusGaming;
    HoldOnModel::shareModel()->resetLevelScore();
    
    for (int i = KBodyTypeRectangleVer; i <= KBodyTypeSquare; i++) {
        CCSprite* sprite = (CCSprite*)gameLayer->getChildByTag(i);
        if (sprite) {
            //add box2d body
            b2FixtureDef birdFixtureDef;
            
            if (i == KBodyTypeTriangle) {
//                CCRect rect = sprite->boundingBox();
//                b2PolygonShape polygonShape;
//
//                b2Vec2 vecs[] = {
//                    b2Vec2((rect.origin.x + rect.size.width / 2) / PTM_RATIO, rect.origin.y / PTM_RATIO),
//                    b2Vec2((rect.origin.x + rect.size.width) / PTM_RATIO, (rect.origin.x + rect.size.width) / PTM_RATIO),
//                    b2Vec2(rect.origin.x / PTM_RATIO, (rect.origin.x + rect.size.width) / PTM_RATIO)
//                };
//                polygonShape.Set(vecs, 3);
//                
//                birdFixtureDef.shape = &polygonShape;
                
                b2CircleShape circleShape;
                circleShape.m_radius = sprite->getContentSize().width / PTM_RATIO / 2;
                
                birdFixtureDef.shape = &circleShape;
            } else {
                CCRect rect = sprite->boundingBox();
                b2PolygonShape polygonShape;
                
                polygonShape.SetAsBox(rect.size.width / PTM_RATIO / 2, rect.size.height / PTM_RATIO / 2);
                
                birdFixtureDef.shape = &polygonShape;
            }
            
            //CCLOG("%f",birdShape.m_radius);
            birdFixtureDef.density = 0.001;             //密度
            birdFixtureDef.friction = 0;                //摩擦力
            birdFixtureDef.restitution = 1;             //弹力

            birdFixtureDef.filter.categoryBits = 0x0002;
            birdFixtureDef.filter.maskBits = 0x0001;
            
            b2BodyDef birdBodyDef;
            birdBodyDef.type = b2_dynamicBody;
            
            float heightOff = (CCDirector::sharedDirector()->getWinSize().height - gameLayer->getContentSize().height) / 2;
            
            birdBodyDef.position.Set(sprite->getPositionX() / PTM_RATIO, (sprite->getPositionY() + heightOff) / PTM_RATIO);
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
            HoldOnModel::shareModel()->countGameScore();
            HoldOnModel::shareModel()->stopBackgroundMusic();   //停止播放背景音乐
            HoldOnModel::shareModel()->playEffect(KEffectTypeGameOver);      //游戏结束音效
            
            CCCallFuncN* callbakc = CCCallFuncN::create(this, callfuncN_selector(HoldOnGame::gameOver));
            CCSequence* actionSeq = CCSequence::create(CCBlink::create(2, 4), callbakc, NULL);
            playerBall->runAction(actionSeq);
            
            HoldOnModel::shareModel()->reportScore();
            
            step++;
        }
            break;
        case 1:{
            CCLog("gameOver() step = %d", step);
            CCSize winSize = gameLayer->getContentSize();
            gameLayer->addChild(KenGameUtils::createSprite("game_over.png", ccp(320, winSize.height - 458)));
            
            playerBall->runAction(CCFadeOut::create(2));
            for (int i = KBodyTypeRectangleVer; i <= KBodyTypeSquare; i++) {
                CCSprite* sprite = (CCSprite*)gameLayer->getChildByTag(i);
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

#pragma mark - contact
void HoldOnGame::BeginContact(b2Contact* contact){
    HoldOnModel::shareModel()->playEffect(KEffectTypeCollision);      //碰撞音效
}

void HoldOnGame::EndContact(b2Contact* contact){
    B2_NOT_USED(contact);
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
                float heightOff = (CCDirector::sharedDirector()->getWinSize().height - gameLayer->getContentSize().height) / 2;
                myActor->setPosition(CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO - heightOff));
//                myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
                b->SetAngularVelocity(0);
                
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
    CCRect rect = CCRect(ballRect.origin.x + KBallOffset, ballRect.origin.y + KBallOffset,
                         ballRect.size.width - KBallOffset * 2, ballRect.size.height - KBallOffset * 2);
    for (int i = KBodyTypeRectangleVer; i <= KBodyTypeSquare; i++) {
        CCSprite* sprite = (CCSprite*)gameLayer->getChildByTag(i);
        if (rect.intersectsRect(sprite->boundingBox())) {
            bool over = true;
            
            if (sprite->getTag() == KBodyTypeTriangle) {
                CCRect sRect = sprite->boundingBox();
                CCRect rect1 = CCRect(sRect.origin.x + (sRect.size.width * 0.25) / 2 + KBallOffset, sRect.origin.y,
                                      sRect.size.width * 0.75 - KBallOffset * 2, sRect.size.height * 0.25);
                if (!rect.intersectsRect(rect1)) {
                    rect1 = CCRect(sRect.origin.x + sRect.size.width * 0.25 + KBallOffset, sRect.origin.y + sRect.size.height * 0.25,
                                   sRect.size.width * 0.5 - KBallOffset * 2, sRect.size.height * 0.25);
                    if (!rect.intersectsRect(rect1)) {
                        rect1 = CCRect(sRect.origin.x + sRect.size.width * 3 / 8 + KBallOffset, sRect.origin.y + sRect.size.height * 0.5,
                                       sRect.size.width * 0.25 - KBallOffset* 2, sRect.size.height * 0.25);
                        if (!rect.intersectsRect(rect1)) {
                            rect1 = CCRect(sRect.origin.x + sRect.size.width * 0.45, sRect.origin.y + sRect.size.height * 0.75,
                                           sRect.size.width * 0.1, sRect.size.height * 0.15);
                            if (!rect.intersectsRect(rect1)) {
                                over = false;
                            } else {
                                CCLOG("44444 rect1 = (%.1f, %.1f, %.1f, %.1f)", rect1.origin.x, rect1.origin.y, rect1.size.width, rect1.size.height);
                            }
                        } else {
                            CCLOG("333333 rect1 = (%.1f, %.1f, %.1f, %.1f)", rect1.origin.x, rect1.origin.y, rect1.size.width, rect1.size.height);
                        }
                    } else {
                        CCLOG("22222222 rect1 = (%.1f, %.1f, %.1f, %.1f)", rect1.origin.x, rect1.origin.y, rect1.size.width, rect1.size.height);
                    }
                } else {
                    CCLOG("1111111 rect1 = (%.1f, %.1f, %.1f, %.1f)", rect1.origin.x, rect1.origin.y, rect1.size.width, rect1.size.height);
                }
                
//                if (over) {
//                    CCLOG("rect = (%.1f, %.1f, %.1f, %.1f)", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
//                    CCLOG("sRect = (%.1f, %.1f, %.1f, %.1f)", sRect.origin.x, sRect.origin.y, sRect.size.width, sRect.size.height);
//                    CCLOG("game over");
////                    this->gameOver();
//                    break;
//                }
            }
            
            if (over) {
                this->gameOver();
                break;
            }
        }
    }
}

#pragma mark - touch
//移动操作
bool HoldOnGame::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void HoldOnGame::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (currentGameStatus == KGameStatusGaming) {
        CCPoint touchLocation = gameLayer->convertTouchToNodeSpace(pTouch);
//        CCPoint oldTouchLocation = pTouch->getPreviousLocationInView();
//
//        oldTouchLocation = CCDirector::sharedDirector()->convertToGL(oldTouchLocation);
//        oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
//
//        CCPoint translation = ccpSub(touchLocation, oldTouchLocation);
        CCSize ballSize = playerBall->getContentSize();
        CCSize contentSize = gameLayer->getContentSize();
        float x = playerBall->getPosition().x;
        float y = playerBall->getPosition().y;
        
        if (touchLocation.x > ballSize.width / 2 && touchLocation.x < contentSize.width - ballSize.width / 2){
            x = touchLocation.x;
        }
        
        if (touchLocation.y > ballSize.height / 2 && touchLocation.y < contentSize.height - 140 - ballSize.height / 2) {
            y = touchLocation.y;
        }
        
        playerBall->setPosition(ccp(x, y));
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
    
    return;
    
    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    glLineWidth(3.0f);
    gameWorld->DrawDebugData();
    
    kmGLPopMatrix();
}
#include "HelloWorldScene.h"

#define  PTM_RATIO 32.0
using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        
        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);
        
        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////
        
        CC_BREAK_IF(! CCLayer::init());
        CCSize winSize=CCDirector::sharedDirector()->getWinSize();
        //create bg
        CCSprite *bg=CCSprite::create("bg.png");
        ratio=winSize.height/bg->getContentSize().height;
        bg->setAnchorPoint(ccp(0,0));
        bg->setPosition(ccp(0,0));
        bg->setScaleY(ratio);
        this->addChild(bg,-2);
        //load source
        batch=CCSpriteBatchNode::create("bird2.png");
        this->addChild(batch);
        CCTexture2D *batchTexture=batch->getTexture();
        //init slingshot
        CCSprite*slingshot1=CCSprite::createWithTexture(batchTexture,CCRectMake(1,1,38,199));
        slingshot1->setScale(ratio*0.7);
        slingshot1->setPosition(ccp(90,ratio*130));
        batch->addChild(slingshot1);
        CCSprite*slingshot2=CCSprite::createWithTexture(batchTexture,CCRectMake(833,1,43,124));
        slingshot2->setScale(ratio*0.7);
        slingshot2->setPosition(ccp(78,ratio*155));
        batch->addChild(slingshot2);
        //init bird
        bird=CCSprite::createWithTexture(batchTexture,CCRectMake(1,379,31,30));
        bird->setScale(ratio);
        bird->setPosition(ccp((slingshot1->getPositionX()+slingshot2->getPositionX())/2,slingshot1->getPositionY()*1.4));
        center=bird->getPosition();
        this->addChild(bird);
        //define the world
        b2Vec2 gravity;
        gravity.Set(0.0f, -10.0f);
        world = new b2World(gravity);
        // Do we want to let bodies sleep?
        world->SetAllowSleeping(true);
        world->SetContinuousPhysics(true);
        // Define the ground body.
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0, 0); // bottom-left corner
        
        // Call the body factory which allocates memory for the ground body
        // from a pool and creates the ground box shape (also from a pool).
        // The body is also added to the world.
        b2Body* groundBody = world->CreateBody(&groundBodyDef);
        // Define the ground box shape.
        b2EdgeShape groundBox;
        CCSize s=CCSize(bg->getContentSize().width,bg->getContentSize().height*ratio);
        // bottom
        groundBox.Set(b2Vec2(0,slingshot2->getPositionY()/PTM_RATIO/2.1), b2Vec2(s.width/PTM_RATIO,slingshot2->getPositionY()/PTM_RATIO/2.1));
        groundBody->CreateFixture(&groundBox,0);
        // top
        groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
        groundBody->CreateFixture(&groundBox,0);
        // left
        groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
        groundBody->CreateFixture(&groundBox,0);
        // right
        groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
        groundBody->CreateFixture(&groundBox,0);
        GLESDebugDraw* m_debugDraw = new GLESDebugDraw( PTM_RATIO );
        world->SetDebugDraw(m_debugDraw);
        uint32 flags = 0;
        flags += b2Draw::e_shapeBit;
        flags += b2Draw::e_jointBit;
        flags += b2Draw::e_aabbBit;
        flags += b2Draw::e_pairBit;
        flags += b2Draw::e_centerOfMassBit;
        m_debugDraw->SetFlags(flags);
        //init
        r=60;//活动范围
        canFly=0;
        setTouchEnabled(true);
        scheduleUpdate();
        this->runAction(CCFollow::create(bird,CCRectMake(0,0,bg->getContentSize().width,bg->getContentSize().height*ratio)));
        bRet = true;
    } while (0);
    
    return bRet;
}

void HelloWorld::ccTouchesBegan(CCSet *touches, CCEvent *pEvent)
{
    
}

void HelloWorld::ccTouchesMoved(CCSet *touches, CCEvent *pEvent){
    CCTouch *touch=(CCTouch*)touches->anyObject();
    CCPoint pt1=touch->getLocationInView();
    pt1=CCDirector::sharedDirector()->convertToGL(pt1);
//    CCSize s=CCDirector::sharedDirector()->getWinSize();
    if(bird->boundingBox().containsPoint(pt1) == false)
        return;
    canFly=1;
    float distance = ccpDistance(pt1, center);
    if(distance > r){
        CCPoint pos;
        float cos = (pt1.x - center.x) / distance,sin=(pt1.y-center.y)/distance;
        pos.x = center.x+r*cos;
        pos.y = center.y+r*sin;
        bird->setPosition(pos);
    } else {
        bird->setPosition(pt1);
    }
}

void HelloWorld::ccTouchesEnded(CCSet *touches, CCEvent *pEvent){
    CCTouch* touch = (CCTouch*)touches->anyObject();
    CCPoint pt1 = touch->getLocationInView();
    pt1 = CCDirector::sharedDirector()->convertToGL(pt1);
    if(canFly == 0){
        return;
    } else{
        canFly = 0;
    }
    float distance = ccpDistance(pt1,center);
    float cos = (pt1.x - center.x) / distance;
    float sin = (pt1.y - center.y) / distance;
    //add box2d body
    b2CircleShape birdShape;
    birdShape.m_radius = ratio*bird->getContentSize().width / PTM_RATIO / 2;
    //CCLOG("%f",birdShape.m_radius);
    b2FixtureDef birdFixtureDef;
    birdFixtureDef.shape = &birdShape;
    birdFixtureDef.density = 0.001;
    birdFixtureDef.friction = 0;
    birdFixtureDef.restitution = 1;
    b2BodyDef birdBodyDef;
    birdBodyDef.type = b2_dynamicBody;
    birdBodyDef.position.Set(bird->getPositionX() / PTM_RATIO,bird->getPositionY() / PTM_RATIO);
    b2Body* birdBody = world->CreateBody(&birdBodyDef);
    birdBody->SetUserData(bird);
    birdBody->CreateFixture(&birdFixtureDef);
    birdBody->ApplyForce(b2Vec2(-5.5f*distance*cos,-5.5f*distance*sin),birdBody->GetWorldCenter());
    birdBody->SetLinearDamping(0);
//    birdBody->SetLinearVelocity(b2Vec2(-5.5f*distance*cos,-5.5f*distance*sin));
    
    
    
    
//    //单位映射值
//    var scale = 30;
//    //创建刚体定义数据对象
//    var bodyDef = new b2BodyDef;
//    //为静态刚体, 即不受碰撞影响
//    bodyDef.type = b2Body.b2_staticBody;
//    //位置
//    bodyDef.position.x = 640 / 2 / scale; bodyDef.position.y = 450 / scale;
//    //创建设备定义数据对象
//    var fixtureDef = new b2FixtureDef;
//    //密度
//    fixtureDef.density = 1.0;
//    //摩擦
//    fixtureDef.friction = 0.5;
//    //恢复系数
//    fixtureDef.restitution = 0.2;
//    //设备形状为多边形
//    fixtureDef.shape = new b2PolygonShape;
//    //设置为矩形 fixtureDef.shape.SetAsBox(320 / scale, 10 / scale);
//    //世界创建刚体, 刚体创建设备, 设备拥有形状
//    var body = world.CreateBody(bodyDef);
//    var fixture = body.CreateFixture(fixtureDef);
}

void HelloWorld::update(float dt){
    int32 velocityIterations = 8;
    int32 positionIterations = 8;
    // Instruct the world to perform a single step of simulation.
    world->Step(dt, velocityIterations, positionIterations);
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        CCSprite *myActor = (CCSprite*)b->GetUserData();
        if (myActor)
        {
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            myActor->setPosition(CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
            myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
        }
    }
    //CCLOG("%f",dt);
//    static float countTime=0.0f;
//    countTime+=dt;
//    if((countTime>0.08f)&&(bird->getPositionX()>center.x+r)&&(bird->getPositionY()>center.y))
//    {
//        countTime=0;
//        //show the route
//        CCSprite *route = CCSprite::createWithTexture(batch->getTexture(),CCRectMake(0,345,35,33));
//        route->setScale(0.4*ratio);
//        batch->addChild(route);
//        route->setPosition(bird->getPosition());
//    }
}

void HelloWorld::draw(){
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    glLineWidth(3.0f);
    world->DrawDebugData();
    
    kmGLPopMatrix();
    
    //draw line
    if(canFly==1){
        CHECK_GL_ERROR_DEBUG();
        
        // line: color, width, aliased
        // glLineWidth > 1 and GL_LINE_SMOOTH are not compatible
        // GL_SMOOTH_LINE_WIDTH_RANGE = (1,1) on iPhone
        //    glDisable(GL_LINE_SMOOTH);
        glLineWidth( 4.0f );
        ccDrawColor4B(0,0,0,255);
        ccDrawLine( ccpAdd(center,ccp(bird->getContentSize().width/3,0)), bird->getPosition() );
        ccDrawLine( ccpAdd(center,ccp(-bird->getContentSize().width/3,0)), bird->getPosition() );
    }
}

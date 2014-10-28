#include "HelloWorldScene.h"

USING_NS_CC;
enum{
    kMonster = 1,
    kNinja,
    kProjectile
};
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0,0));
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->setPhyWorld(scene->getPhysicsWorld());
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(Color4F(0.6f,0.6f,0.6f,1.0f))) )
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();

    initTouch();
    addNinja();
    addPhysics();
    this->schedule(schedule_selector(HelloWorld::addMonster),1.5f);
    return true;
}

void HelloWorld::initTouch(){
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::addNinja(){
    ninja = Sprite::create("player2.png");
    ninja->setPosition(Vec2(_visibleSize.width * 0.1 ,_visibleSize.height/2));
    
//    auto pysicsBody = PhysicsBody::createBox(ninja->getContentSize());
//    ninja->setPhysicsBody(pysicsBody);
//    ninja->getPhysicsBody()->setCategoryBitmask(kNinja);
//    ninja->getPhysicsBody()->setContactTestBitmask(kMonster);

    this->addChild(ninja);
}

bool HelloWorld::onTouchBegan(Touch *touch, Event * event){ 
    CCLOG("Touch");
    Vec2 touchLocation = touch->getLocation();
    Vec2 offset = ninja->getPosition() - touchLocation;
    
    float   ratio     = offset.y/offset.x;
    int     targetX   = ninja->getContentSize().width/2 + _visibleSize.width;
    int     targetY   = (targetX*ratio) + ninja->getPositionY();
    Vec2 targetPosition = Vec2(targetX,targetY);
    
    
    float angleRadians = (float)offset.y / (float)offset.x;
    float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);
    float cocosAngle = -1 * angleDegrees;
    ninja->setRotation(cocosAngle);
    
    Sprite* projectile = Sprite::create("projectile2.png");
    auto pysicsBody = PhysicsBody::createBox(projectile->getContentSize());
    projectile->setPhysicsBody(pysicsBody);
    projectile->getPhysicsBody()->setContactTestBitmask(kMonster);
    projectile->getPhysicsBody()->setCategoryBitmask(kProjectile);
    projectile->setPosition(ninja->getPosition());
    this->addChild(projectile);
    
    auto actionMove = MoveTo::create(1.5f,targetPosition);
    auto seq = Sequence::create(actionMove,
        CallFunc::create(projectile, callfunc_selector(Sprite::removeFromParent)),
        NULL);
    
    projectile->runAction(seq);
    
    return true;
}

void HelloWorld::addMonster(float dt){
    Sprite * monster = Sprite::create("monster.png");
    auto pysicsBody = PhysicsBody::createBox(monster->getContentSize());
    monster->setPhysicsBody(pysicsBody);
    monster->getPhysicsBody()->setCategoryBitmask(kMonster);
    monster->getPhysicsBody()->setContactTestBitmask(kNinja|kProjectile);
    
    int minY = monster->getContentSize().height / 2;
    int maxY = _visibleSize.height - monster->getContentSize().height / 2;
    int rangeY = maxY - minY;
    int randomY = (rand() % rangeY) + minY;
    
    monster->setPosition(Vec2(_visibleSize.width + monster->getContentSize().width/2, randomY));
    this->addChild(monster);

    int minDuration = 2.0;
    int maxDuration = 4.0;
    int rangeDuration = maxDuration - minDuration;
    int randomDuration = (rand() % rangeDuration) + minDuration;

    auto actionMove = MoveTo::create(randomDuration,Vec2(0,randomY));
    Sequence* seq = Sequence::create(actionMove,
        CallFunc::create(monster, callfunc_selector(Sprite::removeFromParent)),
        NULL);
    
    monster->runAction(seq);
}

void HelloWorld::addPhysics(){
    CCLOG("addPhysics");
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool HelloWorld::onContactBegin(cocos2d::PhysicsContact& contact){
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    
    contactLogic(a,b);
    contactLogic(b,a);
    
    return true;
}

void HelloWorld::contactLogic(PhysicsBody *a,PhysicsBody *b){
    if(a->getCategoryBitmask() == kProjectile){
        this->removeChild(a->getNode(),true);
        this->removeChild(b->getNode(),true);
    }
}
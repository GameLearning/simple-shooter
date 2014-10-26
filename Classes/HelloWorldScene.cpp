#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

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
    this->addChild(addNinja());
    this->schedule(schedule_selector(HelloWorld::addMonster),1.5f);
    return true;
}

void HelloWorld::initTouch(){
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

Sprite* HelloWorld::addNinja(){
    ninja = Sprite::create("player.png");
    ninja->setPosition(Vec2(_visibleSize.width * 0.1 ,_visibleSize.height/2));
    return ninja;
}

bool HelloWorld::onTouchBegan(Touch *touch, Event * event){ 
    Vec2 touchLocation = touch->getLocation();
    Vec2 offset = ninja->getPosition() - touchLocation;
    
    float   ratio     = offset.y/offset.x;
    int     targetX   = ninja->getContentSize().width/2 + _visibleSize.width;
    int     targetY   = (targetX*ratio) + ninja->getPositionY();
    Vec2 targetPosition = Vec2(targetX,targetY);
    
    Sprite* projectile = Sprite::create("projectile.png");
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
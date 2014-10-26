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
    Sprite* ninja = Sprite::create("player.png");
    ninja->setPosition(Vec2(_visibleSize.width * 0.1 ,_visibleSize.height/2));
    return ninja;
}

bool HelloWorld::onTouchBegan(Touch *touch, Event * event){ 
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
    CCSequence* seq = CCSequence::create(actionMove,
        CCCallFunc::create(monster, callfunc_selector(CCSprite::removeFromParent)),
        NULL);
    
    monster->runAction(seq);
}
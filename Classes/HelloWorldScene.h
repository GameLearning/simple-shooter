#pragma once
#include "cocos2d.h"

class HelloWorld : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event * event);
private:
    void initTouch();
    void addMonster(float dt);
    void addNinja();
    void addProjectile(cocos2d::Vec2 targetPosition);
    
    void addPhysics();
    void setPhyWorld(cocos2d::PhysicsWorld* world){_physicsWorld = world;}
    bool onContactBegin(cocos2d::PhysicsContact &contact);
    void contactLogic(cocos2d::PhysicsBody *a, cocos2d::PhysicsBody *b);
    
    cocos2d::Sprite* ninja;
    
    cocos2d::Size _visibleSize;
    cocos2d::PhysicsWorld *_physicsWorld;
};
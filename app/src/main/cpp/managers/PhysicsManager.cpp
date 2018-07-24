//
// Created by vadik on 06.07.2018.
//

#include "PhysicsManager.h"
#include "../helpers/Logger.h"
static const int32_t VELOCITY_ITER = 6;
static const int32_t POSITION_ITER = 2;

PhysicsManager::PhysicsManager(TimeManager &_TimerManager, GraphicsManager &_GraphicsManager) :
                _TimeManager(_TimerManager), graphicsManager(_GraphicsManager),
                _PhysicsBodies(), _PhysicsBodyCount(0), world(b2Vec2_zero), Bodies(), Locations(), BodyBoundsObj(NULL) {
    Logger::info("Creating PhysicsManager");
    world.SetContactListener(this);
}

PhysicsManager::~PhysicsManager() {
    Logger::info("Destroying PhysicsManager");
    std::vector<b2Body*>::iterator bodyIt;
    for (bodyIt = Bodies.begin(); bodyIt < Bodies.end(); ++bodyIt) {
        delete (PhysicsCollision*) (*bodyIt)->GetUserData();
    }
}

b2Body* PhysicsManager::loadBody(Location &location, uint16 pCategoty,
                                 uint16 pMask, int32_t pSizex, int32_t pSizeY, float pRestitution) {
    PhysicsCollision* physicsCollision = new PhysicsCollision();

    b2BodyDef b2BodyDef;
    b2Body* b2BodyObj;
    b2CircleShape ShapeDef; b2FixtureDef FixtureDef;
    b2BodyDef.type = b2_dynamicBody;
    b2BodyDef.userData = physicsCollision;
    b2BodyDef.awake = true;
    b2BodyDef.fixedRotation = true;

    ShapeDef.m_p = b2Vec2_zero;
    int32_t diameter = (pSizex + pSizeY) / 2;

    ShapeDef.m_radius = diameter / (2.0f * PHYSICS_SCALE);

    FixtureDef.shape = &ShapeDef;
    FixtureDef.density = 1.0f;
    FixtureDef.friction = 0.0f;
    FixtureDef.restitution = pRestitution;
    FixtureDef.filter.categoryBits = pCategoty;
    FixtureDef.filter.maskBits = pMask;
    FixtureDef.userData = physicsCollision;

    b2BodyObj = world.CreateBody(&b2BodyDef);
    b2BodyObj -> CreateFixture(&FixtureDef);
    b2BodyObj -> SetUserData(physicsCollision);
    Locations.push_back(&location);
    Bodies.push_back(b2BodyObj);

    return b2BodyObj;
}

void PhysicsManager::update() {
    int32_t size = Bodies.size();
    for (int32_t i = 0; i < size; ++i) {
        PhysicsCollision* physicsCollision = (PhysicsCollision*)(Bodies[i]->GetUserData());
        physicsCollision->collide = false;
    }
    float timeStep = _TimeManager.elapsed();
    world.Step(timeStep, VELOCITY_ITER, POSITION_ITER);

    for (int32_t i = 0; i < size; ++i) {
        const b2Vec2& position = Bodies[i]->GetPosition();
        Locations[i]->x = position.x * PHYSICS_SCALE;
        Locations[i]->y = position.y * PHYSICS_SCALE;
    }
}

void PhysicsManager::start() {
    if (BodyBoundsObj == nullptr) {
        b2BodyDef boundsBodyDef;
        b2ChainShape boundsShapeDef;
        float renderWidth = graphicsManager.getRenderWidth() / PHYSICS_SCALE;
        float renderHeight = graphicsManager.getRenderHeight() / PHYSICS_SCALE;
        b2Vec2 boundaries[4];
        boundaries[0].Set(0.0f, 0.0f);
        boundaries[1].Set(renderWidth, 0.0f);
        boundaries[2].Set(renderWidth, renderHeight);
        boundaries[3].Set(0.0f, renderHeight);
        boundsShapeDef.CreateLoop(boundaries, 4);
        BodyBoundsObj = world.CreateBody(&boundsBodyDef);
        BodyBoundsObj->CreateFixture(&boundsShapeDef, 0);

    }
}

b2MouseJoint* PhysicsManager::loadTarget(b2Body * pBody) {
    b2BodyDef emptyBodyDef;
    b2Body* emptyBody = world.CreateBody(&emptyBodyDef);
    b2MouseJointDef mouseJointDef;
    mouseJointDef.bodyA = emptyBody;
    mouseJointDef.bodyB = pBody;
    mouseJointDef.target = b2Vec2(0.0f, 0.0f);
    mouseJointDef.maxForce = 50.0f * pBody -> GetMass();
    mouseJointDef.dampingRatio = 0.15f;
    mouseJointDef.frequencyHz = 3.5f;
    return (b2MouseJoint*) world.CreateJoint(&mouseJointDef);
}

void PhysicsManager::BeginContact(b2Contact * pContact) {
    void* userDataA = pContact->GetFixtureA()->GetUserData();
    void* userDataB = pContact->GetFixtureB()->GetUserData();
    if (userDataA != nullptr && userDataB != nullptr) {
        ((PhysicsCollision*)(userDataA))->collide = true;
        ((PhysicsCollision*)(userDataA))->collide = true;
    }
}
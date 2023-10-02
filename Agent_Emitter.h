//
//  Agent_Emitter.h
//  Pursuit Game Tool 
//
//  Created by luyenhoo on 3/10/23.
//

#ifndef Agent_Emitter_h
#define Agent_Emitter_h

#pragma once

#include "ofMain.h"
#include "Shape.h"
#include "Agent.h"

//
//  Manages all Agents in a system.  You can create multiple system
//

class AgentForce {
protected:
public:
    bool applyOnce = false;
    bool applied = false;
    virtual void updateForce(Agent *) = 0;
};

typedef enum { DirectionalEmitter, RadialEmitter, SphereEmitter } EmitterType;
class AgentList {
public:
    void add(Agent);
    void addForce(AgentForce *);
    void remove(int);
    void update();
    void reset();
    int removeNear(const ofVec3f & point, float dist);
    void draw();
    vector<Agent> agents;
    vector<AgentForce *> forces;
};

class ImpulseRadialForce : public AgentForce {
    float magnitude;
public:
    ImpulseRadialForce(float magnitude);
    void updateForce(Agent *);
};

class TurbulenceForce : public AgentForce {
    ofVec3f tmin, tmax;
public:
    TurbulenceForce(const ofVec3f & min, const ofVec3f &max);
    void updateForce(Agent *);
};


//  General purpose Emitter class for emitting agents
//  This works similar to a Particle emitter
//
class Agent_Emitter : public Shape {
public:
    Agent_Emitter();
    Agent_Emitter(AgentList *s);
    ~Agent_Emitter();
    void init();
    void draw();
    void start();
    void stop();
    
    void setLifespan(int);
    void setVelocity(const glm::vec3 v);
    void setChildImage(ofImage);
    void setImage(ofImage);
    void setRate(int);
    void setLimit(int);
    void setEmitterType(EmitterType t) { type = t; }
    void setGroupSize(int s) { groupSize = s; }
    void setOneShot(bool s) { oneShot = s; }
    void update();
    
    void setSpeed(float);
    void setAttack(float);
    void setPoint(float);
    

    // virtuals - can overloaded
    virtual void moveAgent(Agent *);
    virtual void spawnAgent(float time);
    virtual bool inside(glm::vec3 p) {
        glm::vec3 s = glm::inverse(getMatrix()) * glm::vec4(p, 1);
        return (s.x > -width / 2 && s.x < width / 2 && s.y > -height / 2 && s.y < height / 2);
    }

    AgentList *sys;
    bool createdSys;
    bool oneShot;
    bool fired;
    int groupSize;
    int rate;
    int agentLimit;
    glm::vec3 velocity;
    float lifespan;
    bool started;
    float lastSpawned;
    ofImage childImage;
    ofImage image;
    bool drawable;
    bool haveChildImage;
    bool haveImage;
    float width, height;
    EmitterType type;
    
    float movespeed;
    float attack;
    float point;
};



#endif /* Agent_Emitter_h */

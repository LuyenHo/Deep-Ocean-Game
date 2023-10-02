//
//  Agent_Emitter.cpp
//  Pursuit Game Tool 
//
//  Created by luyenhoo on 3/10/23.
//

#include "Agent_Emitter.h"
#include "ofApp.h"

//  Add a Agent to the Agent System
//
void AgentList::add(Agent s) {
    agents.push_back(s);
}

void AgentList::addForce(AgentForce *f) {
    forces.push_back(f);
}

// Remove a agent from the agent system. Note that this function is not currently
// used. The typical case is that agents automatically get removed when the reach
// their lifespan.
//
void AgentList::remove(int i) {
    agents.erase(agents.begin() + i);
}

void AgentList::reset() {
    for (int i = 0; i < forces.size(); i++) {
        forces[i]->applied = false;
    }
}

//  Update the AgentSystem by checking which agents have exceeded their
//  lifespan (and deleting).  Also the agent is moved to it's next
//  location based on velocity and direction.
//
void AgentList::update() {

    if (agents.size() == 0) return;
    vector<Agent>::iterator s = agents.begin();
    vector<Agent>::iterator tmp;

    // check which agents have exceed their lifespan and delete
    // from list.  When deleting multiple objects from a vector while
    // traversing at the same time, use an iterator.
    //
    while (s != agents.end()) {
        if (s->lifespan != -1 && s->age() > s->lifespan) {
            //            cout << "deleting agent: " << s->name << endl;
            tmp = agents.erase(s);
            s = tmp;
        }
        else s++;
    }
    
    // update forces on all agent first
    //
    for (int i = 0; i < agents.size(); i++) {
        for (int k = 0; k < forces.size(); k++) {
            if (!forces[k]->applied)
                forces[k]->updateForce( &agents[i] );
        }
    }
    
    // update all forces only applied once to "applied"
    // so they are not applied again.
    //
    for (int i = 0; i < forces.size(); i++) {
        if (forces[i]->applyOnce)
            forces[i]->applied = true;
    }

    // integrate all the particles in the store
    //
//    for (int i = 0; i < agents.size(); i++)
//        agents[i].integrate();
}

int AgentList::removeNear(const ofVec3f & point, float dist) { return 0; }

//  Render all the agents
//
void AgentList::draw() {
    for (int i = 0; i < agents.size(); i++) {
        agents[i].draw();
    }
}

// Turbulence Force Field
//
TurbulenceForce::TurbulenceForce(const ofVec3f &min, const ofVec3f &max) {
    tmin = min;
    tmax = max;
}

void TurbulenceForce::updateForce(Agent * agent) {
    //
    // We are going to add a little "noise" to a particles
    // forces to achieve a more natual look to the motion
    //
    agent->force.x += ofRandom(tmin.x, tmax.x);
    agent->force.y += ofRandom(tmin.y, tmax.y);
    agent->force.z += ofRandom(tmin.z, tmax.z);
}

// Impulse Radial Force - this is a "one shot" force that
// eminates radially outward in random directions.
//
ImpulseRadialForce::ImpulseRadialForce(float magnitude) {
    this->magnitude = magnitude;
    applyOnce = true;
}

void ImpulseRadialForce::updateForce(Agent * agent) {

    // we basically create a random direction for each particle
    // the force is only added once after it is triggered.
    //
    ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
    agent->force += dir.getNormalized() * magnitude;
}

Agent_Emitter::Agent_Emitter() {
    sys = new AgentList();
    createdSys = true;
    init();
}
Agent_Emitter::Agent_Emitter(AgentList *s) {
    if (s == NULL)
    {
        cout << "fatal error: null particle system passed to ParticleEmitter()" << endl;
        ofExit();
    }
    sys = s;
    createdSys = true;
    init();
}
Agent_Emitter::~Agent_Emitter() {

    // deallocate particle system if emitter created one internally
    //
    if (createdSys) delete sys;
}

void Agent_Emitter::init() {
    lifespan = 5000;    // default milliseconds
    rate = 2;    // agents/sec
    agentLimit = 5;
    started = false;
    oneShot = false;
    fired = false;
    lastSpawned = 0;
    haveChildImage = false;
    haveImage = false;
    velocity = ofVec3f(0, 0, 0);
    drawable = false;
    width = 50;
    height = 50;
    type = DirectionalEmitter;
    groupSize = 1;
    
    movespeed = 150;
    attack = 10;
    point = 20;
}



//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Agent_Emitter::draw() {
    if (drawable) {

        if (haveImage) {
            image.draw(-image.getWidth() / 2.0 + pos.x, -image.getHeight() / 2.0 + pos.y);
        }
        else {
            ofSetColor(0, 0, 200);
            ofDrawRectangle(-width / 2 + pos.x, -height / 2 + pos.y, width, height);
        }
    }

    // draw agent system
    //
    sys->draw();
}

//  Update the Emitter. If it has been started, spawn new agents with
//  initial velocity, lifespan, birthtime.
//
void Agent_Emitter::update() {
    if (!started) return;

    float time = ofGetElapsedTimeMillis();
    int count = sys->agents.size();
    
    if (oneShot) {
        if (!fired) {

            // spawn a new particle(s)
            //
            for (int i = 0; i < groupSize; i++)
                spawnAgent(time);

            lastSpawned = time;
        }
        fired = true;
        stop();
    }
    else if ((time - lastSpawned) > (500.0 / rate) && count < agentLimit) {

        // call virtual to spawn a new agent
        //
        spawnAgent(time);
        count++;
        lastSpawned = time;
    }
    
    for (int i = 0; i < sys->agents.size(); i++) {
        moveAgent(&sys->agents[i]);
    }
    
    // update agent list
    //
    sys->update();
}

// virtual function to move agent (can be overloaded)
//
void Agent_Emitter::moveAgent(Agent *agent) {
    float framerate = ofGetFrameRate();
    float dt = 1.0 / framerate;
    agent->acceleration = agent->heading() * agent->movespeed;
    
    // linear motion
    //
    //pos += (velocity * dt);
    glm::vec3 accel = agent->acceleration;
    accel += (agent->force * 1.0 / agent->mass);
    agent->velocity += accel * dt;
    agent->velocity *= agent->damping;
    
    glm::vec3 move = glm::rotate(glm::radians(rot), glm::vec3(0, 0, 1)) * glm::vec4(agent->velocity, 1);
    agent->pos += (move * dt);
    
    if(agent->pos.x > ofGetWidth()) agent->pos.x = ofGetWidth();
    if(agent->pos.x < 0) agent->pos.x = 0;
    if(agent->pos.y > ofGetHeight()) agent->pos.y = ofGetHeight();
    if(agent->pos.y < 0) agent->pos.y = 0;

}

// virtual function to spawn agent (can be overloaded)
//
void Agent_Emitter::spawnAgent(float time) {
    Agent agent;
    switch (type) {
    case RadialEmitter:
    {
        ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), 0);
        float speed = velocity.length();
        agent.velocity = dir.getNormalized() * speed / ofGetFrameRate();
        agent.pos = pos;
    }
    break;
    case SphereEmitter:
        break;
    case DirectionalEmitter:
            agent.velocity = velocity;
            agent.pos = glm::vec3(rand() % ofGetWidth(), rand() % ofGetHeight(), 0);
        break;
    }
    if (haveChildImage) agent.setImage(childImage);
    agent.rot = ofRandom(360);
    agent.lifespan = lifespan;
    agent.birthtime = time;
    
    agent.movespeed = movespeed;
    agent.attack = attack;
    agent.point = point;
    sys->add(agent);
}

// Start/Stop the emitter.
//
void Agent_Emitter::start() {
    started = true;
    lastSpawned = ofGetElapsedTimeMillis();
}

void Agent_Emitter::stop() {
    started = false;
    fired = false;
}

void Agent_Emitter::setLifespan(int life) {
    lifespan = life;
}

void Agent_Emitter::setVelocity(const glm::vec3 v) {
    velocity = v;
}

void Agent_Emitter::setChildImage(ofImage img) {
    childImage = img;
    haveChildImage = true;
}

void Agent_Emitter::setImage(ofImage img) {
    image = img;
}

void Agent_Emitter::setRate(int r) {
    rate = r;
}

void Agent_Emitter::setLimit(int l) {
    agentLimit = l;
}

void Agent_Emitter::setSpeed(float m){
    movespeed = m;
}
void Agent_Emitter::setAttack(float a){
    attack = a;
}
void Agent_Emitter::setPoint(float p){
    point = p;
}

//
//  Agent.h
//  Pursuit Game Tool 
//
//  Created by luyenhoo on 3/10/23.
//

#ifndef Agent_h
#define Agent_h

#pragma once

#include "Shape.h"



// Base class for a Agent. Can be instantiated on it's own (deafult)
// or subclassed to create a custom agent.  Includes Shape transformations.
// If the agent has no image set on it, then a simple triangle will be drawn.
//

class Agent : public Shape {
public:
    Agent() {
        // default geometry (triangle) if no image attached.
        //
        verts.push_back(glm::vec3(-20, 20, 0));
        verts.push_back(glm::vec3(20, 20, 0));
        verts.push_back(glm::vec3(0, -20, 0));
    }
    
    // some functions for highlighting when selected
    //
    void draw() {
        if (bShowImage) {
            ofPushMatrix();
            ofSetColor(ofColor::white);
            ofMultMatrix(getMatrix());
            agentImage.draw(-agentImage.getWidth() / 2, -agentImage.getHeight() / 2.0);
            ofPopMatrix();
        }
        else
        {
            if (bHighlight) ofSetColor(ofColor::white);
            else ofSetColor(ofColor::yellow);
            ofPushMatrix();
            ofMultMatrix(getMatrix());
            ofDrawTriangle(verts[0], verts[1], verts[2]);
            ofPopMatrix();
        }
    }

    float age() {
        return (ofGetElapsedTimeMillis() - birthtime);
    }

    void setImage(ofImage img) {
        agentImage = img;
        bShowImage = true;
        width = img.getWidth();
        height = img.getHeight();
    }

    virtual bool inside(const glm::vec3 p);
    virtual void update(){
    };
    virtual void agentRot(glm::vec3 p);

    bool insideTriangle(const glm::vec3 p);
    void    integrate();

    void setSelected(bool state) { bSelected = state; }
    void setHighlight(bool state) { bHighlight = state; }
    bool isSelected() { return bSelected; }
    bool isHighlight() { return bHighlight; }


    bool bHighlight = false;
    bool bSelected = false;
    bool bShowImage = false;
    
    // Physics data
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    glm::vec3 acceleration = glm::vec3(0, 0, 0);
    glm::vec3 force = glm::vec3(0, 0, 0);
    float mass = 1.0;
    float damping = .99;
    float angularForce = 0;
    float angularVelocity = 0.0;
    float angularAcceleration = 0.0;
    bool bThrust = false;
    float prevDist = 0;
    
    glm::vec3 heading() {
        return glm::rotate(glm::radians(rot), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1);
    }
    glm::vec3 SpriteToPlayer(glm::vec3 p) { return glm::normalize(p - pos); }

    float movespeed = 150;
    float attack = 10;
    float point = 20;
    float radian = height / 2;
    
    float birthtime = 0; // elapsed time in ms
    float lifespan = -1;  //  time in ms
    string name =  "UnammedAgent";
    float width = 40;
    float height = 40;

    ofImage agentImage;

    // default verts for polyline shape if no image on agent
    //
    vector<glm::vec3> verts;
};



#endif /* Agent_h */

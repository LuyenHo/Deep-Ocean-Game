//
//  Player.h
//  Pursuit Game Tool 
//
//  Created by luyenhoo on 3/10/23.
//

#ifndef Player_h
#define Player_h

#pragma once

#include "Shape.h"

// Base class for a Player. Can be instantiated on it's own (default)
// or subclassed to create a custom player.  Includes Shape transformations.
// If the player has no image set on it, then a simple triangle will be drawn.
//

class Player : public Shape {
public:
    Player() {
        // default geometry (triangle) if no image attached.
        //
        pos = glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0);
        moveSpeed = 200.0;
        playerLevel = 0;
        levelPoint = 0.0;
        
        verts.push_back(glm::vec3( -20, 20, 0));
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
            playerImage.draw(-playerImage.getWidth() / 2, -playerImage.getHeight() / 2.0);
            ofPopMatrix();
        }
        else
        {
            if (bHighlight) ofSetColor(ofColor::white);
            else ofSetColor(ofColor::mediumVioletRed);
            ofPushMatrix();
            ofMultMatrix(getMatrix());
            ofDrawTriangle(verts[0], verts[1], verts[2]);
            ofPopMatrix();
        }
    }

    void setImage(ofImage img) {
        playerImage = img;
        bShowImage = true;
        width = img.getWidth();
        height = img.getHeight();
    }

    virtual bool inside(const glm::vec3 p);
    virtual void update();

    bool insideTriangle(const glm::vec3 p);
    
    void integrate();
    void resetPlayer(float);
    void setSpeed(float);
    void setPoint(float);
    
    void setSelected(bool state) { bSelected = state; }
    void setHighlight(bool state) { bHighlight = state; }
    bool isSelected() { return bSelected; }
    bool isHighlight() { return bHighlight; }


    bool bHighlight = false;
    bool bSelected = false;
    bool bShowImage = false;
    bool start = false;

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
    
//    glm::vec3 heading() {
//        glm::vec3 initialHeading = glm::vec3(0, -1, 0);    // heading at start
//        glm::mat4 Mrot = glm::rotate(glm::mat4(1.0), glm::radians(rot), glm::vec3(0, 0, 1));
//        glm::vec3 h = Mrot * glm::vec4(initialHeading, 1);
//        return glm::normalize(h);
//        return glm::rotate(glm::radians(rot), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1);
//    }
    
    //Player data
    int playerLevel = 0;
    float levelPoint = 0.0;
    float exp = 0;
    float energy = 0.0;
    float moveSpeed = 0.0;
    
    float timer;
    float timerBegin;
    float score;
    string name =  "UnammedPlayer";
    float width = 40;
    float height = 40;

    ofImage playerImage;

    // default verts for polyline shape if no image on player
    //
    vector<glm::vec3> verts;
};



#endif /* Player_h */

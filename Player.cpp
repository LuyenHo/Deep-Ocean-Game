//
//  Player.cpp
//  Pursuit Game Tool 
//
//  Created by luyenhoo on 3/10/23.
//

#include "Player.h"
#include "ofApp.h"



//
    // inside - check if point is inside player (can be an image or TriangleShape if no image)
    //
bool Player::inside(const glm::vec3 p) {

    // if there is no player image attached, then just use triangle case.
    //
    if (!bShowImage) {
        return insideTriangle (p);
    }

    // if player image attached, then first check if point is inside bounds of image
    // in object space.  If point is inside bounds, then make sure the point is in
    // opaque part of image.
    //
    glm::vec3 s = glm::inverse(getMatrix()) * glm::vec4(p, 1);
    int w = playerImage.getWidth();
    int h = playerImage.getHeight();
    if (s.x > -w / 2 && s.x < w / 2 && s.y > -h / 2 && s.y < h / 2) {
        int x = s.x + w / 2;
        int y = s.y + h / 2;
        ofColor color = playerImage.getColor(x, y);
        return (color.a != 0);   // check if color is opaque (not the transparent background)
    }
    else return false;
}


bool Player::insideTriangle(const glm::vec3 p) {

    // transform the screen space point p to the triangle's local
    // oordinate system  (object space);  this will take into account any
    // rotation, translation or scale on the object.
    //
    glm::vec4 p2 = glm::inverse(getMatrix()) * glm::vec4(p, 1);

    glm::vec3 v1 = glm::normalize(verts[0] - p2);
    glm::vec3 v2 = glm::normalize(verts[1] - p2);
    glm::vec3 v3 = glm::normalize(verts[2] - p2);
    float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
    float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
    float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
    if (a1 < 0 && a2 < 0 && a3 < 0) return true;
    else return false;

}

void Player::resetPlayer(float e) {
    energy = e * 100;
    playerLevel = 0;
    exp = 0;
    timerBegin = ofGetElapsedTimeMillis();
    pos = glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0);
    rot = 0;
    bThrust = false;
    angularVelocity = 0;
    velocity = glm::vec3(0, 0, 0);
}
void Player::setSpeed(float s){
    moveSpeed = s;
}
void Player::setPoint(float p){
    levelPoint = p;
}

void Player::update() {
    float time = ofGetElapsedTimeMillis();
    timer = (time - timerBegin )/ 1000.0;

    if (start) integrate();
    force = glm::vec3(0, 0, 0);
    
    if(pos.x > ofGetWidth()) pos.x = ofGetWidth();
    if(pos.x < 0) pos.x = 0;
    if(pos.y > ofGetHeight()) pos.y = ofGetHeight();
    if(pos.y < 0) pos.y = 0;
}
void Player::integrate() {
    float framerate = ofGetFrameRate();
    float dt = 1.0 / framerate;

    // linear motion
    //
    //pos += (velocity * dt);
    glm::vec3 accel = acceleration;
    accel += (force * 1.0 / mass);
    velocity += accel * dt;
    velocity *= damping;
    
    glm::vec3 move = glm::rotate(glm::radians(rot), glm::vec3(0, 0, 1)) * glm::vec4(velocity, 1);
    pos += (move * dt);

    // angular motion
    //
    rot += (angularVelocity * dt);
    float a = angularAcceleration;;
    a += (angularForce * 1.0 / mass);
    angularVelocity += a * dt;
    angularVelocity *= damping;
}


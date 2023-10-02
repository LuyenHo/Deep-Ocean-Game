//
//  Agent.cpp
//  Pursuit Game Tool 
//
//  Created by luyenhoo on 3/10/23.
//

#include "Agent.h"



//
    // inside - check if point is inside player (can be an image or TriangleShape if no image)
    //
bool Agent::inside(const glm::vec3 p) {

    // if there is no agent image attached, then just use triangle case.
    //
    if (!bShowImage) {
        return insideTriangle (p);
    }

    // if agent image attached, then first check if point is inside bounds of image
    // in object space.  If point is inside bounds, then make sure the point is in
    // opaque part of image.
    //
    glm::vec3 s = glm::inverse(getMatrix()) * glm::vec4(p, 1);
    int w = agentImage.getWidth();
    int h = agentImage.getHeight();
    if (s.x > -w / 2 && s.x < w / 2 && s.y > -h / 2 && s.y < h / 2) {
        int x = s.x + w / 2;
        int y = s.y + h / 2;
        ofColor color = agentImage.getColor(x, y);
        return (color.a != 0);   // check if color is opaque (not the transparent background)
    }
    else return false;
}


bool Agent::insideTriangle(const glm::vec3 p) {

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
void Agent::agentRot(glm::vec3 p){
    float framerate = ofGetFrameRate();
    float dt = 1.0 / framerate;
    
    float a = angularAcceleration;;
    a += (angularForce * 1.0 / mass);
    angularVelocity += a * dt;
    angularVelocity *= damping;
    
    glm::vec3 h = heading();
    glm::vec3 v = SpriteToPlayer(p);
    float dotp = glm::dot(h, v);
    float eps = 0.0005;

    // if player and  sprite are lined up then dot product == 1.0,
    // if not, then "course" needs to be corrected by rotating the player
    //
    if (dotp != (1.0 - eps)) angularAcceleration = 200;
    else angularAcceleration = 0.0;
    if (dotp < (1.0 - eps)) {
        glm::vec3 crossp = glm::cross(h, v);
        if (crossp.z > 0.0) rot += (angularVelocity * dt);
        else rot -= (angularVelocity * dt);
    }
}

void Agent::integrate() {
    float framerate = ofGetFrameRate();
    float dt = 1.0 / framerate;
    acceleration = heading() * movespeed;
    
    // linear motion
    //
    //pos += (velocity * dt);
    glm::vec3 accel = acceleration;
    accel += (force * 1.0 / mass);
    velocity += accel * dt;
    velocity *= damping;
    
    glm::vec3 move = glm::rotate(glm::radians(rot), glm::vec3(0, 0, 1)) * glm::vec4(velocity, 1);
    pos += (move * dt);
    
//    if(pos.x > ofGetWidth()) pos.x = ofGetWidth();
//    if(pos.x < 0) pos.x = 0;
//    if(pos.y > ofGetHeight()) pos.y = ofGetHeight();
//    if(pos.y < 0) pos.y = 0;
}

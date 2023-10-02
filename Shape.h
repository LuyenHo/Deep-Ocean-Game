//
//  Shape.h
//  Pursuit Game Tool 
//
//  Created by luyenhoo on 3/9/23.
//

#ifndef Shape_h
#define Shape_h

#pragma once

#include "ofMain.h"


// Basic Shape class supporting matrix transformations and drawing.
//
//
class Shape {
public:
    Shape() {}
    virtual void draw() {

        // draw a box by defaultd if not overridden
        //
        ofPushMatrix();
        ofMultMatrix(getMatrix());
        ofDrawBox(defaultSize);
        ofPopMatrix();
    }

    virtual bool inside(glm::vec3 p) {
        return false;
    }

    glm::mat4 getMatrix() {
        glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(pos));
        glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(rot), glm::vec3(0, 0, 1));
        glm::mat4 S = glm::scale(glm::mat4(1.0), scale);      // added this scale if you want to change size of object
        return T*R*S;
    }
    glm::vec3 pos;
    float rot = 0.0;    // degrees
    glm::vec3 scale = glm::vec3(1, 1, 1);
    float defaultSize = 20.0;

};


#endif /* Shape_h */

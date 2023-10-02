//
//  Player_Emitter.h
//  Deep Ocean Life
//
//  Created by luyenhoo on 4/7/23.
//

#ifndef Player_Emitter_h
#define Player_Emitter_h

#include "ofMain.h"
#include "Agent_Emitter.h"


class Player_Emitter : public Agent_Emitter {
    void spawnAgent(float time) {
        Agent agent;
        if (haveChildImage) agent.setImage(childImage);
        agent.attack = 10.0;
        agent.velocity = velocity;
        agent.lifespan = lifespan;
        agent.movespeed = 500;
        agent.pos = pos;
        agent.birthtime = time;
        sys->add(agent);
    }
    
    void moveAgent(Agent *bulet) {
        bulet->pos += bulet->movespeed * (bulet->heading() / ofGetFrameRate());
    }
    public:
    bool collision(Agent agent, Agent bullet ){
        if (bullet.bShowImage){
            bullet.radian = (bullet.scale.x/2) * (bullet.height / 2);
            float dis = glm::distance(bullet.pos, agent.pos);
            if(dis < (bullet.radian + agent.radian)) {
                return true;
            }
        }
        else {
            glm::mat4 N = bullet.getMatrix();
            glm::vec3 a0 = N * glm::vec4(bullet.verts[0], 1);
            glm::vec3 a1 = N * glm::vec4(bullet.verts[1], 1);
            glm::vec3 a2 = N * glm::vec4(bullet.verts[2], 1);
            if(agent.inside(a0) || agent.inside(a1) || agent.inside(a2)) {
                return true;
            }
        }
            
        return false;
    }
};


#endif /* Player_Emitter_h */

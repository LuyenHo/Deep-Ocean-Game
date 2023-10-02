#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    //player.scale = glm::vec3(.1, .1, .1);
    font1.load("verdana.ttf", 50, true, true);
    font1.setLineHeight(550.0f);
    font1.setLetterSpacing(1.037);
    
    font2.load("verdana.ttf", 25, true, true);
    font2.setLineHeight(25.0f);
    font2.setLetterSpacing(1.035);
    
    ofSetVerticalSync(true);

    // create an image for agents being spawned by emitter
    //
    if (defaultImage.load("images/Shark.png")) {
        imageLoaded = true;
    }
    else {
        cout << "Can't open image file" << endl;
        ofExit();
    }
    if (buble.load("images/bubble.png")) {
        imageLoaded = true;
    }
    else {
        cout << "Can't open image file" << endl;
        ofExit();
    }
    if (playerImage.load("images/Octopus.png")) {
        imageLoaded = true;
    }
    else {
        cout << "Can't open image file" << endl;
        ofExit();
    }
    
    if (backGround_1.load("images/Ocean_background.png")) {
        imageLoaded = true;
    }
    else {
        cout << "Can't open image file" << endl;
        ofExit();
    }
    if (backGround_2.load("images/Ocean_Background_blur.png")) {
        imageLoaded = true;
    }
    else {
        cout << "Can't open image file" << endl;
        ofExit();
    }
    

    // create an array of emitters and set their position;
    //
    
    emitter = new Agent_Emitter();

    emitter->pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
    emitter->setChildImage(defaultImage);
    //emitter->start();

    playEmitter = new Player_Emitter();
    playEmitter->setChildImage(buble);
    
    player.setImage(playerImage);
    
    glm::vec3 gui_pos = glm::vec3(player.pos.x - 100, player.pos.y - 50, 0);
    gui.setPosition(gui_pos);
    gui.setup();
    gui.add(difficult.setup("Difficult Level", 1, 1, 3));
    gui.add(energy.setup("Player Life", 5, 1, 10));
    gui.add(playerScale.setup("Player Scale", .5, .1, 2.0));
    gui.add(customPlayer.setup("Custom Player", true));
    
    gui.add(rate.setup("Agents respawn rate", 2, 1, 15));
    gui.add(life.setup("Agent life", 5, 0, 10));
    gui.add(agentLimit.setup("Agent limit", 5, 0, 30));
    gui.add(agentScale.setup("Agent Scale", 1.0, .5, 3.0));
    gui.add(customAgent.setup("Custom Agent", true));

    bHide = false;
    
    // set up the emitter forces
    //
    turbForce = new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20));
    radialForce = new ImpulseRadialForce(1000.0);

    explosion = new Agent_Emitter();
    
    explosion->setChildImage(buble);

    explosion->sys->addForce(turbForce);
    explosion->sys->addForce(radialForce);

    explosion->setVelocity(ofVec3f(0, 0, 0));
    explosion->setOneShot(true);
    explosion->setEmitterType(RadialEmitter);
    explosion->setGroupSize(50);
    
    //  setup sounds
    moving.load("sounds/swim-44183.mp3");
    moving.setLoop(true);
    shoting.load("sounds/soap-bubbles-pop-96873.mp3");
    shoting.setLoop(true);
    explosing.load("sounds/bubble_broken.mp3");
    attacking.load("sounds/punch-140236.mp3");
    

}

//--------------------------------------------------------------
void ofApp::gameOver() {
    gameStop = true;
    shoting.stop();
    gameStart = false;
    emitter->sys->agents.clear();
    emitter->stop();
    playEmitter->sys->agents.clear();
    playEmitter->stop();
    rate = 3;
    agentLimit = 5;
    agentScale = 1;
    playerScale = .5;
    player.score = player.timer;
    ofResetElapsedTimeCounter();
}

//--------------------------------------------------------------
//update difficult level
void ofApp::setDiff (int d) {
    if (d == 1) {
        emitter->setAttack(10);
        emitter->setPoint(15);
        emitter->setSpeed(150);
        player.setSpeed(150);
        player.setPoint(100);
    }
    if (d == 2) {
        emitter->setAttack(20);
        emitter->setPoint(20);
        emitter->setSpeed(200);
        player.setSpeed(150);
        player.setPoint(150);
    }
    if (d == 3) {
        emitter->setAttack(30);
        emitter->setPoint(25);
        emitter->setSpeed(250);
        player.setSpeed(150);
        player.setPoint(200);
    }
}

void ofApp::kill_agent(Agent agent){
    explosing.play();
    explosion->pos = agent.pos;
    explosion->sys->reset();
    explosion->start();
    player.exp += agent.point;
}

//--------------------------------------------------------------
void ofApp::update() {
    if(gameStart){
        if (player.bThrust && !moving.isPlaying()) {
            moving.play();
        }
        else if (!player.bThrust && moving.isPlaying()) {
            moving.stop();
        }
        
        float sc_player = playerScale;
        float agent_scale = agentScale;
        if (customPlayer) player.scale = glm::vec3(sc_player, sc_player, sc_player);
        else player.scale = glm::vec3(2 * sc_player, 2 * sc_player, 2 * sc_player);
        player.update();
        player.bShowImage = customPlayer;
        player.start = gameStart;
        
        //update player attack
        playEmitter->pos = player.pos;
        playEmitter->setRate(2);
        playEmitter->setLimit(20);
        playEmitter->setLifespan(5 * 1000);
        playEmitter->started = gameStart;
        playEmitter->update();
        
        for (int i = 0; i < playEmitter->sys->agents.size(); i++) {
            if(playEmitter->inside(playEmitter->sys->agents[i].pos)){
                playEmitter->sys->agents[i].rot = player.rot;
            }
            playEmitter->sys->agents[i].scale = glm::vec3(sc_player/2, sc_player/2, sc_player/2);
        }
        
        //explosion effect
        explosion->setLifespan(1 * 1000);
        explosion->setRate(1);
        explosion->update();
        
        for (int i = 0; i < explosion->sys->agents.size(); i++) {
            explosion->sys->agents[i].scale = glm::vec3(.3, .3, .3);
            explosion->sys->agents[i].integrate();
        }

        //update agents emitter
        emitter->setRate(rate);
        emitter->setLimit(agentLimit);
        emitter->setLifespan(life * 1000);    // convert to milliseconds
        emitter->haveChildImage = customAgent;
        emitter->started = gameStart;
        emitter->update();
        
        for (int i = 0; i < emitter->sys->agents.size(); i++) {
            
            // Get values from sliders and update agents dynamically
            //
            emitter->sys->agents[i].scale = glm::vec3(agent_scale, agent_scale, agent_scale);
            emitter->sys->agents[i].bShowImage = customAgent;
            emitter->sys->agents[i].agentRot(player.pos);
            
            //detect collision between agent and player
            float agentRadian;
            if (emitter->sys->agents[i].bShowImage){
                agentRadian = agentScale * (emitter->sys->agents[i].height / 2);
                emitter->sys->agents[i].radian = agentRadian;
                glm::vec3 in_agent = emitter->sys->agents[i].pos + glm::normalize(emitter->sys->agents[i].heading()) * agentRadian;
                if(player.inside(in_agent)) {
                    coll = true;
                }
            }
            else {
                glm::mat4 M = emitter->sys->agents[i].getMatrix();
                glm::vec3 v0 = M * glm::vec4(emitter->sys->agents[i].verts[0], 1);
                glm::vec3 v1 = M * glm::vec4(emitter->sys->agents[i].verts[1], 1);
                glm::vec3 v2 = M * glm::vec4(emitter->sys->agents[i].verts[2], 1);
                
                if(player.inside(v0) || player.inside(v1) || player.inside(v2)) {
                    coll = true;
                }
                emitter->sys->agents[i].radian = glm::distance(emitter->sys->agents[i].pos, v0);
            }
            //update player status
            if(coll) {
                coll = false;
                attacking.play();
                emitter->sys->agents[i].lifespan = 0;
                player.energy -= emitter->sys->agents[i].attack;
                if (player.energy <= 0) {
                    gameOver();
                    moving.stop();
                }
            }
            //detect collision between agent and player attact, update player
            for (int j = 0; j < playEmitter->sys->agents.size(); j++){
                if(playEmitter->collision(emitter->sys->agents[i], playEmitter->sys->agents[j])){
                    kill_agent(emitter->sys->agents[i]);
                    playEmitter->sys->agents[j].lifespan = 0;
                    emitter->sys->agents[i].lifespan = 0;
                    if(player.exp >= player.levelPoint) {
                        player.playerLevel++;
                        player.energy += 100;
                        player.setPoint(player.levelPoint * 2);
                    }
                }
            }
        }
        
        //increase the difficult level overtime
        if (player.timer - level > 10) {
            emitter->attack += 2;
            rate = rate + difficult;
            if(rate > 15) rate = 15;
            agentLimit = agentLimit + difficult * 2;
            if(agentLimit > 30) agentLimit = 30;
            level = player.timer;
        }
    }
    
    
    //setup gui panel position before and during game start
    if (!gameStart) {
        glm::vec3 gui_pos = glm::vec3(ofGetWidth() / 2 - 100, ofGetHeight() / 2, 0);
        gui.setPosition(gui_pos);
        setDiff(difficult);
    }
    else {
        gui.setPosition(glm::vec3(10, 10, 0));
    }
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    if (!gameStart) {
        ofSetColor(ofColor::white);
        backGround_2.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
   
    if (gameStart){
        ofSetColor(ofColor::white);
        backGround_1.draw(0, 0, ofGetWidth(), ofGetHeight());
        emitter->draw();
        playEmitter->draw();
        explosion->draw();
        player.draw();
        
        ofSetColor(ofColor::white, 150);
        ofDrawRectangle(3*ofGetWidth()/10 - 20, 0, 4*ofGetWidth()/5, 30);
        ofSetColor(ofColor::black);
        ofDrawBitmapString("Level: " + ofToString(player.playerLevel), 3*ofGetWidth()/10, 20);
        ofDrawBitmapString("Life: " + ofToString(player.energy), 5*ofGetWidth()/10, 20);
        int min = player.timer / 60;
        int sec = player.timer - min * 60;
        ofDrawBitmapString("Timer: " + ofToString(min) + " min " + ofToString(sec) + "s", 7*ofGetWidth()/10, 20);
        ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 9*ofGetWidth()/10, 20);
    }
    if (!gameStart && !gameStop) {
        ofSetColor(ofColor::orangeRed);
        font1.drawString("Deep Ocean Life", ofGetWidth() / 2 - 250, ofGetHeight() / 2 - 100);
        ofSetColor(ofColor::white);
        font2.drawString("Press Space To Start", ofGetWidth() / 2 - 150, ofGetHeight() / 2 - 50);
    }

    if (!gameStart && gameStop) {
        ofSetColor(ofColor::orangeRed);
        font1.drawString("Game Over", ofGetWidth() / 2 - 175, ofGetHeight() / 2 - 150);
        int min = player.score / 60;
        int sec = player.score - min * 60;
        ofSetColor(ofColor::whiteSmoke);
        font2.drawString("Time: " + ofToString(min) + " min " + ofToString(sec) + "s", ofGetWidth() / 2 - 100, ofGetHeight() / 2 - 100);
        ofSetColor(ofColor::white);
        font2.drawString("Press Space To Restart", ofGetWidth() / 2 - 175, ofGetHeight() / 2 - 50);
    }
    
    if (!bHide) {
        gui.draw();
    }
}


//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//    cout << "mouse( " << x << "," << y << ")" << endl;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

void ofApp::keyPressed(int key) {
    switch (key) {
    case 'C':
    case 'c':
        break;
    case 'F':
    case 'f':
        ofToggleFullscreen();
        break;
    case 'H':
    case 'h':
        bHide = !bHide;
        break;
    case 'r':
        break;
    case 's':
        break;
    case 'u':
        break;
    case OF_KEY_ALT:
        break;
    case OF_KEY_CONTROL:
        break;
    case OF_KEY_SHIFT:
        break;
    case OF_KEY_DEL:
        break;
    case ' ':
        gameStart = true;
        shoting.play();
        gameStop = false;
        if (player.energy <= 0){
            emitter->start();
            playEmitter->start();
            player.resetPlayer(energy);
            level = 0;
        }
        break;
    case OF_KEY_LEFT:
        //player.bThrust = true;
        player.angularAcceleration = -200.0;
        break;
    case OF_KEY_RIGHT:
        //player.bThrust = true;
        player.angularAcceleration = 200.0;
        break;
    case OF_KEY_UP:
        player.bThrust = true;
        //    engineSound.play();
        player.angularVelocity = 0;
        player.acceleration = glm::vec3(0, -player.moveSpeed, 0);
        break;
    case OF_KEY_DOWN:
        player.bThrust = true;
        player.angularVelocity = 0;
        player.acceleration = glm::vec3(0, player.moveSpeed, 0);
        break;
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    switch (key) {
    case OF_KEY_LEFT:
        //player.bThrust = false;
        player.angularAcceleration = 0;
        break;
    case OF_KEY_RIGHT:
        //player.bThrust = false;
        player.angularAcceleration = 0;
        break;
    case OF_KEY_UP:
        player.bThrust = false;
        player.acceleration = glm::vec3(0, 0, 0);
        break;
    case OF_KEY_DOWN:
        player.bThrust = false;
        player.acceleration = glm::vec3(0, 0, 0);
        break;
    case OF_KEY_ALT:
        break;
    case OF_KEY_CONTROL:
        break;
    case OF_KEY_SHIFT:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}


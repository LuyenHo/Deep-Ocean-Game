#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Agent_Emitter.h"
#include "Shape.h"
#include "Player.h"
#include "Player_Emitter.h"

typedef enum { Easy, Medium, Hard } Difficult;
class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        void gameOver();
        void setDiff (int);
        void kill_agent(Agent);

        Agent_Emitter  *emitter = NULL;
        Agent_Emitter  *explosion = NULL;
        Player_Emitter *playEmitter = NULL;
        Player player;

        ofImage defaultImage;
        ofImage buble;
        ofImage playerImage;
        ofImage backGround_1;
        ofImage backGround_2;
    
        bool imageLoaded;
        bool gameStart = false;
        bool gameStop = false;
        bool backImage = false;
        bool coll = false;
    
        TurbulenceForce *turbForce;
        ImpulseRadialForce *radialForce;

        // Some basic UI
        //
        bool bHide;
    
        float level = 0;    
        ofTrueTypeFont font1;
        ofTrueTypeFont font2;
        
        ofxIntSlider difficult;
        ofxFloatSlider energy;
        ofxFloatSlider playerScale;
        ofxToggle customPlayer;
    
        ofxIntSlider rate;
        ofxIntSlider life;
        ofxFloatSlider agentScale;
        ofxIntSlider agentLimit;
        ofxToggle customAgent;
    
        ofxLabel screenSize;

        ofxPanel gui;
        ofSoundPlayer shoting;
        ofSoundPlayer explosing;
        ofSoundPlayer moving;
        ofSoundPlayer attacking;
};

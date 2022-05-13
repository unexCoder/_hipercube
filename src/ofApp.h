#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Teseracto.h"

// listening port
#define PORT 12345

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
    
    void oscImput();
    void renderMode(int mode, bool shaderOn);
    void screenSettings(int settings);
    
    ofxOscReceiver receiver;
    float peak,rms;
    int cc,val;
    
    float rotX,rotY,rotZ;
    float scale,zPos;
    float modDepth;
    
    
    ofEasyCam cam;
    std::vector<std::vector<ofColor>> colors;
    ofColor back, penA, penB, penC;
    int numPaleta;
    int shiftColors;
    bool backRefresh,fill;
    
    float camZ;
    
    Teseracto hypercubo;
    
    ofShader shader,shader_2;
    glm::vec3 lightDir;
    float ribbonWidth;
    
    bool applyShader;
    int numShader;
    int mode,screenMode;
    
    ofTexture mainFrame;
    ofFbo fbo;
};


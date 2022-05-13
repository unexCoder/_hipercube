#include "ofApp.h"
#include <sstream>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    // listen on the given port
    ofLog() << "listening for osc messages on port " << PORT;
    receiver.setup(PORT);
    
    colors = {{ofColor::fromHex(0x140D06),ofColor::fromHex(0xDCC6AE),ofColor::fromHex(0x605F5B)},{ofColor::fromHex(0x141213),ofColor::fromHex(0xEAEEBD),ofColor::fromHex(0xC0CDDD)},{ofColor::fromHex(0xF30E69),ofColor::fromHex(0x0CB984),ofColor::fromHex(0x1899B8)},{ofColor::fromHex(0x1D4461),ofColor::fromHex(0xF7326A),ofColor::fromHex(0xE5D6B9)},{ofColor::fromHex(0xE55667),ofColor::fromHex(0xDCD5BB),ofColor::fromHex(0x8F316C)},{ofColor::fromHex(0x0B939F),ofColor::fromHex(0x0B939F),ofColor::fromHex(0xFB5815)},{ofColor::fromHex(0x318554),ofColor::fromHex(0x84000B),ofColor::fromHex(0xC29E0C)},{ofColor::fromHex(0xD9C17B),ofColor::fromHex(0xF3F4F6),ofColor::fromHex(0xBE3119)},{ofColor::fromHex(0xB50050),ofColor::fromHex(0xFEAD6E),ofColor::fromHex(0xECEA00),ofColor::fromHex(0xF30D0B)},{ofColor::fromHex(0xA0A4AF),ofColor::fromHex(0xA2862B),ofColor::fromHex(0xF40078),ofColor::fromHex(0x4577CE)},{ofColor::fromHex(0xF94B6E),ofColor::fromHex(0xF5F0EE),ofColor::fromHex(0xF0EEC7),ofColor::fromHex(0xB63784)},{ofColor::fromHex(0xD28BAB),ofColor::fromHex(0xF5F0EE),ofColor::fromHex(0x5E8E2A),ofColor::fromHex(0xE7D221)},{ofColor::fromHex(0xDFCCDF),ofColor::fromHex(0x90FAF8),ofColor::fromHex(0x31A623),ofColor::fromHex(0xFA5D57)},{ofColor::fromHex(0xE86378),ofColor::fromHex(0xE2C223),ofColor::fromHex(0x9F5B48),ofColor::fromHex(0x111789)},{ofColor::fromHex(0x125187),ofColor::fromHex(0xFE1CAE),ofColor::fromHex(0xBCC3BB),ofColor::fromHex(0x02382A)},{ofColor::fromHex(0xCCAC3D),ofColor::fromHex(0x8A0002)},
        {ofColor::fromHex(0xFEF5EC),ofColor::fromHex(0xC2A130)}
    };
    numPaleta = 0;shiftColors = 0;
    back = colors[numPaleta][0];penA = colors[numPaleta][1];
    backRefresh = true;fill = true;
    
    glEnable(GL_DEPTH_TEST);
    ofBackground(back);
    cam.setFarClip(150000.f);
    camZ = 0.f; // -10000 <> 150000 range
    
    hypercubo.setup();
    shader.load("shaders/vostok.vert","shaders/vostok.frag",
                "shaders/vostok.geom");
    
    screenMode = 0;

    // screen buffer
    fbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGB8,4096);

    /////// video-rec
    //mainFrame.allocate(1920,1080,GL_RGB);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
    oscImput();
    // spatial
    //if(peak > 0.15) {}
    //ribbonWidth = ofMap(ofGetMouseX()*rms,0,ofGetWidth()/rms,1,50);
    ribbonWidth = modDepth*rms;

    lightDir = glm::vec3(sin(ofGetElapsedTimef()/10.f), cos(ofGetElapsedTimef()/10.f), 0.f);
    
    screenSettings(screenMode);
}

//--------------------------------------------------------------
void ofApp::draw(){
 
    // framebuffer write
    fbo.begin();
    cam.begin();
    cam.setDistance(camZ);
    if (fill) {
        ofFill();
    } else {
        ofNoFill();
    }
    if (backRefresh) {
        ofBackground(back);
    }
    renderMode(mode,applyShader);
    cam.end();
    fbo.end();
    
    // screen render
    fbo.draw(0,0,ofGetWidth(),ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case 'z':
            applyShader = !applyShader;
            break;
        case 'a':
            numShader++;numShader=numShader%2;
            switch (numShader) {
                case 1:
                    shader.load("shaders/noise.vert","shaders/noise.frag",
                                "shaders/noise.geom");
                    break;
                default:
                    shader.load("shaders/vostok.vert","shaders/vostok.frag",
                                "shaders/vostok.geom");
                    break;
            }
            break;
        case 'x':
            numPaleta = (numPaleta+1)%colors.size();
            back = colors[numPaleta][0];penA = colors[numPaleta][1];
            backRefresh=true;
            ofBackground(back);
            hypercubo.setGlobalColor(penA);
            break;
        case 'c':
            shiftColors = (shiftColors+1)%4;
            back = colors[numPaleta][shiftColors % colors[numPaleta].size()];
            penA = colors[numPaleta][(1+shiftColors) % colors[numPaleta].size()];
            penB = colors[numPaleta][(2+shiftColors) % colors[numPaleta].size()];
            penC = colors[numPaleta][(3+shiftColors) % colors[numPaleta].size()];
            ofBackground(back);
            hypercubo.setGlobalColor(penA);
            break;
        case 'v':
            backRefresh = !backRefresh;
            break;
        case 'm':
            mode++;mode=mode%5;
            break;
        case 'n':
            fill = !fill;
            break;
        case 'b':
            screenMode++;screenMode=screenMode%2;
            break;
        case 's':
            ofSaveScreen(to_string(ofGetFrameNum())+"out.png");
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::oscImput(){
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        string s = m.getAddress();
        if(s == "/amp"){
            // both the arguments are floats
            peak = m.getArgAsFloat(0);
            rms = m.getArgAsFloat(1);
            //cout << rms << endl;
        } else if(s == "/trigger") {
            cout << "osc_trigger_in" << endl;
        } else if (s == "/midi") {
            cc = m.getArgAsInt(0);
            val = m.getArgAsInt(1);
            cout << cc << endl;
            cout << val << endl;
            switch (cc) {
                case 64:
                    rotX = val/32.0;
                    break;
                case 72:
                    rotY = val/32.0;
                    break;
                case 80:
                    rotZ = val/32.0;
                    break;
                case 88:
                    zPos = (val-64.f)*20.f;
                    break;
                case 65:
                    scale = val*2.f;
                    break;
                case 73:
                    modDepth = val/127.f;
                    break;

                default:
                    break;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::renderMode(int mode, bool shaderOn){
    
    if (shaderOn) {
        shader.begin();
        shader.setUniform1f("thickness", ribbonWidth);
        shader.setUniform3f("lightDir", lightDir);
    }
    
    ofPushMatrix();
    ofTranslate(0,0,zPos);
    
    ofScale(scale);
    
    ofRotateRad(rotX, 1.0f, 0.0f, 0.0f);
    ofRotateRad(rotY, 0.0f, 1.0f, 0.0f);
    ofRotateRad(rotZ, 0.0f, 0.0f, 1.0f);
    
    switch (mode) {
        case 0:
            // hypercubo.draw(1,0.3);
            hypercubo.draw(0,1.0);
           break;
        case 1:
            // hypercubo.draw(1,0.3);
            ofTranslate(sin(ofGetFrameNum()), 0 , 0);
            hypercubo.draw(0,1.0);
            break;
        case 2:
            // noise
            for (int i = 0; i < 40; i++) {
                ofRotateRad((i/100)+(ofRandomf()/100), 1.0f, 0.0f, 1.0f);
                hypercubo.draw(0,1.0);
            }
           break;
        case 3:
            // seno noise
            for(int i = 0; i < 4; i++) {
                ofTranslate(sin(ofGetFrameNum())*0.1, cos(ofGetFrameNum())*0.1 , cos(ofGetFrameNum())*0.1);
                hypercubo.draw(0,1.0);
            }
            break;
        case 4:
            // simetricomplex rotation
            for (int i = 0; i < 4; i++) {
                ofRotateRad((i*sin(i)*10), 1.0f, 1.0f, 1.0f);
                hypercubo.draw(0,1.0);
            }
            break;
        default:
            break;
    }
    
    ofPopMatrix();
    if (shaderOn) {shader.end();}
}
//--------------------------------------------------------------

void ofApp::screenSettings(int settings) {
    switch (settings) {
        case 1:
            rotY=PI/4;
            rotX=ofGetElapsedTimef();
            break;
            
        default:
            break;
    }
}

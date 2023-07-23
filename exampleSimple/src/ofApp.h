#pragma once
#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxColorQuantizer.h"

#include "ofxGui.h"

//--

class ofApp : public ofBaseApp
{
public:
    ofxColorQuantizer colorQuantizer;
    vector<ofColor> palette;
    void doBuildQuantize();
    void buildFromImageFile(string path, int num);
    void buildFromImageUrl(string url, int num);
    void quantizeImage(string imageName, int numColors);

    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    
    string imageName;
    ofImage image;
    ofImage imageCopy;//use an smaller image to improve speed processing!

    ofParameterGroup parameters;
    ofParameter<int> numColors;
    ofParameter<void> bReBuild;
    void Changed_parameters(ofAbstractParameter &e);
    ofxPanel gui;

    void draw_Palette_Preview();
    int boxSize;
    int boxPad;

    void drawBg();//blinks to reflect that quantization is threaded
    ofParameter<ofColor> colorBg{"ColorBg", ofColor::grey, ofColor(), ofColor()};
    float v = 1;
};


#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxColorQuantizer.h"
#include "ofxGui.h"

typedef struct {
    int pos;
    ofColor color;
} colorNameMapping;

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
	
		void keyPressed(int key);
		void keyReleased(int key);
	
		ofxColorQuantizer colorQuantizer;
        void quantizeImage(string imageName, int numColors);
    
        ofImage image;
        int index = 0;
    
        struct weightedColor {
            ofColor color;
            float weight;
            float distance;
        };
    
        vector< weightedColor > sortedColors;
    
        struct by_distance {
            bool operator()(weightedColor const &a, weightedColor const &b) {
                return a.distance > b.distance;
            }
        };

    //-

    void getImagePalette(string path, int num);
    void getImageFromURLPalette(string rul, int num);

    void map_setup();
    map < int, ofColor > colorNameMap;
    vector < colorNameMapping > colorNames;

    ofxPanel gui;
    ofParameter<int> sortedType;
    ofParameter<int> numColors;
    ofParameter<string> labelStr;
    ofParameterGroup parameters;
    void Changed_parameters(ofAbstractParameter &e);

    vector<ofColor> palette;

    void draw_Palette_Preview();
    int boxSize = 50;
    int boxPad = 10;

    void kMeansTest();

    string imageName;

};

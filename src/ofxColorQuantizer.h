
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

/* -----------------------------------------------------------------------------
\
/	ofxColorQuantizer
\	Quantizes colors found in an image to extract a color palette
/	Based on Cinder ocvColorQuantize demo -- https://github.com/cinder/Cinder-OpenCV/blob/master/samples/ocvColorQuantize/
\	Ported by Jeremy Rotsztain -- http://www.mantissa.ca
/
\ ----------------------------------------------- */


class ofxColorQuantizer : public ofThread {

public:
	ofxColorQuantizer();
	~ofxColorQuantizer();
	
	void quantize(ofPixels image);

	void draw(ofPoint pos = ofPoint(0, 0));
	void draw(int x, int y);

	void setNumColors(unsigned int nColors);
	int getNumColors();

	vector<ofColor>& getColors();
	vector<float> getColorWeights();
	float getColorWeight(int index);

protected:
	ofPixels inputImage;

	vector<ofColor>colors;
	unsigned int numColors;
	vector<float>histogram;

	void threadedFunction();

	bool bReady = false;
	bool bProcessing = false;
	uint64_t timeForLastProcess;

public:
	uint64_t getTimeforLastProcess() { return timeForLastProcess; };
	bool isReady();
	bool isProcessing();


};

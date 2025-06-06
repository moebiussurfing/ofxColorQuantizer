
#include "ofxColorQuantizer.h"

ofxColorQuantizer::ofxColorQuantizer() {
	ofLogNotice("ofxColorQuantizer") << __FUNCTION__;

	// set default # of colors
	numColors = 12;
}

ofxColorQuantizer::~ofxColorQuantizer() {

	waitForThread(true);
}

void ofxColorQuantizer::quantize(ofPixels inputImage) {
	ofLogNotice("ofxColorQuantizer") << __FUNCTION__;

	if (isThreadRunning()) {
		ofLogWarning("ofxColorQuantizer") << "Quantization already in progress. Please wait.";
		return;
	}

	// Copy the input image to a member variable
	this->inputImage = inputImage;
	
	//fix
	bReady = false;

	// Start the thread
	startThread();
}

void ofxColorQuantizer::threadedFunction() {
	ofLogNotice("ofxColorQuantizer") << __FUNCTION__;

	while (isThreadRunning())
	{
		if (lock())
		{
			// Perform the color quantization here
			// palette = ...
			bReady = false;
			bProcessing = true;
			auto t = ofGetElapsedTimeMillis();
			timeForLastProcess = 0;

			//--

			const int colorCount = numColors;
			const int sampleCount = inputImage.getHeight() * inputImage.getWidth();
			cv::Mat colorSamples(sampleCount, 1, CV_32FC3);

			// get our pixels
			unsigned char* pixels = inputImage.getPixels();

			// clear our list of colors
			colors.clear();

			// build our matrix of samples
			cv::MatIterator_<cv::Vec3f> sampleIt = colorSamples.begin<cv::Vec3f>();
			for (int i = 0; i < sampleCount; i++) {
				int pos = i * 3;
				*sampleIt = cv::Vec3f(pixels[pos], pixels[pos + 1], pixels[pos + 2]);
				sampleIt++;
			}

			// call k-means
			cv::Mat labels, clusters;
			cv::kmeans(colorSamples, colorCount, labels, cv::TermCriteria(), 2, cv::KMEANS_RANDOM_CENTERS, clusters);
			//cv::TermCriteria::COUNT, 8, 0 

#if 1
			// calculate histogram
			histogram.clear();
			histogram.resize(colorCount);

			for (int i = 0; i < labels.rows; i++) {
				++histogram[labels.at<int>(i, 0)];
			}
			float sum = 0;
			for (int i = 0; i < histogram.size(); i++) {
				histogram[i] = histogram[i] / colorSamples.rows;
				sum += histogram[i];
			}
#endif

			// clear our list of colors
			colors.clear();

			for (int i = 0; i < colorCount; ++i) {
				ofColor clusterColor = ofColor(clusters.at<cv::Vec3f>(i, 0)[0], clusters.at<cv::Vec3f>(i, 0)[1], clusters.at<cv::Vec3f>(i, 0)[2]);
				colors.push_back(clusterColor);
			}

			/*
			// add colors from labels
			unsigned char * p = temp.getPixels();
			cv::MatIterator_<int> labelIt = labels.begin<int>();
			for(int i=0; i<colorCount; i++){

				ofColor newColor;
				newColor.r = clusterColors[*labelIt].r;
				newColor.g = clusterColors[*labelIt].g;
				newColor.b = clusterColors[*labelIt].b;

				++labelIt;
				//cout << i << " " << colors[i] << endl;
			}
			*/

			bReady = true;
			bProcessing = false;
			timeForLastProcess = ofGetElapsedTimeMillis() - t;

			//--

			unlock();
			break;
		}
	}
}

bool ofxColorQuantizer::isReady() {
	if (bReady) {
		bReady = false;
		return true;
	}
	return false;
}

bool ofxColorQuantizer::isProcessing() {
	return bProcessing;
}

void ofxColorQuantizer::draw(ofPoint pos) {
	ofLogNotice("ofxColorQuantizer") << __FUNCTION__;

	int swatchSize = 20;
	int swatchID = 0;

	ofPushMatrix();

	ofTranslate(pos);

	vector<ofColor>::iterator cIter = colors.begin();
	while (cIter != colors.end()) {

		ofSetColor(*cIter);
		ofDrawRectangle(0, 0, swatchSize, swatchSize);
		ofTranslate(swatchSize, 0, 0);
		cIter++;
	}

	ofPopMatrix();
}

void ofxColorQuantizer::draw(int x, int y) {
	draw(ofPoint(x, y));
}

vector<ofColor>& ofxColorQuantizer::getColors() {
	ofLogNotice("ofxColorQuantizer") << __FUNCTION__;
	return colors;
}

void ofxColorQuantizer::setNumColors(unsigned int nColors) {
	ofLogNotice("ofxColorQuantizer") << __FUNCTION__;
	numColors = nColors;
}

int ofxColorQuantizer::getNumColors() {
	ofLogNotice("ofxColorQuantizer") << __FUNCTION__;
	return numColors;
}

vector< float > ofxColorQuantizer::getColorWeights() {
	ofLogNotice("ofxColorQuantizer") << __FUNCTION__;
	return histogram;
}

float ofxColorQuantizer::getColorWeight(int index) {
	if (index < histogram.size())
		return histogram[index];
}

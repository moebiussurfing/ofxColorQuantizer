#include "ofApp.h"

#include "ColorConverter.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);

	//-

	parameters.setName("COLOR QUANTIZER");
	parameters.add(numColors.set("Number of Colors", 10, 1, 50));
	parameters.add(bReBuild.set("Build"));

	gui.setup("ofxColorQuantizer");
	gui.add(parameters);
	gui.setPosition(450, 50);

	ofAddListener(parameters.parameterChangedE(), this, &ofApp::Changed_parameters);

	//-

	imageName = "0.jpg";
	buildFromImageFile(imageName, numColors);
}

//--------------------------------------------------------------
void ofApp::update()
{
	string s = ofToString(ofGetFrameRate(), 0) + " FPS";
	ofSetWindowTitle(s);

	if (colorQuantizer.isReady())
	{
		palette = colorQuantizer.getColors();
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawBg();

	//--

	// Debug text
	string s = "ofxColorQuantizer\n\n";
	s += "Loaded image Path/Url: '" + imageName + "'\n";
	s += "Select image: Key [01234567][qwe][asd][zxc]\n";
	s += "Pick a random image: [SPACE]\n\n";
	s += "Last Process Time: " + ofToString(colorQuantizer.getTimeforLastProcess()) + "ms\n";
	s += "Last Image size: " + ofToString(image.getWidth()) + "x" + ofToString(image.getHeight()) + "px";
	ofColor c;
	if (colorQuantizer.isProcessing()) {
		const int d = 7;
		if ((ofGetFrameNum() % d) < (d / 2.f)) c = ofColor::white;
		else c = ofColor::black;
	}
	else c = ofColor::white;

	ofDrawBitmapStringHighlight(s, 10, 20, ofColor::black, c);

	//--

	if (palette.size() > 0)
	{
		// Image

		ofPushStyle();
		ofPushMatrix();
		{

			int x = 50;
			boxPad = 2;
			ofTranslate(x, 200);
			ofSetColor(255);

			// draw original image but resized to ImgW pixels width, same aspect ratio
			float imgRatio = image.getHeight() / image.getWidth();
			int ImgW = 200;
			int imgH = imgRatio * ImgW;

			image.draw(0, 0, ImgW, imgH);

			//--

			// Colors
			if (!colorQuantizer.isProcessing())
			{
				float wPal = ofGetWidth() - (x + ImgW + x);
				float boxW = wPal / palette.size();
				boxSize = boxW - boxPad;

				ofPushMatrix();
				ofTranslate(0, imgH);

				ofSetColor(255, 100);
				ofDrawBitmapString("(Original sorting has colors weighted based on their areas, their order is based on their chroma values)", 0, 50);

				ofTranslate(ImgW + 20, 0);

				// all colors % bars
				for (int i = 0; i < palette.size(); i++)
				{
					ofSetColor(0, 50);
					ofDrawRectangle(i * (boxSize + boxPad), 0, boxSize, -imgH);
					ofSetColor(palette[i]);
					ofDrawRectangle(i * (boxSize + boxPad), 0, boxSize, ofMap(colorQuantizer.getColorWeight(i), 0, 1, 0, -imgH));
					ofSetColor(255);
					ofDrawBitmapString(ofToString(int(colorQuantizer.getColorWeight(i) * 100)) + "%", i * (boxSize + boxPad), 30);
				}

				// palette preview
				ofTranslate(0, 100);
				draw_Palette_Preview();
				ofPopMatrix();
			}
		}
		ofPopMatrix();
		ofPopStyle();
	}

	//--

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::draw_Palette_Preview()
{
	ofPushMatrix();
	ofPushStyle();

	ofRectangle r;
	string str = "Palette Preview";
	ofDrawBitmapStringHighlight(str, 5, -10, ofColor::black, ofColor::white);

	for (int col = 0; col < palette.size(); col++)
	{
		r = ofRectangle(0, 0, boxSize, boxSize);
		ofFill();
		ofSetColor(palette[col]);
		ofDrawRectangle(r);
		ofTranslate(boxSize + boxPad, 0);
	}

	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawBg()
{
	if (colorQuantizer.isProcessing())
	{
		// Fade blink when waiting. 
		ofColor c = colorBg.get();
		auto br = c.getBrightness();
		float g = 30; //amplitude
		float v = glm::cos(10 * ofGetElapsedTimef());
		float a1 = ofMap(v, -1, 1, br - g, br, true);
		c.setBrightness(a1);
		ofClear(c);
	}
	else
	{
		// Flash when submit
		if (v > 0) v -= 0.05f;
		else v = 0;
		int bgMin = 100;
		if (v > 0) ofClear(bgMin + (255 - bgMin) * v);
		else
		{
			ofClear(colorBg.get());
		}
	}
}

//--

//--------------------------------------------------------------
void ofApp::doBuildQuantize() {
	v = 1;

	colorQuantizer.setNumColors(numColors);
	colorQuantizer.quantize(imageCopy.getPixels());
}

//--------------------------------------------------------------
void ofApp::quantizeImage(string imgName, int _numColors) {
	numColors = _numColors;

	image.load(imgName);

	// We will quantize this down sized version to speed up
	imageCopy.clear();
	imageCopy = image;
	imageCopy.resize(imageCopy.getWidth() / 4, imageCopy.getHeight() / 4);

	doBuildQuantize();
}

//--------------------------------------------------------------
void ofApp::buildFromImageFile(string path, int num) {
	//TODO: improve with threading load..
	quantizeImage(path, num);
}

//--------------------------------------------------------------
void ofApp::buildFromImageUrl(string url, int num) {
	//TODO: improve with threading load and some HTTP image browsing api..
	quantizeImage(url, num);
}

//--

//--------------------------------------------------------------
void ofApp::Changed_parameters(ofAbstractParameter& e) {
	string n = e.getName();

	ofLogNotice() << "Changed_parameters: " << n << ": " << e;

	if (n == bReBuild.getName())
	{
		doBuildQuantize();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofLogNotice("ofApp") << __FUNCTION__<< char(key);

	// Pick files

	if (key == ' ')
	{
		switch ((int)ofRandom(19))
		{
		case 0: key = '0'; break;
		case 1: key = '1'; break;
		case 2: key = '2'; break;
		case 3: key = '3'; break;
		case 4: key = '4'; break;
		case 5: key = '5'; break;
		case 6: key = '6'; break;
		case 7: key = '7'; break;
		case 8: key = 'q'; break;
		case 9: key = 'w'; break;
		case 10: key = 'e'; break;
		case 11: key = 'a'; break;
		case 12: key = 's'; break;
		case 13: key = 'd'; break;
		case 14: key = 'f'; break;
		case 15: key = 'z'; break;
		case 16: key = 'x'; break;
		case 17: key = 'c'; break;
		}
	}

	if (key == '0')
	{
		imageName = "0.jpg";
		buildFromImageFile(imageName, numColors);
	}

	else if (key == '1')
	{
		imageName = "1.jpg";
		buildFromImageFile(imageName, numColors);
	}

	else if (key == '2')
	{
		imageName = "2.jpg";
		buildFromImageFile(imageName, numColors);
	}

	else if (key == '3')
	{
		imageName = "3.jpg";
		buildFromImageFile(imageName, numColors);
	}

	else if (key == '4')
	{
		imageName = "4.jpg";
		buildFromImageFile(imageName, numColors);
	}

	else if (key == '5')
	{
		imageName = "5.jpg";
		buildFromImageFile(imageName, numColors);
	}

	else if (key == '6')
	{
		imageName = "6.jpg";
		buildFromImageFile(imageName, numColors);
	}

	else if (key == '7')
	{
		imageName = "7.jpg";
		buildFromImageFile(imageName, numColors);
	}

	//--

	// Urls palettes


	else if (key == 'q')
	{
		imageName = "https://media.gettyimages.com/photos/argentina-buenos-aires-dawn-at-center-with-rush-hour-picture-id667138246?k=6&m=667138246&s=612x612&w=0&h=PTpexeaqMB-gYmImToxeV2cMGrRAPImeLkws6fFKfh0=";
		buildFromImageFile(imageName, numColors);
	}


	else if (key == 'w')
	{
		imageName = "https://as1.ftcdn.net/jpg/02/13/60/70/500_F_213607058_uz3KRA8ASgk89L1DahwlfHrfQ74T2g5n.jpg";
		buildFromImageFile(imageName, numColors);
	}


	else if (key == 'e')
	{
		imageName = "https://creativepro.com/wp-content/uploads/sites/default/files/styles/article-full-column-width/public/20140306-color1.jpg?itok=3oHDuKTN";
		buildFromImageFile(imageName, numColors);
	}

	//--

	// Url pictures


	else if (key == 'a')
	{
		imageName = "https://hips.hearstapps.com/esq.h-cdn.co/assets/16/20/blade-runner_1.jpg";
		buildFromImageFile(imageName, numColors);
	}


	else if (key == 's')
	{
		imageName = "https://www.eldiario.es/fotos/Paleta-San-Junipero-Black-Mirror_EDIIMA20190731_0485_19.jpg";
		buildFromImageFile(imageName, numColors);
	}


	else if (key == 'd')
	{
		imageName = "https://www.eldiario.es/fotos/Paleta-colores-verdes-escena-Land_EDIIMA20190731_0457_19.jpg";
		buildFromImageFile(imageName, numColors);
	}

	else if (key == 'f')
	{
		imageName = "http://mymodernmet.com/wp/wp-content/uploads/2017/08/palette-maniac-15.jpg";
		buildFromImageFile(imageName, numColors);
	}

	//--

	// More Urls

	else if (key == 'z')
	{
		imageName = "https://s3.amazonaws.com/images.gearjunkie.com/uploads/2018/05/matterhorn-3x2.jpg";
		buildFromImageFile(imageName, numColors);
	}

	if (key == 'x')
	{
		imageName = "http://cdn.cnn.com/cnnnext/dam/assets/170407220916-04-iconic-mountains-matterhorn-restricted.jpg";
		buildFromImageFile(imageName, numColors);
	}

	if (key == 'c')
	{
		imageName = "https://store-images.s-microsoft.com/image/apps.33776.13570837168441901.d8820ad6-c4ef-45a9-accb-c6dd763aee48.560134ce-5fa0-4486-95cd-b0ba8d4921ff?w=672&h=378&q=80&mode=letterbox&background=%23FFE4E4E4&format=jpg";
		buildFromImageFile(imageName, numColors);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

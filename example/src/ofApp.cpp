#include "ofApp.h"

//bool compareName( const colorNameMapping& s1, const colorNameMapping& s2 ) {
//    return s1.name < s2.name;
//}

bool comparePos( const colorNameMapping& s1, const colorNameMapping& s2 ) {
    return s1.pos < s2.pos;
}

bool compareBrightness( const colorNameMapping& s1, const colorNameMapping& s2 ) {
    return s1.color.getBrightness() < s2.color.getBrightness();
}

bool compareHue( const colorNameMapping& s1, const colorNameMapping& s2 ) {
    return s1.color.getHue() < s2.color.getHue();
}

bool compareSaturation( const colorNameMapping& s1, const colorNameMapping& s2 ) {
    return s1.color.getSaturation() < s2.color.getSaturation();
}

//--------------------------------------------------------------
void ofApp::map_setup(){
    palette.clear();
    int palSize = colorQuantizer.getNumColors();
    palette.resize(palSize);
    palette = colorQuantizer.getColors();

    colorNameMap.clear();
    colorNames.clear();

    for (int i=0; i<palSize; i++)
    {
        colorNameMap[i] = palette[i];
    }


    // this map is useful if we want to address the colors by string.
    // since we might want to sort this, we can put them in a vector also

    for (unsigned int i = 0; i < palSize; i++){//colorNameMap

//        map<string, ofColor>::iterator mapEntry = colorNameMap.begin();
        map<int, ofColor>::iterator mapEntry = colorNameMap.begin();
        std::advance( mapEntry, i );

        colorNameMapping mapping;
//        mapping.name = mapEntry->first;
        mapping.pos = mapEntry->first;
        mapping.color = mapEntry->second;
        colorNames.push_back(mapping);

    }

//    //TODO: BUG
//    // sort
//    sortedType = 4;
//    labelStr = "Saturation";
//    sortLabel = labelStr;
//    ofSort(colorNames, compareSaturation);

    switch (sortedType) {
        case 1:
            ofSort(colorNames, comparePos);
            labelStr = "Original";
            sortLabel = labelStr;
            break;
        case 2:
            ofSort(colorNames, compareHue);
            labelStr = "Hue";
            sortLabel = labelStr;
            break;
        case 3:
            ofSort(colorNames, compareBrightness);
            labelStr = "Brightness";
            sortLabel = labelStr;
            break;
        case 4:
            ofSort(colorNames, compareSaturation);
            labelStr = "Saturation";
            sortLabel = labelStr;
            break;
        default:
            break;
    }
}


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);

    //-
    
    gui.setup(); // most of the time you don't need a name
    gui.add(numColors.setup("numColors", 10, 1, 50));
    gui.add(sortLabel.setup("sortLabel", labelStr));
    gui.setPosition(10, ofGetHeight()-100);

    sortedType = 2;

    getImagePalette("photo.jpg", numColors);

//    map_setup();
}


//--------------------------------------------------------------
void ofApp::getImagePalette(string path, int num){

    // load our image
    image.loadImage(path);

    // for speedz sake, quantize a smaller copy of the image
    ofImage imageCopy;
    imageCopy.loadImage(path);
    imageCopy.resize(imageCopy.getWidth()/2, imageCopy.getHeight()/2);

    // get our colors
    colorQuantizer.setNumColors(num);
    colorQuantizer.quantize(imageCopy.getPixelsRef());

    // resize the window to match the image
//    ofSetWindowShape(image.getWidth(), image.getHeight());

    map_setup();
}


//--------------------------------------------------------------
void ofApp::getImageFromURLPalette(string url, int num){

    // load our image
    image.loadImage(url);
    
    if(image.getWidth()>1000 || image.getHeight()>600 )
    image.resize(image.getWidth()/3, image.getHeight()/3);

    // for speedz sake, quantize a smaller copy of the image
    ofImage imageCopy;
    imageCopy.loadImage(url);
    imageCopy.resize(imageCopy.getWidth()/3, imageCopy.getHeight()/3);

    // get our colors
    colorQuantizer.setNumColors(num);
    colorQuantizer.quantize(imageCopy.getPixelsRef());

    // resize the window to match the image
    //    ofSetWindowShape(image.getWidth(), image.getHeight());

    map_setup();
}


//--------------------------------------------------------------
void ofApp::update(){
	

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofBackground(100,100,100);

	ofSetColor(255);

	image.draw(0, 0);
	
//    colorQuantizer.draw(ofPoint(0, image.getHeight()-20));
    colorQuantizer.draw(ofPoint(20, image.getHeight()+20));

    draw_PaleteMINI();

    gui.draw();
}



//--------------------------------------------------------------
void ofApp::draw_PaleteMINI()
{
    int boxW = 40;
    int boxPad = 1;
    int boxSize = boxW+boxPad;
    glm::vec2 palettePos = glm::vec2(ofGetWidth() - palette.size()*2*boxSize, ofGetHeight()-boxSize);
    ofRectangle r;

    ofPushMatrix();
    ofPushStyle();
    ofTranslate(palettePos);

    for (int col=0; col<palette.size(); col++)
    {
        r = ofRectangle(0, 0, boxW, boxW);
        ofFill();

        ofSetColor(colorNames[col].color);
//        ofSetColor(palette[col]);

        ofDrawRectangle(r);
        ofTranslate ( boxSize, 0);
    }

    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::kMeansTest(){

	cv::Mat samples = (cv::Mat_<float>(8, 1) << 31 , 2 , 10 , 11 , 25 , 27, 2, 1);
	cv::Mat labels;
	
	// double kmeans(const Mat& samples, int clusterCount, Mat& labels,
	cv::TermCriteria termcrit;
	int attempts, flags;
	cv::Mat centers;
	double compactness = cv::kmeans(samples, 3, labels, cv::TermCriteria(), 2, cv::KMEANS_PP_CENTERS, centers);
	
	cout<<"labels:"<<endl;
	for(int i = 0; i < labels.rows; ++i)
	{ 
		cout<<labels.at<int>(0, i)<<endl;
	}
	
	cout<<"\ncenters:"<<endl;
	for(int i = 0; i < centers.rows; ++i)
	{
		cout<<centers.at<float>(0, i)<<endl;
	}
	
	cout<<"\ncompactness: "<<compactness<<endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == 'k'){
        kMeansTest();
    }

    // sort
 if (key==OF_KEY_BACKSPACE){
     sortedType++;
     if (sortedType>4)
         sortedType = 1;

     switch (sortedType) {
         case 1:
             ofSort(colorNames, comparePos);
             labelStr = "Original";
             sortLabel = labelStr;
             break;
         case 2:
             ofSort(colorNames, compareHue);
             labelStr = "Hue";
             sortLabel = labelStr;
             break;
         case 3:
             ofSort(colorNames, compareBrightness);
             labelStr = "Brightness";
             sortLabel = labelStr;
             break;
         case 4:
             ofSort(colorNames, compareSaturation);
             labelStr = "Saturation";
             sortLabel = labelStr;
             break;
         default:
             break;
     }
 }

//    if (key == '1'){
//        if (sortedType != 1){
//            sortedType = 1;
//            ofSort(colorNames, comparePos);
////            ofSort(colorNames, compareName);
//        }
//    }
//    if (key == '2'){
//        if (sortedType != 2){
//            sortedType = 2;
//            ofSort(colorNames, compareHue);
//        }
//    }
//    else if (key == '3'){
//        if (sortedType != 3){
//            sortedType = 3;
//            ofSort(colorNames, compareBrightness);
//        }
//    }
//    else if (key == '4'){
//        if (sortedType != 4){
//            sortedType = 4;
//            ofSort(colorNames, compareSaturation);
//        }
//    }

    // files
    if (key=='0')
        getImagePalette("0.jpg", numColors);
    if (key=='1')
        getImagePalette("1.jpg", numColors);
    if (key=='2')
        getImagePalette("2.jpg", numColors);
    if (key=='3')
        getImagePalette("3.jpg", numColors);

    // urls palettes
    if (key=='q')
        getImageFromURLPalette("https://mk0learntocodew6bl5f.kinstacdn.com/wp-content/uploads/2016/01/material-palette.png", numColors);
    if (key=='w')
        getImageFromURLPalette("https://as1.ftcdn.net/jpg/02/13/60/70/500_F_213607058_uz3KRA8ASgk89L1DahwlfHrfQ74T2g5n.jpg", numColors);
    if (key=='e')
        getImageFromURLPalette("https://creativepro.com/wp-content/uploads/sites/default/files/styles/article-full-column-width/public/20140306-color1.jpg?itok=3oHDuKTN", numColors);
    if (key=='r')
        getImageFromURLPalette("http://www.newdesignfile.com/postpic/2011/11/flat-ui-design-colors_104577.jpg", numColors);
    if (key=='t')
        getImageFromURLPalette("https://cmkt-image-prd.global.ssl.fastly.net/0.1.0/ps/211731/910/607/m1/fpnw/wm0/main-.jpg?1413717475&s=845b5e05fb5961018c74a2ca8ae4d3dc", numColors);
    if (key=='y')
        getImageFromURLPalette("https://www.sketchappsources.com/resources/source-image/material-color-palette-egorfedorov.png", numColors);
    if (key=='u')
        getImageFromURLPalette("https://cdn.dribbble.com/users/282234/screenshots/4135500/flat_ui_colors_by_anwaltzzz.png", numColors);
    if (key=='i')
        getImageFromURLPalette("https://pbs.twimg.com/media/DlXu-iDVAAEHQmL.jpg", numColors);

    // url pictures
    if (key=='a')
        getImageFromURLPalette("https://www.eldiario.es/fotos/Paleta-colores-empleada-Her_EDIIMA20190731_0426_19.jpg", numColors);
    if (key=='s')
        getImageFromURLPalette("https://www.eldiario.es/fotos/Paleta-San-Junipero-Black-Mirror_EDIIMA20190731_0485_19.jpg", numColors);
    if (key=='d')
        getImageFromURLPalette("https://www.eldiario.es/fotos/Paleta-colores-verdes-escena-Land_EDIIMA20190731_0457_19.jpg", numColors);
    if (key=='f')
        getImageFromURLPalette("https://www.eldiario.es/fotos/Escala-monocromatica-Roma_EDIIMA20190731_0438_19.jpg", numColors);
    if (key=='g')
        getImageFromURLPalette("https://www.eldiario.es/fotos/Paleta-colores-John-Wick_EDIIMA20190731_0490_19.jpg", numColors);
    if (key=='h')
        getImageFromURLPalette("https://www.eldiario.es/fotos/Paleta-colores-John-Wick_EDIIMA20190731_0490_19.jpg", numColors);

    //field scapes
    if (key=='z')
        getImageFromURLPalette("https://s3.amazonaws.com/images.gearjunkie.com/uploads/2018/05/matterhorn-3x2.jpg", numColors);
    if (key=='x')
        getImageFromURLPalette("http://cdn.cnn.com/cnnnext/dam/assets/170407220916-04-iconic-mountains-matterhorn-restricted.jpg", numColors);
    if (key=='c')
        getImageFromURLPalette("https://store-images.s-microsoft.com/image/apps.33776.13570837168441901.d8820ad6-c4ef-45a9-accb-c6dd763aee48.560134ce-5fa0-4486-95cd-b0ba8d4921ff?w=672&h=378&q=80&mode=letterbox&background=%23FFE4E4E4&format=jpg", numColors);
    if (key=='v')
        getImageFromURLPalette("https://www.mountainphotography.com/images/xl/20120928-Cimarron-Sunset-Panorama.jpg", numColors);
    if (key=='b')
        getImageFromURLPalette("https://www.mountainphotography.com/images/640/20130608-Alpenglow-on-Mt.-Sneffels.jpg", numColors);
    if (key=='n')
        getImageFromURLPalette("https://image.shutterstock.com/image-vector/large-number-mountains-vast-landscapes-260nw-1389573218.jpg", numColors);
    if (key=='m')
        getImageFromURLPalette("https://www.telegraph.co.uk/content/dam/Travel/galleries/travel/activityandadventure/The-worlds-most-beautiful-mountains/mountains-eiger_3374107a.jpg", numColors);



}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

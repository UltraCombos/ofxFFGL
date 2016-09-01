#include "testApp.h"
#include "ofxFFGLPlugin.h"

testApp::testApp()
{
	addParameter(numRepeats.set("num repeats", numRepeats, 1, 20));
	addParameter(bbbbb.set("num repeatssss", bbbbb, 1, 20));
	addParameter(_bool_para.set("bool_para", _bool_para));
	addEventParameter(_event_para.set("event", _event_para));
	_event_para.addListener(this, &testApp::_on_event);
	//addParameter(_string_para.set("str", _string_para));
}

//--------------------------------------------------------------
void testApp::setup(){
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);
}

//--------------------------------------------------------------
void testApp::draw(){
	// input textures from host are stored here
	ofTexture * tex = inputTextures[0];
	
	if( !tex )
		return;

	int nrep = numRepeats;

	float w = (float)ofGetWidth() / nrep;
	float h = (float)ofGetHeight() / nrep;

	for( int y = 0; y < nrep; y++ )
		for( int x = 0; x < nrep; x++ )
		{
			tex->draw(w*x,h*y,w,h);
		}
	ofDrawBitmapString(_string_para, 0, ofGetHeight()/2);
	ofDrawBitmapString(_bool_para, 0, ofGetHeight() / 2 + 20);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

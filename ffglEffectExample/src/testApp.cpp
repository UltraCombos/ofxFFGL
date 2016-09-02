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
	_string_para += "setup!!!";
}

//--------------------------------------------------------------
void testApp::update(){
	//ofBackground(200,100,100);
}

//--------------------------------------------------------------
void testApp::draw(){
	ofPushMatrix(); 
	ofSetupScreen();
	// input textures from host are stored here
	ofTexture * tex = inputTextures[0];
	
	if( !tex )
		return;

	int nrep = numRepeats;

#if 0
	float w = (float)1.f / nrep;
	float h = (float)1.f / nrep;
#else
	float w = (float)ofGetWidth() / nrep;
	float h = (float)ofGetHeight() / nrep;
#endif
	for (int y = 0; y < nrep; y++)
	{
		for (int x = 0; x < nrep - 1; x++)
		{
			tex->draw(w*x, h*y, w*0.9, h*0.9);
		}
	}

	if (fbo.isAllocated() == false)
		fbo.allocate(tex->getWidth(), tex->getHeight());
	fbo.begin();
	ofClear(ofColor::blueSteel);
	fbo.end();

	fbo.draw(w, 0, w, h);

	ofSetColor(ofColor::gray);
	ofRect(0, 0, w, h);

	ofPopMatrix();
 	//ofDrawBitmapString(_string_para, 0, ofGetHeight()/2);
// 	ofSetColor(ofColor::red);
// 	ofDrawBitmapString(_bool_para, 0, ofGetHeight() / 2 + 20);
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

/*
 *  ofxFFGLApp.h
 *  FFGLExample
 *
 *  Created by ensta on 2/4/10.
 *  Copyright 2010 Daniel Berio. All rights reserved.
 *
 *  http://www.enist.org
 */

#ifndef _OFXFFGLAPP
#define _OFXFFGLAPP

#include "ofMain.h"
#include "ofParameterGroup.h"

#define MAX_INPUT_TEXTURES 3

class _FFGL_event : public ofParameter<bool>
{
	shared_ptr<ofAbstractParameter> newReference() const {
		return std::make_shared<_FFGL_event>(*this);
	}
};

class ofFFGLApp : public ofBaseApp {
	public:
        ofFFGLApp();
		virtual ~ofFFGLApp();
		
		// These functions add a parameter to the plugin, pass the address of the variable that will be automatically updated.
		// We could add normal FFGL parameter handling but this way even though it is more limited, it makes life much easier
		// Parameters MUST be added in the constructor or FFGL will ignore them.
		void addParameter(ofAbstractParameter& para);
		void addEventParameter(ofParameter<bool>& para);
		
		ofTexture * inputTextures[MAX_INPUT_TEXTURES];
		friend class ofFFGLPlugin;
private:
	ofParameterGroup paras;
};

#endif

/*
 *  ofxFFGLApp.cpp
 *  FFGLExample
 *
 *  Created by ensta on 2/4/10.
 *  Copyright 2010 Daniel Berio. All rights reserved.
 *
*  http://www.enist.org
 */

#include "ofxFFGLApp.h"

ofFFGLApp::ofFFGLApp()
{
	paras.clear();
	for( int i = 0; i < MAX_INPUT_TEXTURES; i++ )
		inputTextures[i] = 0;
}

ofFFGLApp::~ofFFGLApp()
{
	paras.clear();

	for( int i = 0; i < MAX_INPUT_TEXTURES; i++ )
	{
		if( inputTextures[i] ) 
		{
			inputTextures[i]->texData.textureID = 0; // prevent OF from deleting the actual GL texture since it is allocated by host
			delete inputTextures[i];
		}
	}
}

void ofFFGLApp::addParameter(ofAbstractParameter& para)
{
	string type = para.type();
	if (type == typeid(ofParameter<float>).name())
	{
		paras.add(para);
	}
	else if (type == typeid(ofParameter<string>).name())
	{
		paras.add(para);
	}
	else if (type == typeid(ofParameter<bool>).name())
	{
		paras.add(para);
	}
}

void ofFFGLApp::addEventParameter(ofParameter<bool>& para)
{
	_event_paras.emplace_back(_FFGL_event());
	_event_paras.back().makeReferenceTo(para);
	_event_paras.back().set(false);
	paras.add(_event_paras.back());
}

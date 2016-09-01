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
	parameters.clear();
	for( int i = 0; i < MAX_INPUT_TEXTURES; i++ )
		inputTextures[i] = 0;
}

ofFFGLApp::~ofFFGLApp()
{
	for(int i = 0; i < parameters.size(); i++ )
		delete parameters[i];
	parameters.clear();

	for( int i = 0; i < MAX_INPUT_TEXTURES; i++ )
	{
		if( inputTextures[i] ) 
		{
			inputTextures[i]->texData.textureID = 0; // prevent OF from deleting the actual GL texture since it is allocated by host
			delete inputTextures[i];
		}
	}
}

void ofFFGLApp::addBoolParameter( const char * name, bool * address )
{
	ofFFGLParameter * p = new ofFFGLParameter();
	p->initBool(name,address);
	parameters.push_back(p);
}


void ofFFGLApp::addEventParameter( const char * name, bool * address )
{
	ofFFGLParameter * p = new ofFFGLParameter();
	p->initEvent(name,address);
	parameters.push_back(p);
}
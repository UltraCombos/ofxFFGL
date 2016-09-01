/*
 *  ofxFFGLPlugin.cpp
 *  FFGLExample
 *
 *  Created by ensta on 2/4/10.
 *  Copyright 2010 Daniel Berio. All rights reserved.
 *
 *  http://www.enist.org
 */

#include "ofxFFGLPlugin.h"

#define FFPARAM_BRIGHTNESS (0)

ofFFGLPlugin::ofFFGLPlugin( ofFFGLApp * app, int minInputs, int maxInputs )
{
	// Input properties
	SetMinInputs(minInputs);
	SetMaxInputs(maxInputs);
	
	_app = shared_ptr<ofFFGLApp>(app);

	initParameters();
	
	isGLInitialized = false;
}

ofFFGLPlugin::~ofFFGLPlugin()
{
	isGLInitialized = false;
}

void ofFFGLPlugin::initParameters()
{
	for(int i = 0; i < _app->getNumParameters(); i++ )
	{
		ofFFGLParameter * v = _app->getParameter(i);
		
		switch( v->getType() )
		{
			case PARAM_BOOL:
			{
				SetParamInfo(i, v->getName(), FF_TYPE_BOOLEAN, v->getBool());
				break;
			}
			
			case PARAM_EVENT:
			{
				SetParamInfo(i,v->getName(), FF_TYPE_EVENT, v->getBool() );
				break;
			}
			default: ;
		}
	}

	for (size_t i = 0; i < _app->paras.size(); i++)
	{
		ofAbstractParameter& para = _app->paras.get(i);
		string type = para.type();
		if (type == typeid(ofParameter<float>).name())
		{
			ofParameter<float>& fff = para.cast<float>();
			float val01 = (fff - fff.getMin()) / (fff.getMax() - fff.getMin());
			SetParamInfo(i+ _app->parameters.size(), para.getName().c_str(), FF_TYPE_STANDARD, val01);
		}
		else if (type == typeid(ofParameter<string>).name())
		{
			ofParameter<string>& fff = para.cast<string>();
			SetParamInfo(i + _app->parameters.size(), fff.getName().c_str(), FF_TYPE_TEXT, fff->c_str());
		}
	}
}

DWORD ofFFGLPlugin::InitGL(const FFGLViewportStruct *vp)
{
	_ofWin = shared_ptr<ofAppGLFWWindow>(new ofAppGLFWWindow());

	ofInit();
	ofGLFWWindowSettings settings;
	settings.multiMonitorFullScreen = false;
	//settings.setGLVersion(4, 3);
	settings.windowMode = OF_WINDOW;
	settings.width = vp->width;
	settings.height = vp->height;
	settings.visible = false;
	ofGetMainLoop()->addWindow(_ofWin);
	_ofWin->setup(settings);

	ofRunApp(_ofWin, _app);

	//glDisable( GL_DEPTH_TEST );

	/// TODO! 
	/// handle Resouce folder in mac, and .rc files on windoz
	//NSString* dataPath = [NSString stringWithFormat:@"%@/Contents/Resources/Data/", [[NSBundle bundleForClass:[self class]] bundlePath]];
	//ofSetDataPathRoot([dataPath cString]);

	isGLInitialized = true;
	return FF_SUCCESS;
}

DWORD ofFFGLPlugin::DeInitGL()
{
	_app->exit();
	isGLInitialized = false;
	return FF_SUCCESS;
}

void	ofFFGLPlugin::setupInputTextures(ProcessOpenGLStruct* pGL)
{
	for(int i = 0; i < pGL->numInputTextures; i++ )
	{
		// create texture here if it isnt there...
		if( ! _app->inputTextures[i] )
			_app->inputTextures[i] = new ofTexture();
			
		FFGLTextureStruct &tex = *(pGL->inputTextures[i]);
		ofTexture * ofTex = _app->inputTextures[i];
		
		// adapted from ofQC by vade.
		ofTex->texData.textureID = tex.Handle;
		ofTex->texData.textureTarget = GL_TEXTURE_2D;
		ofTex->texData.width = tex.Width;
		ofTex->texData.height = tex.Height;
		ofTex->texData.bFlipTexture = true;
		ofTex->texData.tex_w = tex.HardwareWidth;
		ofTex->texData.tex_h = tex.HardwareHeight;
		ofTex->texData.tex_t = ((float)tex.Width) / tex.HardwareWidth;
		ofTex->texData.tex_u = ((float)tex.Height) / tex.HardwareHeight;
		ofTex->texData.glInternalFormat = GL_RGBA;  // this is just a guess...
		//ofTex->texData.glType = GL_RGBA;
		ofTex->texData.bAllocated = true;
	}
}


DWORD	ofFFGLPlugin::ProcessOpenGL(ProcessOpenGLStruct* pGL)
{
	if(!isGLInitialized)
		return FF_SUCCESS;
		
	_ofWin->update();
	
	setupInputTextures(pGL);

	GLint mmode;
	glGetIntegerv(GL_MATRIX_MODE,&mmode);

	// this could be optimized...alot
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	// draw
	_ofWin->draw();
	
	glPopAttrib();
	glMatrixMode(mmode);

	// we reset the host fbo id here
	// in case we have been rendering offscreen in the plugin.
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,pGL->HostFBO);
	
	// TODO it may be necessary to even do this:
	// but it's a bit of a hack....
	
	/*if( pGL->HostFBO )
	{
		glDrawBuffer(GL_BACK);
		glReadBuffer(GL_BACK);
	}
	else 
	{
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	}
	*/
	
	// ... and it would be better if host handles these things 
	return FF_SUCCESS;
}


DWORD	ofFFGLPlugin::SetTime(double time)
{
	// todo.....
	 return FF_SUCCESS; 
}

DWORD ofFFGLPlugin::GetParameter(DWORD dwIndex)
{
	DWORD dwRet;

	if (dwIndex >= _app->parameters.size())
	{
		dwIndex -= _app->parameters.size();
		ofAbstractParameter& para = _app->paras.get(dwIndex);
		string type = para.type();
		if (type == typeid(ofParameter<float>).name())
		{
			ofParameter<float>& fff = para.cast<float>();
			float val = (fff - fff.getMin()) / (fff.getMax() - fff.getMin());
			*((float *)(unsigned)&dwRet) = val;
			return dwRet;
		}
		else if (type == typeid(ofParameter<string>).name())
		{
			ofParameter<string>& fff = para.cast<string>();
			const char * str = fff->c_str();
			dwRet = (DWORD)str;
			return dwRet;
		}
	}
	else
	{
		ofFFGLParameter * v = _app->getParameter(dwIndex);

		if (!v)
		{
			return FF_FAIL;
		}

		switch (v->getType())
		{
		case PARAM_BOOL:
		case PARAM_EVENT:
		{
			*((float *)(unsigned)&dwRet) = v->getBool();
			return dwRet;
		}

		default:
		{
			return FF_FAIL;
		}
		}
	}
	return FF_FAIL;
}

DWORD ofFFGLPlugin::SetParameter(const SetParameterStruct* pParam)
{
	DWORD vp = pParam->NewParameterValue;

	int idx = pParam->ParameterNumber;

	if (idx >= _app->parameters.size())
	{
		idx -= _app->parameters.size();
		ofAbstractParameter& para = _app->paras.get(idx);
		string type = para.type();
		if (type == typeid(ofParameter<float>).name())
		{
			ofParameter<float>& fff = para.cast<float>();
			float val = *((float *)(unsigned)&vp);
			fff = fff.getMin() + val*(fff.getMax() - fff.getMin());
			return FF_SUCCESS;
		}
		else if (type == typeid(ofParameter<string>).name())
		{
			ofParameter<string>& fff = para.cast<string>();
			char * str = (char*)vp;
			fff = str;
			return FF_SUCCESS;
		}
	}
	else
	{
		ofFFGLParameter * v = _app->getParameter(idx);
		if (!v)
		{
			//debugPrint("ofFFGLPlugin::SetParameter unknown parameter %d\n",pParam->ParameterNumber);
			return FF_FAIL;
		}

		switch (v->getType())
		{
		case PARAM_CSTRING:
		case PARAM_STRING:
		{
			char * str = (char*)vp;
			v->setString(str);
			_app->onParameterChanged(v);

			return FF_SUCCESS;
		}

		case PARAM_BOOL:
		case PARAM_EVENT:
		{
			float val = *((float *)(unsigned)&vp);
			v->setBool((bool)val);
			_app->onParameterChanged(v);
			return FF_SUCCESS;
		}

		default:
		{
			//debugPrint("ofFFGLPlugin::SetParameter  unknown parameter type for %s\n",v->getName().str);
			return FF_FAIL;
		}
		}
	}
	return FF_FAIL;
}

char*	ofFFGLPlugin::GetParameterDisplay(DWORD dwIndex) 
{
	if (dwIndex >= _app->parameters.size())
	{
		dwIndex -= _app->parameters.size();
		ofAbstractParameter& para = _app->paras.get(dwIndex);
		string type = para.type();
		if (type == typeid(ofParameter<float>).name())
		{
			ofParameter<float>& fff = para.cast<float>();
			sprintf(_paramDisplay, "aaa%0.2f", fff.get());
			return _paramDisplay;
		}
		else if (type == typeid(ofParameter<string>).name())
		{
			ofParameter<string>& fff = para.cast<string>();
			return (char*)fff->c_str();
		}
	}
	else
	{
		ofFFGLParameter * v = _app->getParameter(dwIndex);

		if (!v)
		{
			printf("No param!");
			return 0;
		}

		switch (v->getType())
		{
		case PARAM_BOOL:
		case PARAM_EVENT:
			sprintf(_paramDisplay, "%d", v->getBool());
			return _paramDisplay;

		default:;
		}
	}
	return 0;
	
}
	

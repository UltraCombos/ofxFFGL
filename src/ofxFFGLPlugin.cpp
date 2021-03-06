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
#include "ofxFFGLApp.h"

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
	int idx = 0;
	for (size_t j = 0; j < _app->paras.size(); j++)
	{
		ofAbstractParameter& para = _app->paras.get(j);
		string type = para.type();
		int step = 1;
		if (type == typeid(ofParameter<float>).name())
		{
			ofParameter<float>& fff = para.cast<float>();
			float val01 = (fff - fff.getMin()) / (fff.getMax() - fff.getMin());
			SetParamInfo(idx, fff.getName().c_str(), FF_TYPE_STANDARD, val01);
		}
		else if (type == typeid(ofParameter<string>).name())
		{
			ofParameter<string>& fff = para.cast<string>();
			SetParamInfo(idx, fff.getName().c_str(), FF_TYPE_TEXT, fff->c_str());
		}
		else if (type == typeid(ofParameter<bool>).name())
		{
			ofParameter<bool>& fff = para.cast<bool>();
			SetParamInfo(idx, fff.getName().c_str(), FF_TYPE_BOOLEAN, fff);
		}
		else if (type == typeid(_FFGL_event).name())
		{
			ofParameter<bool>& fff = para.cast<bool>();
			SetParamInfo(idx, fff.getName().c_str(), FF_TYPE_EVENT, fff);
		}
		else if (type == typeid(ofParameter<ofFloatColor>).name())
		{
			ofParameter<ofFloatColor>& fff = para.cast<ofFloatColor>();
			SetParamInfo(idx + 0, (fff.getName()+"_R").c_str(), FF_TYPE_RED, fff->r);
			SetParamInfo(idx + 1, (fff.getName()+"_G").c_str(), FF_TYPE_GREEN, fff->g);
			SetParamInfo(idx + 2, (fff.getName()+"_B").c_str(), FF_TYPE_BLUE, fff->b);
			//SetParamInfo(idx + 3, (fff.getName()+"_A").c_str(), 11, fff->a);
			step = 3;
		}
		idx += step;
		SetTimeSupported(true);
		/*
			#define FF_TYPE_RED					2
			#define FF_TYPE_GREEN				3
			#define FF_TYPE_BLUE				4
			#define FF_TYPE_XPOS				5
			#define FF_TYPE_YPOS				6
		*/
	}
}

DWORD ofFFGLPlugin::InitGL(const FFGLViewportStruct *vp)
{
	ofInit();
	ofSetLogLevel(ofLogLevel::OF_LOG_VERBOSE);
	//ofLogToFile(ofVAArgsToString("E:\\logs/%s_log.txt", ofGetTimestampString("%Y-%m-%d-%H-%M-%S").c_str()));
	ofDisableArbTex();

#if 1
	shared_ptr<ofxFFGLWindow> _ofWin = shared_ptr<ofxFFGLWindow>(new ofxFFGLWindow());
	ofGLFWWindowSettings settings;
	settings.multiMonitorFullScreen = false;
	//settings.setGLVersion(4, 3);
	settings.windowMode = OF_WINDOW;
	settings.width = vp->width;
	settings.height = vp->height;
	settings.visible = false;
	ofGetMainLoop()->addWindow(_ofWin);
	_ofWin->setup(settings);
#else
	shared_ptr<ofAppGlutWindow> _ofWin = shared_ptr<ofAppGlutWindow>(new ofAppGlutWindow());
	ofGLWindowSettings settings;
	settings.setGLVersion(4, 3);
	settings.windowMode = OF_WINDOW;
	settings.width = vp->width;
	settings.height = vp->height;
	ofGetMainLoop()->addWindow(_ofWin);
	_ofWin->setup(settings);
	//_ofWin->disableSetupScreen();
#endif

	ofRunApp(_ofWin, _app);

	/// TODO! 
	/// handle Resouce folder in mac, and .rc files on windoz
	//NSString* dataPath = [NSString stringWithFormat:@"%@/Contents/Resources/Data/", [[NSBundle bundleForClass:[self class]] bundlePath]];
	//ofSetDataPathRoot([dataPath cString]);

	isGLInitialized = true;
	return FF_SUCCESS;
}

DWORD ofFFGLPlugin::DeInitGL()
{
	ofGetMainLoop()->exit();
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
namespace
{
	template<typename T>
	inline void set_default_fbo_id(shared_ptr<ofBaseRenderer> r, GLuint fbo_id)
	{
		if (dynamic_cast<T*>(r.get()) != nullptr)
			dynamic_cast<T*>(r.get())->defaultFramebufferId = dynamic_cast<T*>(r.get())->currentFramebufferId = fbo_id;
	}

	inline void set_default_fbo_id(shared_ptr<ofBaseRenderer> r, GLuint fbo_id)
	{
		set_default_fbo_id<ofGLRenderer>(r, fbo_id);
		set_default_fbo_id<ofGLProgrammableRenderer>(r, fbo_id);
	}
}
DWORD	ofFFGLPlugin::ProcessOpenGL(ProcessOpenGLStruct* pGL)
{
	if(!isGLInitialized)
		return FF_SUCCESS;

	set_default_fbo_id(ofGetMainLoop()->getCurrentWindow()->renderer(), pGL->HostFBO);
	setupInputTextures(pGL);
	// this could be optimized...alot
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	// draw

	ofGetMainLoop()->loopOnce();

	glPopAttrib();
	// we reset the host fbo id here
	// in case we have been rendering offscreen in the plugin.
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pGL->HostFBO);
	
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
	//ofLogVerbose("ofFFGLPlugin", "%f", time);
}

DWORD ofFFGLPlugin::GetParameter(DWORD dwIndex)
{
	DWORD dwRet = FF_FAIL;

	//if (dwIndex >= _app->paras.size())
	//	return FF_FAIL;
	if(dwIndex >= GetNumParams())
		return FF_FAIL;

	int count_idx = 0;
	int para_idx = 0;
	for (int i = 0; i < _app->paras.size(); ++i)
	{
		int step = 1;
		ofAbstractParameter& para = _app->paras.get(i);
		string type = para.type();
		if (type == typeid(ofParameter<ofFloatColor>).name())
			step = 3;

		if (dwIndex >= count_idx && dwIndex < count_idx + step)
		{
			para_idx = i;
			break;
		}
		count_idx += step;
	}

	//ofAbstractParameter& para = _app->paras.get(dwIndex);
	ofAbstractParameter& para = _app->paras.get(para_idx);
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
		//dwRet = (DWORD)str;
		return dwRet;
	}
	else if (type == typeid(ofParameter<bool>).name() || type == typeid(_FFGL_event).name())
	{
		ofParameter<bool>& fff = para.cast<bool>();
		*((float *)(unsigned)&dwRet) = fff;
		return dwRet;
	}
	else if (type == typeid(ofParameter<ofFloatColor>).name())
	{
		ofParameter<ofFloatColor>& fff = para.cast<ofFloatColor>();
		*((float *)(unsigned)&dwRet) = fff.get().v[dwIndex - count_idx];
		return dwRet;
	}

	return FF_FAIL;
}

DWORD ofFFGLPlugin::SetParameter(const SetParameterStruct* pParam)
{
	DWORD vp = pParam->NewParameterValue;

	int idx = pParam->ParameterNumber;

	//if (idx >= _app->paras.size())
	//	return 0;
	if (idx >= GetNumParams())
		return FF_FAIL;

	int count_idx = 0;
	int para_idx = 0;
	for (int i = 0; i < _app->paras.size(); ++i)
	{
		int step = 1;
		ofAbstractParameter& para = _app->paras.get(i);
		string type = para.type();
		if (type == typeid(ofParameter<ofFloatColor>).name())
			step = 3;

		if (idx >= count_idx && idx < count_idx + step)
		{
			para_idx = i;
			break;
		}
		count_idx += step;
	}

	ofAbstractParameter& para = _app->paras.get(para_idx);
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
	else if (type == typeid(ofParameter<bool>).name() || type == typeid(_FFGL_event).name())
	{
		ofParameter<bool>& fff = para.cast<bool>();
		float val = *((float *)(unsigned)&vp);
		fff = (bool)val;
		return FF_SUCCESS;
	}
	else if (type == typeid(ofParameter<ofFloatColor>).name())
	{
		ofParameter<ofFloatColor>& fff = para.cast<ofFloatColor>();
		float val = *((float *)(unsigned)&vp);
		int iii = idx - count_idx;
		ofFloatColor tmp = fff.get();
		if (iii == 0)
			tmp.set(ofFloatColor(val, fff->g, fff->b, fff->a));
		else if (iii == 1)
			tmp.set(ofFloatColor(fff->r, val, fff->b, fff->a));
		else if (iii == 2)
			tmp.set(ofFloatColor(fff->r, fff->g, val, fff->a));
		//else if (iii == 3)
//			tmp.set(ofFloatColor(fff->r, fff->g, fff->b, val));
		fff = tmp;
		
		return FF_SUCCESS;
	}

	return FF_FAIL;
}
// 
// char*	ofFFGLPlugin::GetParameterDisplay(DWORD dwIndex)
// {
// 	if (dwIndex >= _app->paras.size())
// 		return 0;
// 
// 	ofAbstractParameter& para = _app->paras.get(dwIndex);
// 	string type = para.type();
// 	if (type == typeid(ofParameter<float>).name())
// 	{
// 		ofParameter<float>& fff = para.cast<float>();
// 		sprintf(_paramDisplay, "aaa %0.2f", fff.get());
// 		return _paramDisplay;
// 	}
// 	else if (type == typeid(ofParameter<string>).name())
// 	{
// 		ofParameter<string> fff = para.cast<string>();
// 		sprintf(_paramDisplay, "%s", fff->c_str());
// 		return _paramDisplay;
// 	}
// 	else if (type == typeid(ofParameter<bool>).name() || type == typeid(_FFGL_event).name())
// 	{
// 		ofParameter<bool>& fff = para.cast<bool>();
// 		sprintf(_paramDisplay, "%d", fff);
// 		return _paramDisplay;
// 	}
// 
// 	return 0;
// }
// 	

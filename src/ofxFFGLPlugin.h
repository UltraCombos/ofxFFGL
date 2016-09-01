/*
 *  ofxFFGLPlugin.h
 *
 *	Created by Daniel Berio 2010
 */

#ifndef _OFXFFGLPLUGIN
#define _OFXFFGLPLUGIN


#include "ofxFFGLApp.h"
#include "ofAppGLFWWindow.h"
#include "FFGLPluginInfo.h"

#include <FFGL.h>
#include <FFGLLib.h>
#include "FFGLPluginSDK.h"

/// Plugin creation call. With Visual Studio __stdcall does not compile
#define DEFINE_FFGL_PLUGIN( appType, minInputs, maxInputs )  \
static DWORD __stdcall createPluginInstance(CFreeFrameGLPlugin **ppInstance) \
{  \
ofFFGLPlugin * plug = new ofFFGLPlugin(new appType(), minInputs, maxInputs); \
*ppInstance = plug; \
return FF_SUCCESS; \
}

class  ofFFGLPlugin: public CFreeFrameGLPlugin
{
public:
	ofFFGLPlugin( ofFFGLApp * app, int minInputs = 1, int maxInputs = 1 );
	~ofFFGLPlugin();
	
	void initParameters();
	
  	DWORD InitGL(const FFGLViewportStruct *vp);
	DWORD DeInitGL();


	DWORD	ProcessOpenGL(ProcessOpenGLStruct* pGL);
	void	setupInputTextures(ProcessOpenGLStruct* pGL);
	
	DWORD	SetTime(double time);
	
	DWORD	SetParameter(const SetParameterStruct* pParam);		
	DWORD	GetParameter(DWORD dwIndex);					
	//char*	GetParameterDisplay(DWORD dwIndex) override;
	
protected:
	char _paramDisplay[100];
	
	bool isGLInitialized;
	
	shared_ptr<ofFFGLApp>  	_app;
	shared_ptr<ofAppGLFWWindow>  _ofWin;

	float m_brightness;
private:
};

#endif

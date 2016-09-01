# ofxFFGL
- this is a work in progress, the main focus wil be on Windows development.
- supports oF 0.9.3
- for now, you need to comment out line 97 in ofAppRunner.cpp, or the compiled .dll will not run in Arena/Avenue.
- this work is based on https://github.com/sambenito/ofxffglplugin by Daniel Berio <http://www.enist.org>

------------------------------------------------------------------------------------------------
openFrameworks addon for creating FFGL ( FreeFrame 1.5 ) plugins

At the moment XCode 3.0 and Visual Studio Express 2015 projects are present.

The windows version of openFrameworks needs to be linked with some dll's that will be automatically copyed in the bin folder after compilation. To get the plugin to work these .dll's must be copyed either into the SYSTEM32 folder or in the same folder as the plugin.

Mouse and keyboard callbacks are not supported.

Color parameters are not yet supported, but you can easily use float instead.

------------------------------------------------------------------------------------------------

## To create your own ofFFGL project:
> copy one of the example projects.

### in main.cpp:
1. In the `DEFINE_FFGL_PLUGIN` macro 
	change testApp with your App class name, if it is different.
	And set min and max input textures.
	
2. Customize the CFFGLPluginInfo struct:

```
static CFFGLPluginInfo PluginInfo (
	createPluginInstance,				// * Default creation method ( created with the DEFINE_FFGL_PLUGIN macro )
	
	"OFEX",								// Plugin unique ID. This must be different for every plugin and 4 characters long.
	"OFTest1",							// Plugin name
	
	1,									// * API major version number
	500,								// * API minor version number
	1,									// Plugin major version number
	000,								// Plugin minor version number
	
	FF_SOURCE,							// Plugin type FF_SOURCE or FF_EFFECT ( filters ) 
	"Test OpenFrameworks Plugin",		// Plugin description
	"by Daniel Berio - www.enist.org"	// About
);
```

### It is important that every plugin has a 4 character identifier. This must be different for every FFGL plugin


------------------------------------------------------------------------------------------------


The App class MUST be derived from ofFFGLApp.
If you want to convert an existing openFrameworks app to FFGL, simply change the derivation from ofBaseApp to ofFFGLApp.


------------------------------------------------------------------------------------------------


### To add custom parameters go to the constructor of the class, and use the custom functions in ofFFGLApp:

```
addParameter( ofAbstractParameter& para );
```

#### Once the plugin is compiled, move the .bundle ( on Mac ) or the .dll file ( on Windows ) to the plugin directory of the host.

-------------------------------------------------------------------------------------------------

For an FFGL host, check out Resolume @ <http://www.resolume.com>
In the Video Preferences you can specify the directories where resolume searches for plugins at startup.

For a openFrameworks FreeFrame host check out 
<http://github.com/prisonerjohn/openFrameworks/tree/master/addons/ofxFreeFrame/>

For more info in FFGL:
<http://freeframe.sourceforge.net/>

#ifndef _TEST_APP
#define _TEST_APP

#include "ofxFFGLApp.h"
#include "ofParameter.h"

class testApp : public ofFFGLApp{
	
	public:
		testApp();
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		
		ofParameter<float> numRepeats = 3;
		ofParameter<float> bbbbb = 3;
		ofParameter<bool> _bool_para = true;
		ofParameter<bool> _event_para;
		ofParameter<string> _string_para = "";

		void _on_event(bool& b)
		{
			if (!b)
				return;

			_string_para += "event!!! ";
			if (_string_para->length() > 50)
				_string_para = "";
		}

		ofFbo fbo;
};

#endif
	

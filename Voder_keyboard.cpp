/*=================================
 * Formant Filter Example
 *
 * Voder2017
 * Oct 2017
 *
 *=================================*/

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Window.h"
#include "FormSwep.h"  // Sweepable Formant Filter
#include "Noise.h"     // White Noise
#include "BlitSaw.h"   // Sawtooth
#include "RtAudio.h"   // Real Time Audio
#include <iostream>    // I/O

using namespace stk;

// Container for Sound Generators
struct SoundGens {
  int mode;
  FormSwep * vox;
  Noise * hiss;
  BlitSaw * buzz;
};

// Callback Function for Realtime Audio
int tick( void * outBuff, void * inBuff, unsigned int nBuffFrames,
	  double streamTime, RtAudioStreamStatus status, void * dataPtr ) {
  SoundGens * sg = (SoundGens *) dataPtr;

  register StkFloat * samples = (StkFloat *) outBuff;
  StkFloat val;
  
  for ( unsigned int i=0; i < nBuffFrames; ++i ) {
    if ( sg->mode == 0 ) { // silent
      *samples++ = 0;
      *samples++ = 0;
    } else if ( sg->mode == 1 ) { // hiss
      val = sg->hiss->tick(); 
      *samples++ = val;
      *samples++ = val;
    } else if ( sg->mode == 2 ) { // buzz
      val = sg->buzz->tick(); 
      *samples++ = val;
      *samples++ = val;
    } else if ( sg->mode == 3 ) { // formant filter hiss
      val = sg->vox->tick( sg->hiss->tick() );
      *samples++ = val;
      *samples++ = val;
    } else if ( sg->mode == 4 ) { // formant filter buzz
      val = sg->vox->tick( sg->buzz->tick() );
      *samples++ = val;
      *samples++ = val;
    }
  }

  return 0;
}

void error_callback( int error, const char * description );
static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods );

int main( int argc, char ** argv ) {

	// Initialize GLFW 
	if (!glfwInit()) {
		std::cerr << "GLFW could NOT init!" << std::endl;
		return -1;
	}

	// Set error callback for GLFW
	glfwSetErrorCallback(error_callback);

	
	// Create Window
	gui::Window * win = new gui::Window("Voder Keyboard Example", 640, 480);

	// Initialize GLEW
	glewInit();
	/*	if ( glewInit() != GLEW_OKAY ) {
		std::cerr << "GLEW could not init!" << std::endl;
		}*/

	// Set keyboard callback
	glfwSetKeyCallback(win->getWindowPtr(), key_callback);
    	
  // Set global sample rate
  Stk::setSampleRate( 44100.0 );

  // Setup RT Audio
  RtAudio dac;
  
  // Make noise generators
  FormSwep vox;
  Noise hiss;
  BlitSaw buzz;
  
  // Setup generators
  buzz.setFrequency( 200 );
  vox.setStates( 800, 0.0, 1.0 );  

  // Pass sound generators into container
  SoundGens base;
  base.mode = 0;
  base.vox = &vox;
  base.hiss = &hiss;
  base.buzz = &buzz;
  

  // Setup stream
  RtAudio::StreamParameters params;
  params.deviceId = dac.getDefaultOutputDevice();
  params.nChannels = 2;
  // check if 64 or 32 bit
  RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
  unsigned int bufferFrames = RT_BUFFER_SIZE;
  try {
    dac.openStream( &params, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&base );
  } catch ( RtAudioError &error ) {
    error.printMessage();
    goto cleanup;
  }

  // Run Stream
  try {
    dac.startStream();
  } catch ( RtAudioError &error ) {
    error.printMessage();
    goto cleanup;
  }

	// Core loop
	while (!win->windowShouldClose()) {
			win->clear();
			win->update();

			if ( win->isKeyPressed( GLFW_KEY_SPACE ) ) {
				base.mode = 0;
			} else if ( win->isKeyPressed( GLFW_KEY_A ) ) {
				base.mode = 1;
			} else if ( win->isKeyPressed( GLFW_KEY_S ) ) {
				base.mode = 2;
			} else if ( win->isKeyPressed( GLFW_KEY_D ) ) {
				base.mode = 3;
			} else if ( win->isKeyPressed( GLFW_KEY_F ) ) {
				base.mode = 4;
			}
	}
  
  // Shutdown Stream
  try {
    dac.closeStream();
  } catch ( RtAudioError &error ) {
    error.printMessage();
  }

 cleanup:
	delete win;
  return 0;
}


void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    
		gui::Window* w = (gui::Window*)glfwGetWindowUserPointer(window);
    
    if (action != GLFW_RELEASE)
        w->setKey(key, true);
    else 
        w->setKey(key, false);
    
}


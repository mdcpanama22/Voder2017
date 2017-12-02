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
  Noise * hiss;         // hiss generator
  BlitSaw * buzz;       // buzz generator
  int mode;             // hiss = 0, buzz = 1
  int * button_states;  // buttons states
  FormSwep ** keys;     // Formant filters
};

// Callback Function for Realtime Audio
int tick( void * outBuff, void * inBuff, unsigned int nBuffFrames,
	  double streamTime, RtAudioStreamStatus status, void * dataPtr ) {
  SoundGens * sg = (SoundGens *) dataPtr;
  
  register StkFloat * samples = (StkFloat *) outBuff;
  StkFloat val, hval, bval;

  for ( unsigned int i=0; i < nBuffFrames; ++i ) {
    val = 0;
    hval = sg->hiss->tick();
    bval = sg->buzz->tick();
    for ( unsigned int j = 0; j < 10; ++j ) {
      // Each filter
      if ( sg->button_states[j] == 0 ) {
				// filter off
      } 
      else if ( sg->button_states[j] == 3 ) { // High filter (q-p)
				if ( sg->mode == 0 ) {
				  //val += sg->hiss->tick();
				  val += sg->keys[j]->tick( hval );
				} 
				else {
				  //val += sg->buzz->tick();
				  val += sg->keys[j]->tick( bval );
				}
      }
    }
    
    // Output value of all active filters
    *samples++ = val;
    *samples++ = val;
    //std::cout << val << " (h:" << hval <<", b:" << bval << ")" << std::endl;
  }

  return 0;
}

void set_up( SoundGens & sg );
void clean_up( SoundGens & sg );

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
  
  // Set keyboard callback
  glfwSetKeyCallback(win->getWindowPtr(), key_callback);

  
  // Set global sample rate
  Stk::setSampleRate( 48000.0 );
  
  // Setup RT Audio
  RtAudio dac;

  // Setup SoundGens and Filters
  SoundGens base;
  set_up( base );
   
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

    // Check which keys are being pressed
    if ( win->isKeyPressed( GLFW_KEY_SPACE ) ) {
      base.mode = 1;
    } else {
      base.mode = 0;
    }

    if ( win->isKeyPressed( GLFW_KEY_Q ) ) {
      base.button_states[0] = 3;
    } else {
      base.button_states[0] = 0;
    }
    if ( win->isKeyPressed( GLFW_KEY_W ) ) {
      base.button_states[1] = 3;
    } else {
      base.button_states[1] = 0;
    }
    if ( win->isKeyPressed( GLFW_KEY_E ) ) {
      base.button_states[2] = 3;
    } else {
      base.button_states[2] = 0;
    }
    if ( win->isKeyPressed( GLFW_KEY_R ) ) {
      base.button_states[3] = 3;
    } else {
      base.button_states[3] = 0;
    }
    if ( win->isKeyPressed( GLFW_KEY_T ) ) {
      base.button_states[4] = 3;
    } else {
      base.button_states[4] = 0;
    }
    if ( win->isKeyPressed( GLFW_KEY_Y ) ) {
      base.button_states[5] = 3;
    } else {
      base.button_states[5] = 0;
    }
    if ( win->isKeyPressed( GLFW_KEY_U ) ) {
      base.button_states[6] = 3;
    } else {
      base.button_states[6] = 0;
    }
    if ( win->isKeyPressed( GLFW_KEY_I ) ) {
      base.button_states[7] = 3;
    } else {
      base.button_states[7] = 0;
    }
    if ( win->isKeyPressed( GLFW_KEY_O ) ) {
      base.button_states[8] = 3;
    } else {
      base.button_states[8] = 0;
    }
    if ( win->isKeyPressed( GLFW_KEY_P ) ) {
      base.button_states[9] = 3;
    } else {
      base.button_states[9] = 0;
    }


  }
  
  // Shutdown Stream
  try {
    dac.closeStream();
  } catch ( RtAudioError &error ) {
    error.printMessage();
  }
  
 cleanup:
  clean_up( base );
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

void set_up( SoundGens & sg ) {
  // Make noise generators
  Noise * hiss = new Noise();
  BlitSaw * buzz = new BlitSaw();
  
  // Setup generators
  buzz->setFrequency( 200 );
  
  // Make button_states
  int * bs = new int[10];   // *** Currently only 10 filters
  for ( unsigned int i = 0; i < 10; ++i ) {
    bs[i] = 0;
  }

  // Make Formant Filters
  FormSwep ** filter = new FormSwep*[10];
  for ( unsigned int i = 0; i < 10; ++i ) {
    filter[i] = new FormSwep();
  }

  // Setup formant filters
  filter[0]->setStates(  225, 0.9, 1.0 ); // 000 - 225
  filter[1]->setStates(  450, 0.9, 1.0 ); // 255 - 450
  filter[2]->setStates(  700, 0.9, 1.0 ); // 450 - 700
  filter[3]->setStates( 1000, 0.9, 1.0 ); // 700 - 1000
  filter[4]->setStates( 1400, 0.9, 1.0 ); // 1000 - 1400
  filter[5]->setStates( 2000, 0.9, 1.0 ); // 1400 - 2000
  filter[6]->setStates( 2700, 0.9, 1.0 ); // 2000 - 2700
  filter[7]->setStates( 3800, 0.9, 1.0 ); // 2700 - 3800
  filter[8]->setStates( 5400, 0.9, 1.0 ); // 3800 - 5400
  filter[9]->setStates( 7500, 0.9, 1.0 ); // 5400 - 7500

  // Pass sound generators into container
  sg.hiss = hiss;
  sg.buzz = buzz;
  sg.button_states = bs;
  sg.keys = filter;
  sg.mode = 0;
}

void clean_up( SoundGens & sg ) {

  for ( unsigned int i = 0; i < 10; ++i ) {
    delete sg.keys[i]; 
  }
  delete[] sg.keys;
  delete[] sg.button_states;
  delete sg.hiss;
  delete sg.buzz;
  
}


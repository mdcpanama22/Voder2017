/************************
 * Voder Header
 *
 * Feb 2018
 * 
 ************************/

#ifndef __Voder_h__
#define __Voder_h__

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
	float pitch;          // frequency for buzz
	float volume;         // percent voume multiplier
  int mode;             // hiss = 0, buzz = 1
  int * button_states;  // buttons states
  FormSwep ** keys;     // Formant filters
};

// ===================================================
// Prototypes:
// ===================================================

void set_up( SoundGens & sg );     // Setup Voder Struct
void clean_up( SoundGens & sg );   // Deallocate Voder Struct

// GUI functions
void error_callback( int error, const char * description );
static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods );

void read_keys( gui::Window * win, SoundGens & base );

void chart();

// Callback Function for Realtime Audio
int tick( void * outBuff, void * inBuff, unsigned int nBuffFrames,
					double streamTime, RtAudioStreamStatus status, void * dataPtr );

// ===================================================
// Functions:
// ===================================================


// Callback Function for Realtime Audio
// Runs filters for sample depending on 
int tick( void * outBuff, void * inBuff, unsigned int nBuffFrames,
	  double streamTime, RtAudioStreamStatus status, void * dataPtr ) {
  SoundGens * sg = (SoundGens *) dataPtr;
  
  register StkFloat * samples = (StkFloat *) outBuff;
  StkFloat val, hval, bval;

	sg->buzz->setFrequency( sg->pitch );
	
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
				  val += sg->keys[j]->tick( hval );
				} 
				else {
				  val += sg->keys[j]->tick( bval );
				}
      }
    }
    
    // Output value of all active filters
		val *= sg->volume;
    *samples++ = val;
    *samples++ = val;
    //std::cout << val << " (h:" << hval <<", b:" << bval << ")" << std::endl;
  }

  return 0;
}


void set_up( SoundGens & sg ) {
  // Make noise generators
  Noise * hiss = new Noise();
  BlitSaw * buzz = new BlitSaw();

	// Set pitch and volume
	sg.pitch = 200;
	sg.volume = 1.0;
	
  // Setup generators
  buzz->setFrequency( sg.pitch );
  
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
  filter[0]->setStates(  225, 0.99, 1.0 ); // 000 - 225
  filter[1]->setStates(  450, 0.99, 1.0 ); // 255 - 450
  filter[2]->setStates(  700, 0.99, 1.0 ); // 450 - 700
  filter[3]->setStates( 1000, 0.99, 1.0 ); // 700 - 1000
  filter[4]->setStates( 1400, 0.99, 1.0 ); // 1000 - 1400
  filter[5]->setStates( 2000, 0.99, 1.0 ); // 1400 - 2000
  filter[6]->setStates( 2700, 0.99, 1.0 ); // 2000 - 2700
  filter[7]->setStates( 3800, 0.99, 1.0 ); // 2700 - 3800
  filter[8]->setStates( 5400, 0.99, 1.0 ); // 3800 - 5400
  filter[9]->setStates( 7500, 0.99, 1.0 ); // 5400 - 7500

  // Pass sound generators into container
  sg.hiss = hiss;
  sg.buzz = buzz;
  sg.button_states = bs;
  sg.keys = filter;
  sg.mode = 0;
}

// Deallocate SoundGens struct
void clean_up( SoundGens & sg ) {

  for ( unsigned int i = 0; i < 10; ++i ) {
    delete sg.keys[i]; 
  }
  delete[] sg.keys;
  delete[] sg.button_states;
  delete sg.hiss;
  delete sg.buzz;
  
}

//Error callback for GUI
void error_callback(int error, const char* description) {
  std::cerr << "Error: " << description << std::endl;
}

//Key callback for GUI
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
  gui::Window* w = (gui::Window*)glfwGetWindowUserPointer(window);
  
  if (action != GLFW_RELEASE) {
    w->setKey(key, true);
  } else { 
    w->setKey(key, false);
  }
}

void read_keys( gui::Window * win, SoundGens & base ) {
	float pd = 10.0;
	float pv = 0.05;
	
	if ( win->isKeyPressed( GLFW_KEY_SPACE ) ) {
		base.mode = 0;
	} else {
		base.mode = 1;
	}


	if ( win->isKeyPressed( GLFW_KEY_UP ) ) {
		base.pitch += pd;
	}
	if ( win->isKeyPressed( GLFW_KEY_DOWN ) ) {
		base.pitch = (base.pitch - pd > 0) ? base.pitch - pd : base.pitch;
	}
	if ( win->isKeyPressed( GLFW_KEY_RIGHT ) ) {
		base.volume = (base.volume + pv < 3.0) ? base.volume - pv : base.volume;
	}
	if ( win->isKeyPressed( GLFW_KEY_LEFT ) ) {
		base.volume = (base.volume - pv > 0) ? base.volume - pv : base.volume;
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

// Display key bindings
void chart() {
	std::cout << "Voder keys:" << std::endl;
	std::cout << "Q-P Filters" << std::endl;
	std::cout << "UP/DOWN - Pitch" << std::endl;
	std::cout << "LEFT/RIGHT - Volume" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Vowels" << std::endl;
	std::cout << "kit \tWUI" << std::endl;
	std::cout << "dress \tEUI" << std::endl;
	std::cout << "trap \tRYU" << std::endl;
	std::cout << "lot \tERU" << std::endl;
	std::cout << "strut \tRTU" << std::endl;
	std::cout << "foot \tWRU" << std::endl;
	std::cout << "fleece \tWUI" << std::endl;
	std::cout << "goose \tWRU" << std::endl;
	std::cout << "start \tRTU" << std::endl;
	std::cout << "law \tERU" << std::endl;
	std::cout << "stir \tEYU" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Consonants" << std::endl;
	std::cout << "soon \tiop" << std::endl;
	std::cout << "zero \tqio" << std::endl;
	std::cout << "ship \tyui" << std::endl;
	std::cout << "vision \tqyu" << std::endl;
	std::cout << "hot \tUnvox next vowel" << std::endl;
	std::cout << "f,v,th \tq?" << std::endl;
	std::cout << "" << std::endl;
	
}


#endif


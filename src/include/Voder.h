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
#include "../graphics/Window.h"
#include "FormSwep.h"  // Sweepable Formant Filter
#include "Noise.h"     // White Noise
#include "BlitSaw.h"   // Sawtooth
#include "RtAudio.h"   // Real Time Audio
#include <iostream>    // I/O

using namespace stk; //WHY is there a using namespace in a header file??

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

// Keyboard functions
void read_keys( const core::graphics::Window * win, SoundGens & base );
int trinary_button( const core::graphics::Window * win, int key1, int key2, int key3 );

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
  StkFloat val, hval, bval, loudness;
  
  sg->buzz->setFrequency( sg->pitch );
	
  for ( unsigned int i=0; i < nBuffFrames; ++i ) {
    val = 0;
    hval = sg->hiss->tick();
    bval = sg->buzz->tick();

    //TODO: if plosive active set mode and switch after timer runs out
    for ( unsigned int j = 0; j < 10; ++j ) {
      // Each filter
      if ( sg->button_states[j] != 0 ) {
	  loudness = sg->button_states[j] / 3.0;
	  if ( sg->mode == 0 ) {
	      val += sg->keys[j]->tick( hval ) * loudness;
	  } 
	  else {
	      val += sg->keys[j]->tick( bval ) * loudness;
	  }
      }
    }
    if ( sg->button_states[10] != 0 ) {
	val = 0;
    } else if ( sg->button_states[11] != 0 ) {
	val = 0;
    } else if ( sg->button_states[12] != 0 ) {
	val = 0;
    }
    
    // Output value of all active filters
    val *= (StkFloat) sg->volume;
    *samples++ = val;
    *samples++ = val;
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
  int * bs = new int[13];   // 10 Filters and 3 stop consonants
  for ( unsigned int i = 0; i < 13; ++i ) {
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


void read_keys( const core::graphics::Window * win, SoundGens & base ) {
    float pd = 5.0;
    float pv = 0.02;
    
    base.mode = (win->isKeyPressed( GLFW_KEY_SPACE ) ) ? 0 : 1;
    
    // Pitch
    if ( win->isKeyPressed( GLFW_KEY_UP ) ) {
	base.pitch += dp;
    }
    if ( win->isKeyPressed( GLFW_KEY_DOWN ) ) {
	base.pitch = (base.pitch - dp > 0) ? base.pitch - dp : base.pitch;
    }
    // Volume
    if ( win->isKeyPressed( GLFW_KEY_RIGHT ) ) {
	base.volume = (base.volume + dv < 3.0) ? base.volume + dv : base.volume;
    }
    if ( win->isKeyPressed( GLFW_KEY_LEFT ) ) {
	base.volume = (base.volume - dv > 0) ? base.volume - dv : base.volume;
    }

    // Reset
    if ( win->isKeyPressed( GLFW_KEY_0 ) ) {
	base.volume = 1.0;
	base.pitch = 200;
    }
    
    // Left Hand
    base.button_states[0] = trinary_button( win, GLFW_KEY_Z, GLFW_KEY_A, GLFW_KEY_Q );
    base.button_states[1] = trinary_button( win, GLFW_KEY_X, GLFW_KEY_S, GLFW_KEY_W );
    base.button_states[2] = trinary_button( win, GLFW_KEY_C, GLFW_KEY_D, GLFW_KEY_E );
    base.button_states[3] = trinary_button( win, GLFW_KEY_V, GLFW_KEY_F, GLFW_KEY_R );
    base.button_states[4] = trinary_button( win, GLFW_KEY_B, GLFW_KEY_G, GLFW_KEY_T );
    // Right Hand
    base.button_states[5] = trinary_button( win, GLFW_KEY_N, GLFW_KEY_H, GLFW_KEY_Y );
    base.button_states[6] = trinary_button( win, GLFW_KEY_M, GLFW_KEY_J, GLFW_KEY_U );
    base.button_states[7] = trinary_button( win, GLFW_KEY_COMMA, GLFW_KEY_K, GLFW_KEY_I );
    base.button_states[8] = trinary_button( win, GLFW_KEY_PERIOD, GLFW_KEY_L, GLFW_KEY_O );
    base.button_states[9] = trinary_button( win, GLFW_KEY_SLASH, GLFW_KEY_SEMICOLON, GLFW_KEY_P );
    // Plosives
    base.button_states[10] = (win->isKeyPressed( GLFW_KEY_1 ) ) ? 1 : 0;
    base.button_states[11] = (win->isKeyPressed( GLFW_KEY_2 ) ) ? 1 : 0;
    base.button_states[12] = (win->isKeyPressed( GLFW_KEY_3 ) ) ? 1 : 0;
    // Notes:
    // when released start timer and switch timer
    // timer counts down to end
    // switch timer is how long to switch from hiss to buzz
    
}

int trinary_button( const core::graphics::Window * win, int key1, int key2, int key3 ) {
    if ( win->isKeyPressed( key1 ) ) {
	return 1;
    } if ( win->isKeyPressed( key2 ) ){
	return 2;
    } if ( win->isKeyPressed( key3 ) ) {
	return 3;
    }
    return 0;
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


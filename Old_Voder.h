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
#include "Old_Window.h"
#include "FormSwep.h"  // Sweepable Formant Filter
#include "Noise.h"     // White Noise
#include "BlitSaw.h"   // Sawtooth
#include "RtAudio.h"   // Real Time Audio
#include <iostream>    // I/O
#include <vector>

#define STK_SAMPLE_RATE 48000.0

#define DEFAULT_FREQ 200
#define DELTA_PITCH 0.05 
#define DELTA_VOLUME 0.005

using namespace stk;

// Struct for Plosive Data
struct PlosiveData {
  std::vector<int> pre_keys;
  std::vector<int> post_keys;
  int t_switch;
  int t_end;
};

// Container for Sound Generators
struct SoundGens {
  Noise * hiss;         // hiss generator
  BlitSaw * buzz;       // buzz generator
  float pitch;          // frequency for buzz
  float volume;         // voume multiplier (%)
  int mode;             // hiss = 0, buzz = 1
  int active_plosive;   // currently active plosive
  long timer;           // tick for plosives
  int * prev_buttons;   // buttons states
  int * curr_buttons; 
  FormSwep ** keys;     // Formant filters
  std::vector<PlosiveData*> plos; // Plosive Data
};

// ===================================================
// Prototypes:
// ===================================================

void set_up( SoundGens & sg );     // Setup Voder Struct
void clean_up( SoundGens & sg );   // Deallocate Voder Struct

// GUI functions
void error_callback( int error, const char * description );
static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods );

// Keyboard functions
void read_keys( gui::Window * win, SoundGens & base );
int trinary_button( gui::Window * win, int key1, int key2, int key3 );

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
  StkFloat val, gen_val, loudness;
  
  sg->buzz->setFrequency( sg->pitch );

  bool silent = sg->curr_buttons[10] != 0 ||
    sg->curr_buttons[11] != 0 || sg->curr_buttons[12] != 0;
  int tmp_timer = sg->timer;
  
  for ( unsigned int n = 0; n < nBuffFrames; ++n ) {
    val = 0;
    if ( silent ) { val = 0; } // ===> Do nothing

    // === Run Plosive ===
    if ( !silent && sg->active_plosive != 0 ) {
      int idx = sg->active_plosive;
      long t_switch = sg->plos[idx]->t_switch;
      long t_end = sg->plos[idx]->t_end;
      if ( tmp_timer < t_switch ) { // Voiceless Phase
	gen_val = sg->hiss->tick();
	for ( unsigned k = 0; k < sg->plos[idx]->pre_keys.size(); k+=2 ) {
	  loudness = sg->plos[idx]->pre_keys[k+1] / 10.0;
	  int key_idx = sg->plos[idx]->pre_keys[k];
	  val += sg->keys[key_idx]->tick( gen_val );// * loudness;
	}
	val *= 2.0;
	++tmp_timer;
	sg->timer = (n==0) ? sg->timer+1 : sg->timer;
      } else if ( tmp_timer < t_switch + t_end ) { // Voiced Phase
	gen_val = sg->buzz->tick();
	for ( unsigned k = 0; k < sg->plos[idx]->post_keys.size(); k+=2 ) {
	  loudness = sg->plos[idx]->post_keys[k+1] / 10.0;
	  int key_idx = sg->plos[idx]->post_keys[k];
	  val += sg->keys[key_idx]->tick( gen_val );// * loudness;
	}
	val *= 2.0;
	++tmp_timer;
	sg->timer = (n==0) ? sg->timer+1 : sg->timer;
      } else { // Deactivate plosive
	sg->active_plosive = 0;
	sg->timer = 0;
      }
    }

    // === Normal Functioning ===
    if ( !silent && sg->active_plosive == 0 ) {
      gen_val = ( sg->mode == 0 ) ? sg->hiss->tick() : sg->buzz->tick();
      for ( int k = 0; k < 10; ++k ) {
	if ( sg->curr_buttons[k] != 0 ) {
	  loudness = sg->curr_buttons[k] / 10.0;
	  val += sg->keys[k]->tick( gen_val ) * loudness;
	}
      }
    }

    // === Output results ===
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

  // Set mode, pitch, and volume
  sg.mode = 0;
  sg.pitch = DEFAULT_FREQ;
  sg.volume = 1.0;

  // Set plosive variables
  sg.active_plosive = 0;
  sg.timer = 0;
	
  // Setup generators
  buzz->setFrequency( sg.pitch );
  
  // Make button_states
  int * bs = new int[13];   // 10 Filters and 3 stop consonants
  int * bs2 = new int[13];
  for ( unsigned int i = 0; i < 13; ++i ) {
    bs[i] = 0;
    bs2[i] = 0;
  }

  // Make Formant Filters
  FormSwep ** filter = new FormSwep*[10];
  for ( unsigned int i = 0; i < 10; ++i ) {
    filter[i] = new FormSwep();
  }

  // Setup formant filters
  //                   freq, radius, gain 
  filter[0]->setStates(  225, 0.99, 1.0 ); // 225
  filter[1]->setStates(  450, 0.99, 1.0 ); // 450
  filter[2]->setStates(  700, 0.99, 1.0 ); // 700
  filter[3]->setStates( 1000, 0.99, 1.0 ); // 1000
  filter[4]->setStates( 1400, 0.99, 1.0 ); // 1400
  filter[5]->setStates( 2000, 0.99, 1.0 ); // 2000
  filter[6]->setStates( 2700, 0.99, 1.0 ); // 2700
  filter[7]->setStates( 3800, 0.99, 1.0 ); // 3800
  filter[8]->setStates( 5400, 0.99, 1.0 ); // 5400
  filter[9]->setStates( 7500, 0.99, 1.0 ); // 7500

  // Setup Plosives -- TODO: make each one (currently all P I think)
  std::vector<PlosiveData*> plos_tmp;
  for ( unsigned int p = 0; p < 6; ++p ) {
    PlosiveData * tmp = new PlosiveData();
    tmp->t_switch = 173;
    tmp->t_end = 243;
    // Stop Gap Phase -- none
    tmp->pre_keys.push_back(3); tmp->pre_keys.push_back(10);
    tmp->pre_keys.push_back(6); tmp->pre_keys.push_back(6);
    tmp->pre_keys.push_back(3); tmp->pre_keys.push_back(4);
    // Release Phase
    // key                      volume (out of 10)
    tmp->post_keys.push_back(3); tmp->post_keys.push_back(10);
    tmp->post_keys.push_back(6); tmp->post_keys.push_back(6);
    tmp->post_keys.push_back(3); tmp->post_keys.push_back(4);
    plos_tmp.push_back( tmp );
  }

  // Pass sound generators into container
  sg.hiss = hiss;
  sg.buzz = buzz;
  sg.curr_buttons = bs;
  sg.prev_buttons = bs2;
  sg.keys = filter;
  sg.plos = plos_tmp;
}

// Deallocate SoundGens struct
void clean_up( SoundGens & sg ) {
  for ( unsigned int i = 0; i < 10; ++i ) {
    delete sg.keys[i]; 
  }

  for ( unsigned int j = 0; j < sg.plos.size(); ++j ) {
    delete sg.plos[j];
  }
  
  delete[] sg.keys;
  delete[] sg.curr_buttons;
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
    float dp = DELTA_PITCH;
    float dv = DELTA_VOLUME;
    
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
	base.pitch = DEFAULT_FREQ;
    }
    
    // Left Hand
    base.curr_buttons[0] = trinary_button( win, GLFW_KEY_Z, GLFW_KEY_A, GLFW_KEY_Q );
    base.curr_buttons[1] = trinary_button( win, GLFW_KEY_X, GLFW_KEY_S, GLFW_KEY_W );
    base.curr_buttons[2] = trinary_button( win, GLFW_KEY_C, GLFW_KEY_D, GLFW_KEY_E );
    base.curr_buttons[3] = trinary_button( win, GLFW_KEY_V, GLFW_KEY_F, GLFW_KEY_R );
    base.curr_buttons[4] = trinary_button( win, GLFW_KEY_B, GLFW_KEY_G, GLFW_KEY_T );
    // Right Hand
    base.curr_buttons[5] = trinary_button( win, GLFW_KEY_N, GLFW_KEY_H, GLFW_KEY_Y );
    base.curr_buttons[6] = trinary_button( win, GLFW_KEY_M, GLFW_KEY_J, GLFW_KEY_U );
    base.curr_buttons[7] = trinary_button( win, GLFW_KEY_COMMA, GLFW_KEY_K, GLFW_KEY_I );
    base.curr_buttons[8] = trinary_button( win, GLFW_KEY_PERIOD, GLFW_KEY_L, GLFW_KEY_O );
    base.curr_buttons[9] = trinary_button( win, GLFW_KEY_SLASH, GLFW_KEY_SEMICOLON, GLFW_KEY_P );
    // Plosives
    base.curr_buttons[10] = (win->isKeyPressed( GLFW_KEY_1 ) ) ? 1 : 0;
    base.curr_buttons[11] = (win->isKeyPressed( GLFW_KEY_2 ) ) ? 1 : 0;
    base.curr_buttons[12] = (win->isKeyPressed( GLFW_KEY_3 ) ) ? 1 : 0;
    // Notes:
    // when released start timer and switch timer
    // timer counts down to end
    // switch timer is how long to switch from hiss to buzz
    for ( unsigned int i = 0; i<13; ++i ){
      if ( i >= 10 && base.prev_buttons[i] != 0 && base.curr_buttons[i] == 0 ) {
	if ( base.mode == 0 ) { // voiceless
	  base.active_plosive = i-10;
	  base.timer = 0;
	  std::cout << "Voiceless Plosive Activated: "<< base.active_plosive << std::endl;
	} else { // voiced
	  base.active_plosive = (i-10) + 3;
	  base.timer = 0;
	  std::cout << "Voiced Plosive Activated: "<< base.active_plosive << std::endl;
	}
      }
      base.prev_buttons[i] = base.curr_buttons[i];
    }
}

int trinary_button( gui::Window * win, int key1, int key2, int key3 ) {
    if ( win->isKeyPressed( key1 ) ) {
	return 3;
    } if ( win->isKeyPressed( key2 ) ){
	return 6;
    } if ( win->isKeyPressed( key3 ) ) {
	return 10;
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


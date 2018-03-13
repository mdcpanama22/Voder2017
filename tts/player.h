/*============================================
 *
 * 
 *
 * March 2018
 * Voder2017
 *
 *============================================*/

#ifndef __player_h__
#define __player_h__

#include "FormSwep.h"  // Sweepable Formant Filter
#include "Noise.h"     // White Noise
#include "BlitSaw.h"   // Sawtooth
#include "RtAudio.h"   // Realtime Audio
// Wavefile output ?

using namespace stk;

/* Plays in_file.voder file
 * If out_file is given, output results to out_file.wav
 * If no out_file is given, play audio to available devices
 */
void play( std::string in_file, std::string out_file );

// Callback function for realtime audio
int tick( void * outBuff, void * inBuff, unsigned int nBuffFrames,
	    double streamTime, RtAudioStreamStatus status, void * dataPtr );


#endif

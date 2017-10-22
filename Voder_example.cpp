/*=================================
 * Formant Filter Example
 *
 * Voder2017
 * Oct 2017
 *
 *=================================*/

#include "FormSwep.h"  // Sweepable Formant Filter
#include "Noise.h"     // White Noise
#include "BlitSaw.h"   // Sawtooth
#include "RtAudio.h"   // Real Time Audio

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
  
  for ( unsigned int i=0; i < nBuffFrames; ++i ) {
    if ( sg->mode == 0 ) { // silent
      *samples++ = 0;
    } else if ( sg->mode == 1 ) { // hiss
      *samples++ = sg->hiss->tick(); 
    } else if ( sg->mode == 2 ) { // buzz
      *samples++ = sg->buzz->tick(); 
    } else if ( sg->mode == 3 ) { // formant filter hiss
      *samples++ = sg->vox->tick( sg->hiss->tick() );
    } else if ( sg->mode == 4 ) { // formant filter buzz
      *samples++ = sg->vox->tick( sg->buzz->tick() );
    }
  }

  return 0;
}

int main( int argc, char ** argv ) {

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
  params.nChannels = 1;
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

  // Run thru program
  char keyhit;
  std::cout << "\nThis is the Voder example.\nPress Enter to continue...\n";
  std::cin.get( keyhit ); base.mode++;
  std::cout << "\nThis is the Hiss Noise Generator.\nPress Enter to continue...\n";
  std::cin.get( keyhit ); base.mode++;
  std::cout << "\nThis is the Buzz Noise Generator.\nPress Enter to continue...\n";
  std::cin.get( keyhit ); base.mode++;
  std::cout << "\nThis is a Formant Filter being applied to the Hiss.\nPress Enter to continue...\n";
  std::cin.get( keyhit ); base.mode++;
  std::cout << "\nThis is a Formant Filter being applied to the Hiss.\nPress Enter to exit...\n";
  std::cin.get( keyhit ); base.mode++;
  
  // Shutdown Stream
  try {
    dac.closeStream();
  } catch ( RtAudioError &error ) {
    error.printMessage();
  }

 cleanup:
  
  return 0;
}

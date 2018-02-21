/*=================================
 * Full Voder Replication v1
 *
 * Voder2017
 * Feb 2018
 *
 *=================================*/

#include "Voder.h"

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
    dac.openStream( &params, NULL, format, (unsigned int)Stk::sampleRate(),
										&bufferFrames, &tick, (void *)&base );
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

	// Display keys
	chart();
	
  // Core loop
  while (!win->windowShouldClose()) {
    win->clear();
    win->update();

    // Check which keys are being pressed
		read_keys( win, base );

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

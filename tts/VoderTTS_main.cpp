/*=========================================
 *    Voder Text-to-Speech
 * 
 * Voder2017
 * March 2018
 *
 *=========================================*/

#include <iostream>
#include <string>

void help();

int main( int argc, char ** argv ) {

  /* Arguments:
   * none
   * -help
   * -read text.txt [file.voder]
   * -play file.voder [out.wav]
   */
  
  if ( argc < 2 ) { // no arguments given
    std::cerr << "voderTTS: error: no commands given" << std::endl << std::endl;;
    help();
    return 0;
  }

  if ( std::string(argv[1]) == std::string("-help") ) {
    help();
  } else if ( std::string(argv[1]) == std::string("-read") && argc >= 3 ) {
    std::string in_file( argv[2] );
    std::string out_file = (argc >= 4) ? argv[3] : "";
    //read( in_file, out_file );
  } else if ( std::string(argv[1]) == std::string("-play") && argc >= 3 ) {
    std::string in_file( argv[2] );
    std::string out_file = (argc >= 4) ? argv[3] : "";
    //play( in_file, out_file );
  } else {
    std::cerr << "voderTTS: error: unrecognized command" << std::endl;
    help();
  }

  return 0;
}

void help() {
  std::cout << "voderTTS:" << std::endl;
  
  std::cout << "DESCRIPTION" << std::endl;
  std::cout << "\tvoderTTS is a text-to-speech program that uses the \n";
  std::cout << "\tVoder2017 program as it's speech engine." << std::endl << std::endl;

  std::cout << "USAGE" << std::endl;
  std::cout << "\tvoderTTS -help" << std::endl;
  std::cout << "\t\tOutputs usage message and exit" << std::endl << std::endl;

  std::cout << "\tvoderTTS -read FILE_IN [FILE_OUT]" << std::endl;
  std::cout << "\t\tConverts ASCII IPA text file (FILE_IN) into a Voder\n";
  std::cout << "\t\treadable file (FILE_OUT). If no filename is given a\n";
  std::cout << "\t\tfile is named using the FILE_IN file.";
  std::cout << std::endl << std::endl;

  std::cout << "\tvoderTTS -play FILE_IN [FILE_OUT]" << std::endl;
  std::cout << "\t\tPlays a Voder file (FILE_IN). If a FILE_OUT is given\n";
  std::cout << "\t\tit outputs to a .wav file. If none if given the audio\n";
  std::cout << "\t\tis played through the available audio device.";
  std::cout << std::endl << std::endl;

}

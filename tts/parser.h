/*============================================
 *
 * Contains IPA ASCII to Voder File Parser
 *
 * March 2018
 * Voder2017
 *
 *============================================*/

#ifndef __parser_h__
#define __parser_h__

#include <string>

/* Parses in_file and outputs results into out_file
 * If out_file is not correct filetype change it
 * If no out_file is given, make one from in_file name
 *   Ex: in_file(time).voder
 */
void read( std::string in_file, std::string out_file );

#endif

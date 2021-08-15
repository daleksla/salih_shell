#ifndef DISPLAY_H
#define DISPLAY_H
#pragma once

/** @brief Enums / Aliases relating to displaying colour, spacing, text-texture etc.
  * @author Salih Ahmed
  * @date 14 Aug 2021 **/

typedef enum {
	/** Numbers pertaining to resetting altered output to defaults. Intended for internal usage **/
	reset_full = 0,
	reset_bold = 21,
	reset_dim,
	reset_underline = 24,
	reset_blink,
	reset_invert = 27,
	reset_hidden,
	reset_foreground = 39,
	reset_background = 49
	
} reset_t ;

typedef enum texture_t {
	/** Numbers petaining to text-centric display changes **/
	texture_bold = 1,
	texture_dim,
	texture_underline = 4,
	texture_blink,
	texture_invert = 7,
	texture_hidden
	
} texture_t ;
 
typedef enum foreground_t {
	/** Numbers petaining to changing text's colour **/
	foreground_black = 30,
	foreground_red,
	foreground_green,
	foreground_yellow, 
	foreground_blue,
	foreground_magenta,
	foreground_cyan,
	foreground_light_grey,
	foreground_dark_grey = 90,
	foreground_light_red,
	foreground_light_green,
	foreground_light_yellow,
	foreground_light_blue,
	foreground_light_magenta,
	foreground_light_cyan,
	foreground_white
	
} foreground_t ;

typedef enum background_t {
	/** Numbers petaining to changing colour around text **/
	background_black = 40,
	background_red,
	background_green,
	background_yellow,
	background_blue,
	background_magenta,
	background_cyan,
	background_light_grey,
	background_dark_grey = 100,
	background_light_red,
	background_light_green,
	background_light_yellow,
	background_light_blue,
	background_light_magenta,
	background_light_cyan,
	background_white,
	
} background_t ;

/** set_display clean way to setup display options
  * @param const texture_t enum containing value of texture (e.g. bold, underlined, etc.)
  * @param const foreground_t enum containing value of foreground (ie what colour)
  * @param const background_t enum containing value of background **/
void set_display(const texture_t, const foreground_t, const background_t) ;

/** reset_display resets display to print normal text **/
void reset_display(void) ;
  
#endif // DISPLAY_H

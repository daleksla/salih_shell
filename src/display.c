#include <string.h> // strlen, strcat
#include <stdio.h> // vprintf
#include <stdarg.h> // variadic parameter support

#include "display.h"

/** @brief Functionality relating to displaying colour, spacing, text-texture etc.
  * @author Salih Ahmed
  * @date 14 Aug 2021 **/

void set_display(const texture_t i_texture, const foreground_t i_foreground, const background_t i_background)
{
	printf("\e[%d;%d;%dm", i_texture, i_foreground, i_background) ;
}

void reset_display(void)
{
	printf("\e[%d;m", reset_full) ;
}

#pragma once

#define DISPLAY_WIDTH   11
#define DISPLAY_HEIGHT  10

/**
 * Display buffer represented by X,Y coördinates
 */
extern unsigned int display_buffer[DISPLAY_WIDTH - 1][DISPLAY_HEIGHT - 1];

/**
 * Flush buffer to display (this actually updates the display)
 */
void flush();

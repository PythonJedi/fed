/*
 *      FED - Folding Editor
 *
 *      By Shawn Hargreaves, 1994
 *
 *      See README.TXT for copyright conditions
 *
 *      IO routines using the Allegro library.
 */


#ifndef TARGET_ALLEG
   #error This file should only be compiled as part of the Allegro target
#endif


#include <time.h>
#include <dirent.h>
#include <glob.h>

#include "fed.h"


int screen_w = 80;
int screen_h = 25; 

int x_pos = 0, y_pos = 0;
int _x_pos = 0, _y_pos = 0;

int term_inited = FALSE;

int attrib = 7;
int norm_attrib = 7;

int m_x = -1;
int m_y = -1;
int m_b = 0;

int mouse_height = 1;

int cursor = 0;
int cursor_x = 0;
int cursor_y = 0;
int cursor_flag = FALSE;
BITMAP *cursor_bmp = NULL;
BITMAP *cursor_bmp2 = NULL;



FONT_8x8 the_font_data =
{
   {
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00 },
      { 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00 },
      { 0x18, 0x7E, 0xC0, 0x7C, 0x06, 0xFC, 0x18, 0x00 },
      { 0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00 },
      { 0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0x00 },
      { 0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00 },
      { 0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00 },
      { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00 },
      { 0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30 },
      { 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00 },
      { 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00 },
      { 0x7C, 0xCE, 0xDE, 0xF6, 0xE6, 0xC6, 0x7C, 0x00 },
      { 0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00 },
      { 0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0x00 },
      { 0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x78, 0x00 },
      { 0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0x00 },
      { 0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00 },
      { 0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0x00 },
      { 0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00 },
      { 0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0x00 },
      { 0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0x00 },
      { 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00 },
      { 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30 },
      { 0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x00 },
      { 0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00 },
      { 0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00 },
      { 0x3C, 0x66, 0x0C, 0x18, 0x18, 0x00, 0x18, 0x00 },
      { 0x7C, 0xC6, 0xDE, 0xDE, 0xDC, 0xC0, 0x7C, 0x00 },
      { 0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x00 },
      { 0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00 },
      { 0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00 },
      { 0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00 },
      { 0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE, 0x00 },
      { 0xFE, 0x62, 0x68, 0x78, 0x68, 0x60, 0xF0, 0x00 },
      { 0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3A, 0x00 },
      { 0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00 },
      { 0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00 },
      { 0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00 },
      { 0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00 },
      { 0xF0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00 },
      { 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00 },
      { 0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00 },
      { 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00 },
      { 0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00 },
      { 0x7C, 0xC6, 0xC6, 0xC6, 0xD6, 0x7C, 0x0E, 0x00 },
      { 0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00 },
      { 0x7C, 0xC6, 0xE0, 0x78, 0x0E, 0xC6, 0x7C, 0x00 },
      { 0xFC, 0xB4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00 },
      { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x00 },
      { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00 },
      { 0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00 },
      { 0xC6, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0xC6, 0x00 },
      { 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00 },
      { 0xFE, 0xC6, 0x8C, 0x18, 0x32, 0x66, 0xFE, 0x00 },
      { 0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00 },
      { 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00 },
      { 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00 },
      { 0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF },
      { 0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00 },
      { 0xE0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xDC, 0x00 },
      { 0x00, 0x00, 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x00 },
      { 0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0x76, 0x00 },
      { 0x00, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00 },
      { 0x38, 0x6C, 0x64, 0xF0, 0x60, 0x60, 0xF0, 0x00 },
      { 0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8 },
      { 0xE0, 0x60, 0x6C, 0x76, 0x66, 0x66, 0xE6, 0x00 },
      { 0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00 },
      { 0x0C, 0x00, 0x1C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78 },
      { 0xE0, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0xE6, 0x00 },
      { 0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00 },
      { 0x00, 0x00, 0xCC, 0xFE, 0xFE, 0xD6, 0xD6, 0x00 },
      { 0x00, 0x00, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0x00 },
      { 0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0x78, 0x00 },
      { 0x00, 0x00, 0xDC, 0x66, 0x66, 0x7C, 0x60, 0xF0 },
      { 0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E },
      { 0x00, 0x00, 0xDC, 0x76, 0x62, 0x60, 0xF0, 0x00 },
      { 0x00, 0x00, 0x7C, 0xC0, 0x70, 0x1C, 0xF8, 0x00 },
      { 0x10, 0x30, 0xFC, 0x30, 0x30, 0x34, 0x18, 0x00 },
      { 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00 },
      { 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00 },
      { 0x00, 0x00, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00 },
      { 0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00 },
      { 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8 },
      { 0x00, 0x00, 0xFC, 0x98, 0x30, 0x64, 0xFC, 0x00 },
      { 0x1C, 0x30, 0x30, 0xE0, 0x30, 0x30, 0x1C, 0x00 },
      { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00 },
      { 0xE0, 0x30, 0x30, 0x1C, 0x30, 0x30, 0xE0, 0x00 },
      { 0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0x00 },
      { 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x18, 0x0C, 0x78 },
      { 0x00, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0x7E, 0x00 },
      { 0x1C, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00 },
      { 0x7E, 0xC3, 0x3C, 0x06, 0x3E, 0x66, 0x3F, 0x00 },
      { 0xCC, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x7E, 0x00 },
      { 0xE0, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x7E, 0x00 },
      { 0x30, 0x30, 0x78, 0x0C, 0x7C, 0xCC, 0x7E, 0x00 },
      { 0x00, 0x00, 0x78, 0xC0, 0xC0, 0x78, 0x0C, 0x38 },
      { 0x7E, 0xC3, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00 },
      { 0xCC, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00 },
      { 0xE0, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00 },
      { 0xCC, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00 },
      { 0x7C, 0xC6, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00 },
      { 0xE0, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00 },
      { 0xC6, 0x38, 0x6C, 0xC6, 0xFE, 0xC6, 0xC6, 0x00 },
      { 0x30, 0x30, 0x00, 0x78, 0xCC, 0xFC, 0xCC, 0x00 },
      { 0x1C, 0x00, 0xFC, 0x60, 0x78, 0x60, 0xFC, 0x00 },
      { 0x00, 0x00, 0x7F, 0x0C, 0x7F, 0xCC, 0x7F, 0x00 },
      { 0x3E, 0x6C, 0xCC, 0xFE, 0xCC, 0xCC, 0xCE, 0x00 },
      { 0x78, 0xCC, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00 },
      { 0x00, 0xCC, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00 },
      { 0x00, 0xE0, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00 },
      { 0x78, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0x7E, 0x00 },
      { 0x00, 0xE0, 0x00, 0xCC, 0xCC, 0xCC, 0x7E, 0x00 },
      { 0x00, 0xCC, 0x00, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8 },
      { 0xC3, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x00 },
      { 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x00 },
      { 0x18, 0x18, 0x7E, 0xC0, 0xC0, 0x7E, 0x18, 0x18 },
      { 0x38, 0x6C, 0x64, 0xF0, 0x60, 0xE6, 0xFC, 0x00 },
      { 0xCC, 0xCC, 0x78, 0xFC, 0x30, 0xFC, 0x30, 0x30 },
      { 0xF8, 0xCC, 0xCC, 0xFA, 0xC6, 0xCF, 0xC6, 0xC7 },
      { 0x0E, 0x1B, 0x18, 0x3C, 0x18, 0x18, 0xD8, 0x70 },
      { 0x1C, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x7E, 0x00 },
      { 0x38, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00 },
      { 0x00, 0x1C, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00 },
      { 0x00, 0x1C, 0x00, 0xCC, 0xCC, 0xCC, 0x7E, 0x00 },
      { 0x00, 0xF8, 0x00, 0xF8, 0xCC, 0xCC, 0xCC, 0x00 },
      { 0xFC, 0x00, 0xCC, 0xEC, 0xFC, 0xDC, 0xCC, 0x00 },
      { 0x3C, 0x6C, 0x6C, 0x3E, 0x00, 0x7E, 0x00, 0x00 },
      { 0x38, 0x6C, 0x6C, 0x38, 0x00, 0x7C, 0x00, 0x00 },
      { 0x30, 0x00, 0x30, 0x60, 0xC0, 0xCC, 0x78, 0x00 },
      { 0x00, 0x00, 0x00, 0xFC, 0xC0, 0xC0, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0xFC, 0x0C, 0x0C, 0x00, 0x00 },
      { 0xC3, 0xC6, 0xCC, 0xDE, 0x33, 0x66, 0xCC, 0x0F },
      { 0xC3, 0xC6, 0xCC, 0xDB, 0x37, 0x6F, 0xCF, 0x03 },
      { 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00 },
      { 0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00 },
      { 0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00 },
      { 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88 },
      { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
      { 0xDB, 0x77, 0xDB, 0xEE, 0xDB, 0x77, 0xDB, 0xEE },
      { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 },
      { 0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0x18, 0x18 },
      { 0x18, 0x18, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18 },
      { 0x36, 0x36, 0x36, 0x36, 0xF6, 0x36, 0x36, 0x36 },
      { 0x00, 0x00, 0x00, 0x00, 0xFE, 0x36, 0x36, 0x36 },
      { 0x00, 0x00, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18 },
      { 0x36, 0x36, 0xF6, 0x06, 0xF6, 0x36, 0x36, 0x36 },
      { 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36 },
      { 0x00, 0x00, 0xFE, 0x06, 0xF6, 0x36, 0x36, 0x36 },
      { 0x36, 0x36, 0xF6, 0x06, 0xFE, 0x00, 0x00, 0x00 },
      { 0x36, 0x36, 0x36, 0x36, 0xFE, 0x00, 0x00, 0x00 },
      { 0x18, 0x18, 0xF8, 0x18, 0xF8, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0x18, 0x18 },
      { 0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, 0x00, 0x00 },
      { 0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0xFF, 0x18, 0x18, 0x18 },
      { 0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x18, 0x18 },
      { 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00 },
      { 0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18 },
      { 0x18, 0x18, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18 },
      { 0x36, 0x36, 0x36, 0x36, 0x37, 0x36, 0x36, 0x36 },
      { 0x36, 0x36, 0x37, 0x30, 0x3F, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x3F, 0x30, 0x37, 0x36, 0x36, 0x36 },
      { 0x36, 0x36, 0xF7, 0x00, 0xFF, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0xFF, 0x00, 0xF7, 0x36, 0x36, 0x36 },
      { 0x36, 0x36, 0x37, 0x30, 0x37, 0x36, 0x36, 0x36 },
      { 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00 },
      { 0x36, 0x36, 0xF7, 0x00, 0xF7, 0x36, 0x36, 0x36 },
      { 0x18, 0x18, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00 },
      { 0x36, 0x36, 0x36, 0x36, 0xFF, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x18, 0x18, 0x18 },
      { 0x00, 0x00, 0x00, 0x00, 0xFF, 0x36, 0x36, 0x36 },
      { 0x36, 0x36, 0x36, 0x36, 0x3F, 0x00, 0x00, 0x00 },
      { 0x18, 0x18, 0x1F, 0x18, 0x1F, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18 },
      { 0x00, 0x00, 0x00, 0x00, 0x3F, 0x36, 0x36, 0x36 },
      { 0x36, 0x36, 0x36, 0x36, 0xFF, 0x36, 0x36, 0x36 },
      { 0x18, 0x18, 0xFF, 0x18, 0xFF, 0x18, 0x18, 0x18 },
      { 0x18, 0x18, 0x18, 0x18, 0xF8, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x18, 0x18 },
      { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
      { 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF },
      { 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0 },
      { 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F },
      { 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x76, 0xDC, 0xC8, 0xDC, 0x76, 0x00 },
      { 0x00, 0x78, 0xCC, 0xF8, 0xCC, 0xF8, 0xC0, 0xC0 },
      { 0x00, 0xFC, 0xCC, 0xC0, 0xC0, 0xC0, 0xC0, 0x00 },
      { 0x00, 0xFE, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x00 },
      { 0xFC, 0xCC, 0x60, 0x30, 0x60, 0xCC, 0xFC, 0x00 },
      { 0x00, 0x00, 0x7E, 0xD8, 0xD8, 0xD8, 0x70, 0x00 },
      { 0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xC0 },
      { 0x00, 0x76, 0xDC, 0x18, 0x18, 0x18, 0x18, 0x00 },
      { 0xFC, 0x30, 0x78, 0xCC, 0xCC, 0x78, 0x30, 0xFC },
      { 0x38, 0x6C, 0xC6, 0xFE, 0xC6, 0x6C, 0x38, 0x00 },
      { 0x38, 0x6C, 0xC6, 0xC6, 0x6C, 0x6C, 0xEE, 0x00 },
      { 0x1C, 0x30, 0x18, 0x7C, 0xCC, 0xCC, 0x78, 0x00 },
      { 0x00, 0x00, 0x7E, 0xDB, 0xDB, 0x7E, 0x00, 0x00 },
      { 0x06, 0x0C, 0x7E, 0xDB, 0xDB, 0x7E, 0x60, 0xC0 },
      { 0x38, 0x60, 0xC0, 0xF8, 0xC0, 0x60, 0x38, 0x00 },
      { 0x78, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00 },
      { 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x00 },
      { 0x30, 0x30, 0xFC, 0x30, 0x30, 0x00, 0xFC, 0x00 },
      { 0x60, 0x30, 0x18, 0x30, 0x60, 0x00, 0xFC, 0x00 },
      { 0x18, 0x30, 0x60, 0x30, 0x18, 0x00, 0xFC, 0x00 },
      { 0x0E, 0x1B, 0x1B, 0x18, 0x18, 0x18, 0x18, 0x18 },
      { 0x18, 0x18, 0x18, 0x18, 0x18, 0xD8, 0xD8, 0x70 },
      { 0x30, 0x30, 0x00, 0xFC, 0x00, 0x30, 0x30, 0x00 },
      { 0x00, 0x76, 0xDC, 0x00, 0x76, 0xDC, 0x00, 0x00 },
      { 0x38, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00 },
      { 0x0F, 0x0C, 0x0C, 0x0C, 0xEC, 0x6C, 0x3C, 0x1C },
      { 0x78, 0x6C, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00 },
      { 0x70, 0x18, 0x30, 0x60, 0x78, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
   }
};



FONT the_font =
{
   8,
   {
      &the_font_data
   }
};



PALETTE the_pal =
{
   { 0,  0,  0  },   /* black */
   { 0,  0,  48 },   /* blue */
   { 0,  48, 0  },   /* green */
   { 0,  48, 48 },   /* cyan */
   { 48, 0,  0  },   /* red */
   { 48, 0,  48 },   /* magenta */
   { 48, 32, 16 },   /* brown */
   { 48, 48, 48 },   /* light grey */
   { 24, 24, 24 },   /* dark grey */
   { 24, 24, 63 },   /* light blue */
   { 24, 63, 24,},   /* light green */
   { 24, 63, 63 },   /* light cyan */
   { 63, 24, 24 },   /* light red */
   { 63, 24, 63 },   /* light magenta */
   { 63, 63, 0  },   /* yellow */
   { 63, 63, 63 },   /* white */
};



int flash_timer = 0;



void flash_cursor()
{
   int i, j;

   j = (flash_timer) ? 0 : 128;

   for (i=0; i<16; i++)
      _set_color(i+128, &the_pal[i+j]);

   flash_timer = !flash_timer;
}

END_OF_FUNCTION(flash_cursor);



void term_init(int screenheight)
{
   int w, h, i;

   if (term_inited)
      term_exit();

   allegro_init();
   install_keyboard();
   install_mouse();
   install_timer();

   if (screenheight < 0) {
      clear_keybuf();
      readkey();

      screenheight = screen_h;
   }

   switch (screenheight) {
      case 25:  w = 320;  h = 200;  break; 
      case 50:  w = 640;  h = 400;  break; 
      case 60:  w = 640;  h = 480;  break; 
      case 75:  w = 800;  h = 600;  break; 
      case 96:  w = 1024; h = 768;  break; 
      case 128: w = 1280; h = 1024; break; 
      default:  w = 640;  h = 480;  break; 
   }

   if (set_gfx_mode(GFX_AUTODETECT, w, h, 0, 0) != 0)
      set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0);

   for (i=16; i<255; i++) {
      the_pal[i].r = 0;
      the_pal[i].g = 0;
      the_pal[i].b = 0;
   }

   the_pal[255].r = 63;
   the_pal[255].g = 63;
   the_pal[255].b = 63;

   for (i=0; i<16; i++) {
      the_pal[i+128].r = 63-the_pal[i].r;
      the_pal[i+128].g = 63-the_pal[i].g;
      the_pal[i+128].b = 63-the_pal[i].b;
   }

   set_palette(the_pal);

   screen_w = SCREEN_W/8;
   screen_h = SCREEN_H/8;

   m_x = mouse_x / 8;
   m_y = mouse_y / 8;

   cursor = 0;
   cursor_flag = FALSE;
   cursor_bmp = create_bitmap(8, 8);
   cursor_bmp2 = create_bitmap(8, 8);

   LOCK_FUNCTION(flash_cursor);
   LOCK_VARIABLE(the_pal);
   LOCK_VARIABLE(flash_timer);

   install_int_ex(flash_cursor, BPS_TO_TIMER(4));

   term_inited = TRUE;

   errno = 0;
}



void term_exit()
{
   destroy_bitmap(cursor_bmp);
   destroy_bitmap(cursor_bmp2);

   allegro_exit();

   term_inited = FALSE;
}



void term_suspend(int newline)
{
   term_exit();
}



void term_reinit(int wait) 
{
   term_init((wait) ? -1 : screen_h);
}



void scare_cursor(int x, int y, int w)
{
   if ((cursor_flag) &&
       ((w < 0) ||
	((cursor_x >= x) && (cursor_x < x+w) && (cursor_y == y)))) {
      blit(cursor_bmp, screen, 0, 0, cursor_x*8, cursor_y*8, 8, 8);
      cursor_flag = FALSE;
   }
}



void unscare_cursor()
{
   int x, y;

   if ((cursor) && (!cursor_flag)) {
      cursor_x = _x_pos;
      cursor_y = _y_pos;

      blit(screen, cursor_bmp, cursor_x*8, cursor_y*8, 0, 0, 8, 8);
      blit(cursor_bmp, cursor_bmp2, 0, 0, 0, 0, 8, 8);

      for (y = ((cursor==1) ? 5 : 0); y<8; y++)
	 for (x=0; x<8; x++)
	    cursor_bmp2->line[y][x] |= 128;

      blit(cursor_bmp2, screen, 0, 0, cursor_x*8, cursor_y*8, 8, 8);

      flash_timer = 0;
      flash_cursor();

      cursor_flag = TRUE;
   }
}



int scancode_map[][2] =
{
   { 15107, 15104 },   /* F1 */
   { 15363, 15360 },   /* F2 */
   { 15619, 15616 },   /* F3 */
   { 15875, 15872 },   /* F4 */
   { 16136, 16128 },   /* F5 */
   { 16387, 16384 },   /* F6 */
   { 16643, 16640 },   /* F7 */
   { 16899, 16896 },   /* F8 */
   { 17155, 17152 },   /* F9 */
   { 17411, 17408 },   /* F10 */
   { 15106, 24064 },   /* ctrl+F1 */
   { 15362, 24320 },   /* ctrl+F2 */
   { 15618, 24576 },   /* ctrl+F3 */
   { 15874, 24832 },   /* ctrl+F4 */
   { 16130, 25088 },   /* ctrl+F5 */
   { 16386, 25344 },   /* ctrl+F6 */
   { 16642, 25600 },   /* ctrl+F7 */
   { 16898, 25856 },   /* ctrl+F8 */
   { 17154, 26112 },   /* ctrl+F9 */
   { 17410, 26368 },   /* ctrl+F10 */
   { 6658, 6683   },   /* ctrl+[ */
   { 6914, 6941   },   /* ctrl+] */
   { 18177, 18176 },   /* shift+home */
   { 20225, 20224 },   /* shift+end */
   { 18689, 18688 },   /* shift+pgup */
   { 20737, 20736 },   /* shift+pgdn */
   { 18433, 18432 },   /* shift+up */
   { 20481, 20480 },   /* shift+down */
   { 19201, 19200 },   /* shift+left */
   { 19713, 19712 },   /* shift+right */
   { 19202, 29440 },   /* ctrl+left */
   { 19714, 29696 },   /* ctrl+right */
   { 19203, 29440 },   /* ctrl+shift+left */
   { 19715, 29696 },   /* ctrl+shift+right */
   { 18178, 30464 },   /* ctrl+home */
   { 20226, 29952 },   /* ctrl+end */
   { 18179, 30464 },   /* ctrl+shift+home */
   { 20227, 29952 },   /* ctrl+shift+end */
};



int gch()
{
   int c, i;

   c = readkey();

   for (i=0; i<sizeof(scancode_map)/sizeof(scancode_map[0]); i++) {
      if (c == scancode_map[i][0]) {
	 c = scancode_map[i][1];
	 break;
      }
   }

   return c;
}



int modifiers()
{
   int mod = 0;

   if (key_shifts & KB_SHIFT_FLAG)
      mod |= KF_SHIFT;

   if (key_shifts & KB_CTRL_FLAG)
      mod |= KF_CTRL;

   if (key_shifts & KB_ALT_FLAG)
      mod |= KF_ALT;

   return mod;
}



void pch(unsigned char c)
{
   char s[2] = { c, 0 };

   if ((x_pos >= 0) && (x_pos < screen_w)) {
      scare_cursor(x_pos, y_pos, 1);
      text_mode(attrib>>4);
      textout(screen, &the_font, s, x_pos*8, y_pos*8, attrib&15);
      unscare_cursor();
   }

   x_pos++;
}



void mywrite(unsigned char *s)
{
   scare_cursor(x_pos, y_pos, strlen(s));
   text_mode(attrib>>4);
   textout(screen, &the_font, s, x_pos*8, y_pos*8, attrib&15);
   unscare_cursor();

   x_pos += strlen(s);
}



void del_to_eol()
{
   if (x_pos < screen_w) {
      scare_cursor(x_pos, y_pos, screen_w-x_pos);
      rectfill(screen, x_pos*8, y_pos*8, screen_w*8-1, y_pos*8+7, attrib>>4);
      unscare_cursor();
   }
}



void cr_scroll()
{
   scare_cursor(0, 0, -1);

   if (y_pos < screen_h-1) {
      y_pos++;
   }
   else {
      blit(screen, screen, 0, 8, 0, 0, SCREEN_W, SCREEN_H-8);
      rectfill(screen, 0, SCREEN_H-8, SCREEN_W-1, SCREEN_H-1, norm_attrib>>4);
   }

   x_pos = 0;

   unscare_cursor();
}



void cls()
{
   scare_cursor(0, 0, -1);

   clear(screen);
   goto1(0, 0);

   unscare_cursor();
}



void home()
{
   x_pos = 0;
   y_pos = 0;
}



void newline()
{
   if (y_pos < screen_h-1) 
      y_pos++;

   x_pos = 0;
}



void goto1(int x, int y)
{
   x_pos = x;
   y_pos = y;
}



void goto2(int x, int y)
{
   if ((_x_pos != x) || (_y_pos != y))
      scare_cursor(0, 0, -1);

   x_pos = _x_pos = x;
   y_pos = _y_pos = y;

   unscare_cursor();
}



void backspace()
{
   if (x_pos > 0) { 
      x_pos--; 
      pch(' '); 
      x_pos--; 
   } 
}



void linefeed()
{
   if (y_pos < screen_h-1) 
      y_pos++;
}



void cr()
{
   x_pos = 0;
}



void tattr(int col)
{
   attrib = col;
}



void hide_c()
{
   scare_cursor(0, 0, -1);
   cursor = 0;
   unscare_cursor();
}



void show_c()
{
   scare_cursor(0, 0, -1);
   cursor = (config.big_cursor) ? 2 : 1;
   unscare_cursor();
}



void show_c2(int ovr)
{
   scare_cursor(0, 0, -1);
   cursor = ((ovr) ? !config.big_cursor : config.big_cursor) ? 2 : 1;
   unscare_cursor();
}



void print(int c)
{
}



int printer_ready()
{
   return FALSE;
}



char *save_screen(int x, int y, int w, int h)
{
   BITMAP *bmp;

   w++;
   h++;

   bmp = create_bitmap(w*8, h*8);

   if (!bmp)
      return NULL;

   scare_cursor(0, 0, -1);
   blit(screen, bmp, x*8, y*8, 0, 0, w*8, h*8);
   unscare_cursor();

   return (char *)bmp;
}



void restore_screen(int x, int y, int w, int h, char *buf)
{
   BITMAP *bmp = (BITMAP *)buf;

   if (!bmp) {
      dirty_everything();
      return;
   }

   w++;
   h++;

   scare_cursor(0, 0, -1);
   blit(bmp, screen, 0, 0, x*8, y*8, w*8, h*8);
   unscare_cursor();

   destroy_bitmap(bmp);
}



MYFILE *open_file(char *name, int mode)
{
   FILE *f;

   if (mode == FMODE_READ)
      f = fopen(name, "rb");
   else if (mode == FMODE_WRITE)
      f = fopen(name, "wb");
   else
      f = NULL;

   if (!f) {
      if (!errno)
	 errno = ENMFILE;
   }
   else
      errno = 0;

   return f;
}



int close_file(MYFILE *f)
{
   if (f) {
      fclose(f);
      return errno;
   }

   return 0;
}



int get_char(MYFILE *f)
{
   int c = fgetc(f);

   if (c == EOF)
      errno = EOF;

   return c;
}



int peek_char(MYFILE *f)
{
   int c;

   c = fgetc(f);
   ungetc(c, f);

   if (c == EOF)
      errno = EOF;

   return c;
}



void put_char(MYFILE *f, int c)
{
   fputc(c, f);
}



int search_path(char *result, char *prog, char *exts, char *var)
{
   /* search for a file on the disk, searching PATH if needed */

   char *p;
   int pos;
   strcpy(result,prog);

   if (find_program(result,exts)) {       /* look in current directory */
      return TRUE;
   }

   p=getenv(var);
   if (p!=NULL) {
      while(*p) {
	 while ((*p==' ') || (*p==',') || (*p==';'))
	    p++;
	 if(*p) {
	    pos=0;
	    result[0]=0;
	    while ((*p) && (*p!=' ') && (*p!=',') && (*p!=';')) {
	       result[pos++]=*(p++);
	       result[pos]=0;
	    }
	    append_backslash(result);
	    strcat(result,prog);
	    if (find_program(result,exts))
	       return TRUE;
	 }
      }
   }

   return FALSE;
}



int find_program(char *name, char *ext)
{
   /* look for a file, checking the possible extensions */

   char *p;
   int pos;

   if ((ext) && (*ext) && (*(find_extension(name))==0)) {
      p = ext;
      while(*p) {
	 while ((*p==' ') || (*p==',') || (*p==';'))
	    p++;
	 if(*p) {
	    pos=strlen(name);
	    name[pos++]='.';
	    name[pos]=0;
	    while ((*p) && (*p!=' ') && (*p!=',') && (*p!=';')) {
	       name[pos++]=*(p++);
	       name[pos]=0;
	    }
	    if (file_exists(name))
	       return TRUE;
	    remove_extension(name);
	 }
      }
   }
   else
      return file_exists(name);

   return FALSE;
}



int do_for_each_file(char *name, int (*call_back)(char *, int), int param)
{
   char buf[256];
   glob_t g;
   int i, ret;
   int matched = FALSE;

   ret = glob(name, GLOB_MARK | GLOB_NOESCAPE, NULL, &g);

   if (ret == 0) {
      errno = 0;

      for (i=0; i<g.gl_pathc; i++) {
	 strcpy(buf, g.gl_pathv[i]);

	 if ((buf[0]) && (buf[strlen(buf)-1] != '/')) {
	    matched = TRUE;

	    (*call_back)(buf, param);

	    if (errno)
	       break;
	 }
      }

      globfree(&g);
   }

   if ((ret == GLOB_NOMATCH) || (!matched)) {
      for (i=0; name[i]; i++) {
	 if ((name[i]=='*') || (name[i]=='?') || (name[i]=='[')) {
	    errno = ENMFILE;
	    goto dont_load;
	 }
      }

      errno = 0;
      (*call_back)(name, param);

      dont_load:
      return errno;
   }

   errno = 0;
   return 0;
}



int do_for_each_directory(char *name, int (*call_back)(char *, int), int param)
{
   char buf[256];
   DIR *dir;
   glob_t g;
   int i;

   strcpy(buf, name);
   buf[strlen(buf)-1] = 0;

   dir = opendir(buf);

   if (!dir) {
      errno = ENMFILE;
      return errno;
   }

   closedir(dir);

   strcat(buf, "../");
   (*call_back)(buf, param);

   if (glob(name, GLOB_MARK | GLOB_NOESCAPE, NULL, &g) == 0) {
      errno = 0;

      if (!errno) {
	 for (i=0; i<g.gl_pathc; i++) {
	    strcpy(buf, g.gl_pathv[i]);

	    if ((buf[0]) && (buf[strlen(buf)-1] == '/')) {
	       (*call_back)(buf, param);

	       if (errno)
		  break;
	    }
	 }
      }

      globfree(&g);
   }

   errno = 0;
   return 0;
}



void windows_init()
{
}



int set_window_title(char *title)
{
   return FALSE;
}



int got_clipboard()
{
   return FALSE;
}



int got_clipboard_data()
{
   return FALSE;
}



int set_clipboard_data(char *data, int size)
{
   return FALSE;
}



char *get_clipboard_data(int *size)
{
   return FALSE;
}



void mouse_init()
{
}



void display_mouse()
{
   show_mouse(screen);
}



void hide_mouse()
{
   show_mouse(NULL);
}



int mouse_changed(int *x, int *y)
{
   if (x)
      *x = mouse_x / 8;

   if (y)
      *y = mouse_y * mouse_height / 8;

   return (((mouse_x / 8) != m_x) || 
	   ((mouse_y * mouse_height / 8) != m_y) || 
	   (mouse_b != m_b) || (mouse_b));
}



int poll_mouse()
{
   static int _x = -1, _y = -1;
   int ret;
   int x, y;

   m_b = mouse_b;
   x = mouse_x / 8;
   y = mouse_y * mouse_height / 8;

   ret = ((x != _x) || (y != _y) || (m_b));

   if (recording_macro()) {
      if (ret) {
	 strcpy(message, "Can't record mouse actions in a macro");
	 display_message(0);
      }
      m_b = 0;
      _x = x;
      _y = y;
      return FALSE;
   }

   m_x = _x = x;
   m_y = _y = y;
   return ret;
}



void set_mouse_pos(int x, int y)
{
   position_mouse(x*8, y*8/mouse_height);

   m_x = x;
   m_y = y;
}



void set_mouse_height(int h)
{
   m_y = m_y*h/mouse_height;

   mouse_height = h;
}



int mouse_dclick(int mode)
{
   int ox, oy;
   int start = clock();

   do {
      ox = m_x;
      oy = m_y;

      poll_mouse();

      if (!mode)
	 if ((ox != m_x) || (oy != m_y))
	    return TRUE;

      if (m_b & 1) {
	 if (mode) 
	    return TRUE;
      }
      else
	 if (!mode)
	    return FALSE;

   } while ((clock() - start) < CLOCKS_PER_SEC / 4);

   return !mode;
}



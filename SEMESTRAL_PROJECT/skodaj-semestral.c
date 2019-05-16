/*******************************************************************
  Simple program to check LCD functionality on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  mzapo_lcdtest.c       - main and only file

  (C) Copyright 2004 - 2017 by Pavel Pisa
	  e-mail:   pisa@cmp.felk.cvut.cz
	  homepage: http://cmp.felk.cvut.cz/~pisa
	  work:     http://www.pikron.com/
	  license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"

#include "utils.h"
#include "shapes.h"

#define WIDTH 480
#define HEIGHT 320

#define GREEN 0x7e0
#define WHITE 0xffff
#define BLUE 0x1f
#define RED 0xf800
#define BLACK 0x0



#include<time.h>
void delay(unsigned int mseconds)
{
	clock_t goal = mseconds + clock();
	while (goal > clock());
}



int main(int argc, char *argv[])
{
	unsigned char *parlcd_mem_base;
	parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	parlcd_hx8357_init(parlcd_mem_base);

	unsigned char *knobs_mem_base;
	knobs_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);


	display_t LCD;
	LCD.width = 480;
	LCD.height = 320;
	LCD.data = (uint16_t**)malloc(LCD.height * sizeof(uint16_t*));
	for (int i = 0; i < LCD.height; i++)
	{
		LCD.data[i] = (uint16_t*)malloc(LCD.width * sizeof(uint16_t));
	}
	for (int r = 0; r < LCD.height; r++)
	{
		for (int c = 0; c < LCD.width; c++)
		{
			LCD.data[r][c] = getColourFromRGB(0, 240, 0);
		}
	}

	while (1)
	{
		char* time = getTime();
		char* date = getShortDate();
		writeString(time, 5, (LCD.width - 75), getColourFromRGB(0, 240, 0), getColourFromRGB(0, 0, 0), &font_winFreeSystem14x16, 2, &LCD);
		writeString(date, 5, 5, getColourFromRGB(0, 240, 0), getColourFromRGB(0, 0, 0), &font_winFreeSystem14x16, 2, &LCD);
		showDisplay(&LCD, parlcd_mem_base);
		uint32_t value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);
		uint32_t blue_val = GET_BLUE(value);
		uint32_t red_val = GET_RED(value);
		uint32_t green_val = GET_GREEN(value);
		char s_value[10];
		writeString("          ", 50, 50, getColourFromRGB(0, 240, 0), getColourFromRGB(50, 50, 255), &font_wArial_44, 2, &LCD);
		writeString("          ", 100, 50, getColourFromRGB(0, 240, 0), getColourFromRGB(50, 50, 255), &font_wArial_44, 2, &LCD);
		writeString("          ", 150, 50, getColourFromRGB(0, 240, 0), getColourFromRGB(50, 50, 255), &font_wArial_44, 2, &LCD);
		sprintf(s_value, "%d", blue_val);
		writeString(s_value, 50, 50, getColourFromRGB(0, 240, 0), getColourFromRGB(50, 50, 255), &font_wArial_44, 2, &LCD);
		sprintf(s_value, "%d", red_val);
		writeString(s_value, 100, 50, getColourFromRGB(0, 240, 0), getColourFromRGB(255, 50, 50), &font_wArial_44, 2, &LCD);
		sprintf(s_value, "%d", green_val);
		writeString(s_value, 150, 50, getColourFromRGB(0, 240, 0), getColourFromRGB(50, 255, 50), &font_wArial_44, 2, &LCD);
		int diode_radius = 50;
		drawCircle(LCD.width - diode_radius - 5, (LCD.height - diode_radius) / 2, diode_radius, getColourFromRGB(255, 255, 0), &LCD);
		drawCircle_line(LCD.width - diode_radius - 5, (LCD.height - diode_radius) / 2, diode_radius, 10, getColourFromRGB(0, 0, 0), &LCD);
		drawGradient(20, 20, 100, 20, getColourFromRGB(255, 0, 0), getColourFromRGB(0, 255, 0), &LCD);

		free(time);
		free(date);
	}


	for (int i = 0; i < LCD.height; i++)
	{
		free(LCD.data[i]);
	}
	free(LCD.data);

	return 0;
}


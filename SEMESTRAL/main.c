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
#include <pthread.h>

#include "varlist.h"

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"

#define RGB 2
#define MAIN_MENU 1
#define SETTINGS 3
#define TEXT_SETTINGS 31
#define BRIGHT_SETTINGS 32
#define REMOTE_SETTINGS 33
#define BLINKING 4
int ignored = 0;
int displayed = MAIN_MENU;
int remote_allowed = 0;
uint16_t default_foreground = 0;
int brightness = 240;
uint16_t default_background = 0;
font_descriptor_t default_font;
int selected_menu = 0;
uint16_t RGB1_colour = 0;
uint16_t RGB2_colour = 0;
int RGB1_blink = 0;
int RGB2_blink = 0;
int RGB1_inBlink = 0;
int RGB2_inBlink = 0;
int RGB1_blink_counter = 0;
int RGB2_blink_counter = 0;
//#include "wArial_44.c"


#include "utils.h"
#include "shapes.h"

#include "screens.h"

#include "effects.h"
#include "threads.h"

#define WIDTH 480
#define HEIGHT 320

#define GREEN 0x7e0
#define WHITE 0xffff
#define BLUE 0x1f
#define RED 0xf800
#define BLACK 0x0

#define BACKGROUND getColourFromRGB(0, 240, 0)
#define FOREGROUND getColourFromRGB(0, 0, 0)
//#define BACKGROUND getColourFromRGB(0, 0, 0)
//#define FOREGROUND getColourFromRGB(255, 255, 255)


#include<time.h>

void displayMenu(display_t* display);

struct timespec startRGB1, startRGB2, end;
int start_set = 0;

int main(int argc, char *argv[])
{

	clock_gettime(CLOCK_MONOTONIC_RAW, &startRGB1);
	clock_gettime(CLOCK_MONOTONIC_RAW, &startRGB2);
	start_set = 3;


	unsigned char *parlcd_mem_base;
	parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	parlcd_hx8357_init(parlcd_mem_base);

	displayColorRGB1(getColourFromRGB(0, 0, 0));
	
	default_background = BACKGROUND;
	default_foreground = FOREGROUND;

	default_font = font_wArial_44;

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
			LCD.data[r][c] = default_background;
		}
	}
	unsigned char *knobs_mem_base;
	knobs_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);


	while (1)
	{
		char* time = getTime();
		char* date = getShortDate();
		writeString(time, 5, (LCD.width - 75), default_background, default_foreground, &font_winFreeSystem14x16, 2, &LCD);
		writeString(date, 5, 5, default_background, default_foreground, &font_winFreeSystem14x16, 2, &LCD);

		if (remote_allowed == 1)
		{
			writeString("REMOTE", 5, (LCD.width / 2) - 75, default_background, default_foreground, &font_winFreeSystem14x16, 2, &LCD);
			writeString(RC_STATUS, 5, (LCD.width / 2) + 10, default_background, default_foreground, &font_winFreeSystem14x16, 2, &LCD);
		}
		
		/*if (displayed = MAIN_MENU)
		{
			
			
			if ((value >> 26) == 1 && displayed == MAIN_MENU)
			{
				displayed = RGB;
				RGB_initiated = 0;
				ignored = 1;
				drawRGBScreen(&LCD, BACKGROUND, FOREGROUND, default_font);
			}
			else
			{
				displayMenu(&LCD);
			}
			
		}
		else if (displayed = RGB)
		{
			if (drawRGBScreen(&LCD, BACKGROUND, FOREGROUND, default_font) == 0)
			{
				displayed = MAIN_MENU;
			}
		}*/
	
		uint32_t value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);
		uint32_t menu_val = GET_RED(value);
		selected_menu = ((int)ceil((menu_val / 9)) % 3);
		if ((value >> 26) == 1 && displayed == MAIN_MENU && selected_menu == 0)
		{
			RGB_initiated = 0;
			displayed = RGB;
			for (int r = 0; r < LCD.height; r++)
			{
				for (int c = 0; c < LCD.width; c++)
				{
					LCD.data[r][c] = default_background;
				}
			}
			delay(SYS_INPUT_DELAY);
		}
		else if (((value >> 26) == 1 && displayed == MAIN_MENU && selected_menu == 1))
		{
			for (int r = 0; r < LCD.height; r++)
			{
				for (int c = 0; c < LCD.width; c++)
				{
					LCD.data[r][c] = default_background;
				}
			}
			displayed = BLINKING;
			delay(SYS_INPUT_DELAY);
		}
		else if (((value >> 26) == 1 && displayed == MAIN_MENU && selected_menu == 2))
		{
			displayed = SETTINGS;
			delay(SYS_INPUT_DELAY);
			settings_initiaited = 0;
		}
		if (displayed == MAIN_MENU)
		{
			displayMenu(&LCD);
		}
		else if (displayed == RGB)
		{
			if (remote_allowed == 1)
			{
				drawRGBScreenRemote(&LCD, default_background, default_foreground, default_font);
			}
			else
			{
				drawRGBScreen(&LCD, default_background, default_foreground, default_font);
			}
			
		}
		else if (displayed == SETTINGS)
		{
			drawSettings(&LCD, default_background, default_foreground, default_font);
			delay(SYS_INPUT_DELAY);
		}
		else if (displayed == TEXT_SETTINGS)
		{
			drawTextSettings(&LCD, default_background, default_foreground, default_font);
		}
		else if (displayed == BRIGHT_SETTINGS)
		{
			drawBrightSettings(&LCD, default_background, default_foreground, default_font);
		}
		else if (displayed == REMOTE_SETTINGS)
		{
			drawRemoteSettings(&LCD, default_background, default_foreground, default_font);
		}
		else if (displayed == BLINKING)
		{
			if (remote_allowed == 1)
			{
				drawBlinkingRemote(&LCD, default_background, default_foreground, default_font);
			}
			else
			{
				drawBlinking(&LCD, default_background, default_foreground, default_font);
			}

		}
		if (remote_allowed == 0)
		{
			if (start_set >= 2 && RGB1_blink != 0)
			{

				clock_gettime(CLOCK_MONOTONIC_RAW, &end);
				uint64_t delta_ms = (((end.tv_sec - startRGB1.tv_sec) * 1000 + (end.tv_nsec - startRGB1.tv_nsec) / 1000) * 1000);
				RGB1_blink_counter += delta_ms;
				if (RGB1_blink_counter < 0)
				{
					RGB1_blink_counter = 0;
				}

				if (RGB1_blink_counter >= RGB1_blink)
				{

					if (RGB1_inBlink == 1)
					{
						displayColorRGB1(RGB1_colour);
						RGB1_inBlink = 0;
						RGB1_blink_counter = 0;
					}
					else if (RGB1_inBlink == 0)
					{
						displayColorRGB1(getColourFromRGB(0, 0, 0));
						RGB1_inBlink = 1;
						RGB1_blink_counter = 0;
					}

					clock_gettime(CLOCK_MONOTONIC_RAW, &startRGB1);
				}
				else
				{
					if (RGB1_inBlink == 1)
					{
						displayColorRGB1(RGB1_colour);
					}
					else if (RGB1_inBlink == 0)
					{
						displayColorRGB1(getColourFromRGB(0, 0, 0));
					}
				}
			}
			else
			{
				displayColorRGB1(RGB1_colour);
			}
			if (start_set >= 2 && RGB2_blink != 0)
			{

				clock_gettime(CLOCK_MONOTONIC_RAW, &end);
				uint64_t delta_ms = (((end.tv_sec - startRGB2.tv_sec) * 1000 + (end.tv_nsec - startRGB2.tv_nsec) / 1000) * 1000);
				RGB2_blink_counter += delta_ms;
				if (RGB2_blink_counter < 0)
				{
					RGB2_blink_counter = 0;
				}

				if (RGB2_blink_counter >= RGB2_blink)
				{

					if (RGB2_inBlink == 1)
					{
						displayColorRGB2(RGB2_colour);
						RGB2_inBlink = 0;
						RGB2_blink_counter = 0;
					}
					else if (RGB2_inBlink == 0)
					{
						displayColorRGB2(getColourFromRGB(0, 0, 0));
						RGB2_inBlink = 1;
						RGB2_blink_counter = 0;
					}

					clock_gettime(CLOCK_MONOTONIC_RAW, &startRGB2);
				}
				else
				{
					if (RGB2_inBlink == 1)
					{
						displayColorRGB2(RGB2_colour);
					}
					else if (RGB2_inBlink == 0)
					{
						displayColorRGB2(getColourFromRGB(0, 0, 0));
					}
				}
			}
			else
			{
				displayColorRGB2(RGB2_colour);
			}
		}
		else if (remote_allowed == 1)
		{
			if (RC_RGB1_THREAD_RUN == RC_FALSE)
			{				
				pthread_create(&RC_RGB1_THREAD, NULL, RGB1_display, NULL);
				RC_RGB1_THREAD_RUN = RC_TRUE;
			}
			if (RC_RGB2_THREAD_RUN == RC_FALSE)
			{
				
				pthread_create(&RC_RGB2_THREAD, NULL, RGB2_display, NULL);
				RC_RGB2_THREAD_RUN = RC_TRUE;
			}
			
		}

		if (remote_allowed == 0 && RC_RGB1_THREAD_RUN == RC_TRUE)
		{
			pthread_cancel(RC_RGB1_THREAD);
			RC_RGB1_THREAD_RUN = RC_FALSE;
			RC_RGB1_BLICK_MODE = RC_BLICK_MODE_NONE;
		}

		if (remote_allowed == 0 && RC_RGB2_THREAD_RUN == RC_TRUE)
		{
			pthread_cancel(RC_RGB2_THREAD);
			RC_RGB2_THREAD_RUN = RC_FALSE;
			RC_RGB2_BLICK_MODE = RC_BLICK_MODE_NONE;
		}
		showDisplay(&LCD, parlcd_mem_base);
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

void displayMenu(display_t *display)
{
	if (selected_menu == 0)
	{
		drawRectangle(10, 100, display->width - 20, 45, default_foreground, display);
		drawRectangle(10, 200, display->width - 20, 45, default_background, display);
		drawRectangle(10, 150, display->width - 20, 45, default_background, display);
	}
	else if (selected_menu == 1)
	{
		drawRectangle(10, 150, display->width - 20, 45, default_foreground, display);
		drawRectangle(10, 100, display->width - 20, 45, default_background, display);
		drawRectangle(10, 200, display->width - 20, 45, default_background, display);
	}
	else if (selected_menu == 2)
	{
		drawRectangle(10, 200, display->width - 20, 45, default_foreground, display);
		drawRectangle(10, 150, display->width - 20, 45, default_background, display);
		drawRectangle(10, 100, display->width - 20, 45, default_background, display);
	}

	writeString("Main menu", 50, 10, default_background, default_foreground, &default_font, 2, display);
	writeString("RGB LED - Colours", 100, 10, (selected_menu == 0 ? default_foreground : default_background), (selected_menu == 0 ? default_background : default_foreground), &default_font,2, display);
	writeString("RGB LED - Blinking", 150, 10, (selected_menu == 1 ? default_foreground : default_background), (selected_menu == 1 ? default_background : default_foreground), &default_font,2, display);
	writeString("Settings", 200, 10, (selected_menu == 2 ? default_foreground : default_background), (selected_menu == 2 ? default_background : default_foreground),&default_font, 2, display);
	drawRectangle(0, display->height - 40, display->width, 2, default_foreground, display);

	drawCircle(10, display->height - 20, 7, getColourFromRGB(255, 0, 0), display);
	drawCircle_line(10, display->height - 20, 7, 2, default_foreground, display);
	writeString("Select", display->height - 25, 20, default_background, default_foreground, &font_rom8x16, 1, display);


}

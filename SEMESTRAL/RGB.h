#pragma once
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

//#include "wArial_44.c"

#include "utils.h"
#include "shapes.h"
int selected_led = 1;
uint16_t RGB1, RGB2 = 0;
int RGB_initiated = 0;
int drawRGBScreen(display_t *display, uint16_t background, uint16_t foreground, font_descriptor_t font)
{
	if (RGB_initiated == 0)
	{
		displayColorRGB1(getColourFromRGB(255, 255, 255));
	}
	int reti = 1;

	unsigned char *knobs_mem_base;
	knobs_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	uint32_t value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);
	uint32_t val_val = GET_BLUE(value);
	uint32_t hue_val = GET_RED(value);
	uint32_t sat_val = GET_GREEN(value);

	//drawCircle(display->width - diode_radius - 5, (display->height - diode_radius) / 2, diode_radius, getColourFromRGB(scaleRed(red_val), scaleGreen(green_val), scaleBlue(blue_val)), display);
	//drawCircle_line(display->width - diode_radius - 5, (display->height - diode_radius) / 2, diode_radius, 3, getColourFromRGB(0, 0, 0), display);

	drawGradient(100, 50, 50, 35, getColourFromRGB(255, 0, 0), getColourFromRGB(255, 255, 0), display);
	drawGradient(150, 50, 50, 35, getColourFromRGB(255, 255, 0), getColourFromRGB(0, 255, 0), display);
	drawGradient(200, 50, 50, 35, getColourFromRGB(0, 255, 0), getColourFromRGB(0, 255, 255), display);
	drawGradient(250, 50, 50, 35, getColourFromRGB(0, 255, 255), getColourFromRGB(0, 0, 255), display);
	drawGradient(300, 50, 50, 35, getColourFromRGB(0, 0, 255), getColourFromRGB(255, 0, 255), display);
	drawGradient(350, 50, 50, 35, getColourFromRGB(255, 0, 255), getColourFromRGB(255, 0, 0), display);

	drawRectangle((100 + scaleValue(hue_val, 0, 0b11111111, 0, 300)), 50, 2, 35, foreground, display);
	drawGradient(100, 100, 300, 35, getColourFromHue(hue_val), getColourFromRGB(255, 255, 255), display);
	drawRectangle((100 + scaleValue(sat_val, 0, 0b11111111, 0, 300)), 100, 2, 35, foreground, display);
	drawGradient(100, 150, 300, 35, getColourFromSaturation(getColourFromHue(hue_val), getColourFromRGB(255, 255, 255), (double)((double)sat_val / (double)256)), getColourFromRGB(0, 0, 0), display);
	drawRectangle((100 + scaleValue(val_val, 0, 0b11111111, 0, 300)), 150, 2, 35, foreground, display);
	uint16_t selectedColour = interpolateColour(getColourFromSaturation(getColourFromHue(hue_val), getColourFromRGB(255, 255, 255), (double)((double)sat_val / (double)256)), getColourFromRGB(0, 0, 0), (double)((double)val_val / (double)256));

	if (RGB_initiated == 0)
	{
		drawCircle(30, 67, 15, getColourFromRGB(255, 0, 0), display);
		drawCircle_line(30, 67, 15, 2, foreground, display);
		writeString("H", 45, 47, background, foreground, &font, 0, display);

		drawCircle(30, 117, 15, getColourFromRGB(0, 255, 0), display);
		drawCircle_line(30, 117, 15, 2, foreground, display);
		writeString("S", 95, 47, background, foreground, &font, 0, display);

		drawCircle(30, 167, 15, getColourFromRGB(0, 0, 255), display);
		drawCircle_line(30, 167, 15, 2, foreground, display);
		writeString("V", 145, 47, background, foreground, &font, 0, display);
	}

	if (selected_led == 1)
	{
		RGB1 = selectedColour;	
		drawRectangle(50, 190, 100, 44, foreground, display);
		drawRectangle(300, 190, 100, 44, background, display);
	}
	else if (selected_led == 2)
	{
		drawRectangle(50, 190, 100, 44, background, display);
		drawRectangle(300, 190, 100, 44, foreground, display);
		RGB2 = selectedColour;
	}
	writeString("LED1", 190, 50, (selected_led == 1 ? foreground: background), (selected_led == 1 ? background : foreground), &font, 2, display);
	drawCircle(95, 255, 20, RGB1, display);
	drawCircle_line(95, 255, 20, 2, foreground, display);

	writeString("LED2", 190, 300, (selected_led == 2 ? foreground : background), (selected_led == 2 ? background : foreground), &font, 2, display);
	drawCircle(345, 255, 20, RGB2, display);
	drawCircle_line(345, 255, 20, 2, foreground, display);

	RGB1_colour = RGB1;
	RGB2_colour = RGB2;

	if ((value >> 24) & 0x1)
	{
		selected_led = 2;
	}
	else if ((value >> 25) & 0x1)
	{
		selected_led = 1;
	}
	else if ((value >> 26) & 0x1 && RGB_initiated == 1)
	{
		reti = 0;
		displayed = MAIN_MENU;
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = background;
			}
		}
	}

	if (RGB_initiated == 0)
	{
		drawRectangle(0, display->height - 40, display->width, 2, foreground, display);

		drawCircle(10, display->height - 20, 7, getColourFromRGB(255, 0, 0), display);
		drawCircle_line(10, display->height - 20, 7, 2, foreground, display);
		writeString("Back", display->height - 25, 20, background, foreground, &font_rom8x16, 1, display);

		drawCircle(80, display->height - 20, 7, getColourFromRGB(0, 255, 0), display);
		drawCircle_line(80, display->height - 20, 7, 2, foreground, display);
		writeString("LED 1", display->height - 25, 90, background, foreground, &font_rom8x16, 1, display);

		drawCircle(150, display->height - 20, 7, getColourFromRGB(0, 0, 255), display);
		drawCircle_line(150, display->height - 20, 7, 2, foreground, display);
		writeString("LED 2", display->height - 25, 160, background, foreground, &font_rom8x16, 1, display);
		
	}
	RGB_initiated = 1;
	return reti;
}

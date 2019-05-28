#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"
#include <math.h>

#define GET_RED(val) (((val) >> 16) & 0xff)
#define GET_GREEN(val) (((val) >> 8) & 0xff)
#define GET_BLUE(val) (((val) >> 0) & 0xff)

struct Display
{
	int width;
	int height;
	uint16_t ** data;
};
typedef struct Display display_t;

void writePoint(int top, int left, uint16_t colour, display_t* display)
{
	if (top >= 0 && top < display->height && left >= 0 && left < display->width)
	{
		display->data[top][left] = colour;
	}

}


int writeChar(char c, int top, int left, uint16_t background, uint16_t foreground, font_descriptor_t *font, display_t *display)
{
	if ((unsigned)(c - font->firstchar) > font->size)
	{
		c = '?';
	}
	uint16_t char_start = (c - font->firstchar) * font->height * ((font->maxwidth + 15) / 16);
	if (font->offset != 0)
	{
		char_start = font->offset[(c - font->firstchar)];
	}
	int char_width = font->maxwidth;
	if (font->width != 0)
	{
		char_width = font->width[(c - font->firstchar)];
	}
	uint16_t *char_bitmap = (font_bits_t*)font->bits + char_start;
	for (int r = 0; r < font->height; r++)
	{
		uint16_t char_row = 0;
		for (int c = 0; c < char_width; c++)
		{
			if (c % 16 == 0)
			{
				char_row = *(char_bitmap++);
			}
			if ((char_row >> (15 - c % 16)) & (0x1))
			{

				//printf("#");
				writePoint(top + r, left + c, foreground, display);
			}
			else
			{
				//printf(".");
				writePoint(top + r, left + c, background, display);
			}
		}
		//printf("\n");
	}
	return char_width;
}

void writeString(const char* s, int top, int left, uint16_t background, uint16_t foreground, font_descriptor_t *font, int space, display_t* display)
{
	int x = top;
	int y = left;
	for (int i = 0; i < strlen(s); i++)
	{
		int width = writeChar(s[i], x, y, background, foreground, font, display);
		y += width;
		y += space;
	}
}





void showDisplay(display_t *display, unsigned char* parlcd_mem_base)
{
	*(volatile uint8_t*)(parlcd_mem_base + PARLCD_REG_CMD_o) = 0x2c;
	for (int r = 0; r < display->height; r++)
	{
		for (int c = 0; c < display->width; c++)
		{
			*(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = display->data[r][c];
		}
	}
}

uint16_t getColourFromRGB(uint16_t red, uint16_t green, uint16_t blue)
{
	return ((blue & 0b0000000000011111) | ((green << 5) & 0b0000011111100000) | ((red << 11) & 0b1111100000000000));
}

char* getTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	char* reti = malloc(9 * sizeof(char));
	char tmp[32];
	sprintf(tmp, "%d", timeinfo->tm_hour);
	if (timeinfo->tm_hour < 10)
	{
		reti[0] = '0';
		reti[1] = tmp[0];
	}
	else
	{
		reti[0] = tmp[0];
		reti[1] = tmp[1];
	}
	if (timeinfo->tm_sec % 2 == 0)
	{
		reti[2] = ':';
	}
	else
	{
		reti[2] = ' ';
	}
	sprintf(tmp, "%d", timeinfo->tm_min);
	if (timeinfo->tm_min < 10)
	{
		reti[3] = '0';
		reti[4] = tmp[0];
	}
	else
	{
		reti[3] = tmp[0];
		reti[4] = tmp[1];
	}
	if (timeinfo->tm_sec % 2 == 0)
	{
		reti[5] = ':';
	}
	else
	{
		reti[5] = ' ';
	}
	sprintf(tmp, "%d", timeinfo->tm_sec);
	if (timeinfo->tm_sec < 10)
	{
		reti[6] = '0';
		reti[7] = tmp[0];
	}
	else
	{
		reti[6] = tmp[0];
		reti[7] = tmp[1];
	}
	reti[8] = '\0';
	return reti;
}

char* getShortDate()
{
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	char* reti = malloc(11 * sizeof(char));
	char tmp[32];
	sprintf(tmp, "%d", timeinfo->tm_mday);
	if (timeinfo->tm_mday < 10)
	{
		reti[0] = '0';
		reti[1] = tmp[0];
	}
	else
	{
		reti[0] = tmp[0];
		reti[1] = tmp[1];
	}
	reti[2] = '.';

	sprintf(tmp, "%d", timeinfo->tm_mon + 1);
	if (timeinfo->tm_mon + 1 < 10)
	{
		reti[3] = '0';
		reti[4] = tmp[0];
	}
	else
	{
		reti[3] = tmp[0];
		reti[4] = tmp[1];
	}
	reti[5] = '.';
	sprintf(tmp, "%d", timeinfo->tm_year + 1900);
	reti[6] = tmp[0];
	reti[7] = tmp[1];
	reti[8] = tmp[2];
	reti[9] = tmp[3];
	reti[10] = '\0';

	return reti;
}

uint16_t scaleRed(uint16_t val)
{
	uint16_t max = 0b11111;
	uint16_t min = 0b0;
	uint16_t r_max = 0b11111111;
	uint16_t r_min = 0;
	
	uint16_t reti = round(((max - min)*(val - r_min) / (r_max - r_min)) + min);
	return reti;
}

uint16_t scaleGreen(uint16_t val)
{
	uint16_t max = 0b111111;
	uint16_t min = 0b0;
	uint16_t r_max = 0b11111111;
	uint16_t r_min = 0;

	uint16_t reti = round(((max - min)*(val - r_min) / (r_max - r_min)) + min);
	return reti;
}

uint16_t scaleBlue(uint16_t val)
{
	uint16_t max = 0b11111;
	uint16_t min = 0b0;
	uint16_t r_max = 0b11111111;
	uint16_t r_min = 0;
	uint16_t reti = round(((max - min)*(val - r_min) / (r_max - r_min)) + min);
	return reti;
}

int scaleValue(int value, int old_min, int old_max, int new_min, int new_max)
{
	int reti = round((((new_max - new_min)*(value - old_min)) / (old_max - old_min)) + new_min);
	return reti;
}

uint16_t interpolateColour(uint16_t colour_start, uint16_t colour_end, double percent)
{
	//uint8_t red = GET_RED(colour_start) + percent * (GET_RED(colour_end) - GET_RED(colour_start));
	//uint8_t green = GET_GREEN(colour_start) + percent * (GET_GREEN(colour_end) - GET_GREEN(colour_start));
	//uint8_t blue = GET_BLUE(colour_start) + percent * (GET_BLUE(colour_end) - GET_BLUE(colour_start));

	uint8_t red_start = (colour_start >> 11) & 0b00011111;
	uint8_t green_start = (colour_start >> 5) & 0b00111111;
	uint8_t blue_start = (colour_start >> 0) & 0b00011111;

	uint8_t red_end = (colour_end >> 11) & 0b00011111;
	uint8_t green_end = (colour_end >> 5) & 0b00111111;
	uint8_t blue_end = (colour_end >> 0) & 0b00011111;

	uint8_t red = red_start + (percent * (red_end - red_start));
	uint8_t green = green_start + (percent * (green_end - green_start));
	uint8_t blue = blue_start + (percent * (blue_end - blue_start));

	return getColourFromRGB(red, green, blue);
}

uint16_t getColourFromHue(int hue)
{
	uint16_t reti = 0;
	int scaled = scaleValue(hue, 0, 0b11111111, 0, 300);
	if (scaled < 50)
	{
		reti = interpolateColour(getColourFromRGB(255, 0, 0), getColourFromRGB(255, 255, 0), (double)((double)hue / (double)50));
	}
	else if (scaled >= 50 && scaled < 100)
	{
		reti = interpolateColour(getColourFromRGB(255, 255, 0), getColourFromRGB(0, 255, 0), (double)((double)(hue - 50) / (double)50));
	}
	else if (scaled >= 100 && scaled < 150)
	{
		reti = interpolateColour(getColourFromRGB(0, 255, 0), getColourFromRGB(0, 255, 255), (double)((double)(hue - 100) / (double)50));
	}
	else if (scaled >= 150 && scaled < 200)
	{
		reti = interpolateColour(getColourFromRGB(0, 255, 255), getColourFromRGB(0, 0, 255), (double)((double)(hue - 150) / (double)50));
	}
	else if (scaled >= 200 && scaled < 250)
	{
		reti = interpolateColour(getColourFromRGB(0, 0, 255), getColourFromRGB(255, 0, 255), (double)((double)(hue - 200) / (double)50));
	}
	else if (scaled >= 250)
	{
		reti = interpolateColour(getColourFromRGB(255, 0, 255), getColourFromRGB(255, 0, 0), (double)((double)(hue - 250) / (double)50));
	}
	return reti;
}

uint16_t getColourFromSaturation(uint16_t start_colour, uint16_t end_colour, double percentage)
{
	return(interpolateColour(start_colour, end_colour, percentage));
}

void displayColorRGB1(uint16_t colour)
{
	unsigned char *phys_mem_base;
	phys_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	uint16_t red = ((0 | (colour >> 11)) & (0 | 0b00011111));
	uint16_t green = ((0 |(colour >> 5)) & (0 | 0b00111111));
	uint16_t blue = ((0 | (colour >> 0)) & (0 | 0b00011111));
	uint32_t regval = (0 | ((red << 16) | (green << 8) | (blue << 0)));

	*(volatile uint16_t*)(phys_mem_base + SPILED_REG_LED_RGB1_o) = regval;
}

void displayColorRGB2(uint16_t colour)
{
	unsigned char *phys_mem_base;
	phys_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	uint16_t red = ((0 | (colour >> 11)) & (0 | 0b00011111));
	uint16_t green = ((0 | (colour >> 5)) & (0 | 0b00111111));
	uint16_t blue = ((0 | (colour >> 0)) & (0 | 0b00011111));
	uint32_t regval = (0 | ((red << 16) | (green << 8) | (blue << 0)));

	*(volatile uint16_t*)(phys_mem_base + SPILED_REG_LED_RGB2_o) = regval;
}
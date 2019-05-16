#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"

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
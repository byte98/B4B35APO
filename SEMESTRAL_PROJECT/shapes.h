#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"
#include <math.h>

void drawCircle(int left, int top, int radius, uint16_t colour, display_t *display)
{


	for (int r = 0; r < display->height; r++)
	{
		for (int c = 0; c < display->width; c++)
		{
			if (((c - left) * (c - left)) + ((r - top) * (r - top)) < (radius * radius))
			{
				writePoint(r, c, colour, display);
			}

		}
	}
}

int computeDistance(int a_top, int a_left, int b_top, int b_left)
{
	
	return (int)ceil(sqrt(pow(abs(b_top - a_top), 2) + pow(abs(b_left - a_left), 2)));
}

void drawCircle_line(int left, int top, int radius, int width, uint16_t colour, display_t *display)
{


	for (int r = 0; r < display->height; r++)
	{
		for (int c = 0; c < display->width; c++)
		{
			if (computeDistance(top, left, r, c) >= (radius - width) && computeDistance(top, left, r, c) <= radius)
			{
				writePoint(r, c, colour, display);
			}

		}
	}
}

uint16_t interpolateColour(uint16_t colour_start, uint16_t colour_end, double percent)
{
	uint8_t red = GET_RED(colour_start) + percent * (GET_RED(colour_end) - GET_RED(colour_start));
	uint8_t green = GET_GREEN(colour_start) + percent * (GET_GREEN(colour_end) - GET_GREEN(colour_start));
	uint8_t blue = GET_BLUE(colour_start) + percent * (GET_BLUE(colour_end) - GET_BLUE(colour_start));
	return getColourFromRGB(red, green, blue);
}

void drawGradient(int left, int top, int width, int height, uint16_t colour_start, uint16_t colour_end, display_t *display)
{
	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			writePoint(top + r, left + c, interpolateColour(colour_start, colour_end,(double)((double)c / (double)width)) , display);
		}
	}
}
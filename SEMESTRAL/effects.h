#ifndef __EFFECTS_H__
#define __EFFECTS_H__

uint32_t getNextColour(uint32_t colour)
{
	uint32_t red = (colour >> 16) & (0b00000000000000000000000011111111);
	uint32_t green = (colour >> 8) & (0b00000000000000000000000011111111);
	uint32_t blue = (colour >> 0) & (0b00000000000000000000000011111111);

	uint32_t reti_red = 0;
	uint32_t reti_green = 0;
	uint32_t reti_blue = 0;

	if (red == 255 && blue == 0 && green < 255)       // RED
	{
		reti_red = red;
		reti_green = (green + 1);
		reti_blue = blue;
	}
	else if (green == 255 && blue == 0 && red > 0)    // YELLOW
	{
		reti_red = (red - 1);
		reti_green = green;
		reti_blue = blue;
	}
	else if (red == 0 && green == 255 && blue < 255)  // GREEN
	{
		reti_red = red;
		reti_green = green;
		reti_blue = (blue + 1);
	}
	else if (red == 0 && green > 0 && blue == 255)     // CYAN
	{
		reti_red = red;
		reti_green = (green - 1);
		reti_blue = blue;
	}
	else if (red < 255 && green == 0 && blue == 255)   // BLUE
	{
		reti_red = (red + 1);
		reti_green = green;
		reti_blue = blue;
	}
	else if (red == 255 && green == 0 && blue > 0)
	{
		reti_red = red;
		reti_green = green;
		reti_blue = (blue - 1);
	}
	else
	{
		reti_red = 255;
		reti_green = 0;
		reti_blue = 0;
	}
	uint32_t reti = (reti_red << 16) | (reti_green << 8) | (reti_blue << 0);
	return reti;
}

uint32_t getOppositeColour(uint32_t colour)
{
	uint32_t red = (colour >> 16) & (0b00000000000000000000000011111111);
	uint32_t green = (colour >> 8) & (0b00000000000000000000000011111111);
	uint32_t blue = (colour >> 0) & (0b00000000000000000000000011111111);

	uint32_t reti_red = 255 - red;
	uint32_t reti_green = 255 - green;
	uint32_t reti_blue = 255 - blue;

	uint32_t reti = (reti_red << 16) | (reti_green << 8) | (reti_blue << 0);
	return reti;
}
#endif // !__EFFECTS_H__

#pragma once

void drawBlinking(display_t *display, uint16_t background, uint16_t foreground, font_descriptor_t font)
{
	unsigned char *knobs_mem_base;
	knobs_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	writeString("Blinking", 19, 10, default_background, default_foreground, &default_font, 2, display);
	writeString("LED1", 57, 10, default_background, default_foreground, &default_font, 2, display);
	drawGradient(10, 110, 400, 50, getColourFromRGB(255, 0, 0), getColourFromRGB(0, 255, 0), display);
	writeString("LED2", 160, 10, default_background, default_foreground, &default_font, 2, display);
	drawGradient(10, 210, 400, 50, getColourFromRGB(255, 0, 0), getColourFromRGB(0, 255, 0), display);
	writeString("OFF", 100, 10, default_background, getColourFromRGB(255, 0, 0), &font_rom8x16, 1, display);
	writeString("MAX", 100, 390, default_background, getColourFromRGB(0, 255, 0), &font_rom8x16, 1, display);

	writeString("OFF", 200, 10, default_background, getColourFromRGB(255, 0, 0), &font_rom8x16, 1, display);
	writeString("MAX", 200, 390, default_background, getColourFromRGB(0, 255, 0), &font_rom8x16, 1, display);

	uint32_t value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);
	uint32_t green_val = GET_GREEN(value);
	uint32_t blue_val = GET_BLUE(value);

	drawRectangle(10 + scaleValue(green_val, 0, 0b11111111, 0, 400), 110, 2, 50, default_foreground, display);
	drawRectangle(10 + scaleValue(blue_val, 0, 0b11111111, 0, 400), 210, 2, 50, default_foreground, display);

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
	int max = 10000;
	if (green_val < 5)
	{
		RGB1_blink = 0;
	}	
	else if (green_val > 252)
	{
		RGB1_blink = 100;
	}
	else
	{
		RGB1_blink = scaleValue((255 - green_val), 0, 255, 100, max);
	}

	if (blue_val < 5)
	{
		RGB2_blink = 0;
	}	
	else if (blue_val > 250)
	{
		RGB2_blink = 100;
	}
	else
	{
		RGB2_blink = scaleValue((255 - blue_val), 0, 255, 100, max);
	}

	if (((value >> 26) == 1 ))
	{
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = default_background;
			}
		}
		displayed = MAIN_MENU;
		return;
	}
}
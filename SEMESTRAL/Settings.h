#pragma once

int settings_initiaited = 0;
int selected_setting = 0;
int selected_txtset = 0;
int txtset_init = 0;
int selected_remote = 0;
unsigned char *knobs_mem_base;
int selected_txt = 0;

void drawTextSettings(display_t *display, uint16_t background, uint16_t foreground, font_descriptor_t font);
void drawBrightSettings(display_t *display, uint16_t background, uint16_t foreground, font_descriptor_t font);
void drawRemoteSettings(display_t *display, uint16_t background, uint16_t foreground, font_descriptor_t font);

void drawSettings(display_t *display, uint16_t background, uint16_t foreground, font_descriptor_t font)
{
	if (settings_initiaited == 0)
	{
		knobs_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = background;
			}
		}
	}
	uint32_t value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);
	uint32_t menu_val = GET_RED(value);
	selected_setting = ((int)ceil((menu_val / 9)) % 3);
	if ((value >> 25) == 1)
	{
		displayed = MAIN_MENU;
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = background;
			}
		}
		delay(SYS_INPUT_DELAY);
		return;
	}
	else if ((value >> 26) == 1 && selected_setting == 0)
	{
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = background;
			}
		}
		displayed = TEXT_SETTINGS;
		delay(SYS_INPUT_DELAY);
		return;
	}
	else if ((value >> 26) == 1 && selected_setting == 1)
	{
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = background;
			}
		}
		displayed = BRIGHT_SETTINGS;
		delay(SYS_INPUT_DELAY);
		return;
	}
	else if ((value >> 26) == 1 && selected_setting == 2)
	{
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = background;
			}
		}
		displayed = REMOTE_SETTINGS;
		delay(SYS_INPUT_DELAY);
		return;
	}

	if (selected_setting == 0)
	{
		drawRectangle(10, 100, display->width - 20, 45, default_foreground, display);
		drawRectangle(10, 200, display->width - 20, 45, default_background, display);
		drawRectangle(10, 150, display->width - 20, 45, default_background, display);
	}
	else if (selected_setting == 1)
	{
		drawRectangle(10, 150, display->width - 20, 45, default_foreground, display);
		drawRectangle(10, 100, display->width - 20, 45, default_background, display);
		drawRectangle(10, 200, display->width - 20, 45, default_background, display);
	}
	else if (selected_setting == 2)
	{
		drawRectangle(10, 200, display->width - 20, 45, default_foreground, display);
		drawRectangle(10, 150, display->width - 20, 45, default_background, display);
		drawRectangle(10, 100, display->width - 20, 45, default_background, display);
	}

	writeString("Settings", 50, 10, default_background, default_foreground, &default_font, 2, display);
	writeString("Text size", 100, 10, (selected_menu == 0 ? default_foreground : default_background), (selected_menu == 0 ? default_background : default_foreground), &default_font, 2, display);
	writeString("Brightness", 150, 10, (selected_menu == 1 ? default_foreground : default_background), (selected_menu == 1 ? default_background : default_foreground), &default_font, 2, display);
	writeString("Remote control", 200, 10, (selected_menu == 2 ? default_foreground : default_background), (selected_menu == 2 ? default_background : default_foreground), &default_font, 2, display);
	drawRectangle(0, display->height - 40, display->width, 2, default_foreground, display);

	drawCircle(10, display->height - 20, 7, getColourFromRGB(255, 0, 0), display);
	drawCircle_line(10, display->height - 20, 7, 2, default_foreground, display);
	writeString("Select", display->height - 25, 20, default_background, default_foreground, &font_rom8x16, 1, display);

	drawCircle(90, display->height - 20, 7, getColourFromRGB(0, 255, 0), display);
	drawCircle_line(90, display->height - 20, 7, 2, default_foreground, display);
	writeString("Back", display->height - 25, 100, default_background, default_foreground, &font_rom8x16, 1, display);
	settings_initiaited = 1;
}

void drawTextSettings(display_t *display, uint16_t background, uint16_t foreground, font_descriptor_t font)
{
	uint32_t value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);
	uint32_t menu_val = GET_RED(value);
	selected_txt = ((int)ceil((menu_val / 9)) % 2);

	if (selected_txt == 0)
	{
		drawRectangle(10, 100, display->width - 20, 45, default_foreground, display);
		drawRectangle(10, 200, display->width - 20, 45, default_background, display);
		drawRectangle(10, 150, display->width - 20, 45, default_background, display);
	}
	else if (selected_txt == 1)
	{
		drawRectangle(10, 150, display->width - 20, 45, default_foreground, display);
		drawRectangle(10, 100, display->width - 20, 45, default_background, display);
		drawRectangle(10, 200, display->width - 20, 45, default_background, display);
	}

	writeString("Text size", 50, 10, default_background, default_foreground, &default_font, 2, display);
	writeString("Small", 100, 10, (selected_txt == 0 ? default_foreground : default_background), (selected_txt == 0 ? default_background : default_foreground), &font_winFreeSystem14x16, 2, display);
	writeString("Large", 150, 10, (selected_txt == 1 ? default_foreground : default_background), (selected_txt == 1 ? default_background : default_foreground), &font_wArial_44, 2, display);	drawRectangle(0, display->height - 40, display->width, 2, default_foreground, display);

	drawCircle(10, display->height - 20, 7, getColourFromRGB(255, 0, 0), display);
	drawCircle_line(10, display->height - 20, 7, 2, default_foreground, display);
	writeString("Select", display->height - 25, 20, default_background, default_foreground, &font_rom8x16, 1, display);

	drawCircle(90, display->height - 20, 7, getColourFromRGB(0, 255, 0), display);
	drawCircle_line(90, display->height - 20, 7, 2, default_foreground, display);
	writeString("Back", display->height - 25, 100, default_background, default_foreground, &font_rom8x16, 1, display);
	txtset_init = 1;
	if ((value >> 25) == 1)
	{
		displayed = SETTINGS;
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = background;
			}
		}
		delay(SYS_INPUT_DELAY);
		return;
	}
	else if ((value >> 26) == 1)
	{
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = background;
			}
		}
		default_font = (selected_txt == 0 ? font_winFreeSystem14x16 : font_wArial_44);
		displayed = SETTINGS;
		delay(SYS_INPUT_DELAY);
		return;
	}
}

void drawBrightSettings(display_t *display, uint16_t background, uint16_t foreground, font_descriptor_t font)
{
	for (int r = 0; r < display->height; r++)
	{
		for (int c = 0; c < display->width; c++)
		{
			display->data[r][c] = background;
		}
	}
	writeString("Brightness", 50, 10, default_background, default_foreground, &default_font, 2, display);
	drawGradient(10, 100, 400, 50, getColourFromRGB(0, 0, 0), getColourFromRGB(255, 255, 255), display);
	drawRectangle(10 + scaleValue(brightness, 0, 0b11111111, 0, 400), 100, 2, 50, foreground, display);
	uint32_t value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);
	uint32_t bright_val = GET_RED(value);
	brightness = bright_val;

	drawCircle(10, display->height - 20, 7, getColourFromRGB(255, 0, 0), display);
	drawCircle_line(10, display->height - 20, 7, 2, default_foreground, display);
	writeString("Select", display->height - 25, 20, default_background, default_foreground, &font_rom8x16, 1, display);

	drawCircle(90, display->height - 20, 7, getColourFromRGB(0, 255, 0), display);
	drawCircle_line(90, display->height - 20, 7, 2, default_foreground, display);
	writeString("Back", display->height - 25, 100, default_background, default_foreground, &font_rom8x16, 1, display);

	default_background = getColourFromRGB(0, scaleValue(bright_val, 0, 0b11111111, 0, 0b111111), 0);

	if ((value >> 25) == 1)
	{
		displayed = SETTINGS;
		delay(SYS_INPUT_DELAY);
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = background;
			}
		}
		return;
	}
}

void drawRemoteSettings(display_t *display, uint16_t background, uint16_t foreground, font_descriptor_t font)
{
drawCircle(10, display->height - 20, 7, getColourFromRGB(255, 0, 0), display);
	drawCircle_line(10, display->height - 20, 7, 2, default_foreground, display);
	writeString("Select", display->height - 25, 20, default_background, default_foreground, &font_rom8x16, 1, display);

	drawCircle(90, display->height - 20, 7, getColourFromRGB(0,255, 0), display);
	drawCircle_line(90, display->height - 20, 7, 2, default_foreground, display);
	writeString("Back", display->height - 25, 100, default_background, default_foreground, &font_rom8x16, 1, display);
	uint32_t value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);
	uint32_t menu_val = GET_RED(value);
	selected_remote = ((int)ceil((menu_val / 9)) % 2);
	
	if (selected_remote == 0)
	{
		drawRectangle(10, 100, display->width - 20, 45, default_foreground, display);
		drawRectangle(10, 150, display->width - 20, 45, default_background, display);
	}
	else if (selected_remote == 1)
	{
		drawRectangle(10, 150, display->width - 20, 45, default_foreground, display);
		drawRectangle(10, 100, display->width - 20, 45, default_background, display);
	}
	writeString("Remote control", 50, 10, default_background, default_foreground, &default_font, 2, display);
	writeString("Allowed", 100, 10, (selected_remote == 0 ? default_foreground : default_background), (selected_remote == 0 ? default_background : default_foreground), &default_font, 2, display);
	writeString("Denied", 150, 10, (selected_remote == 1 ? default_foreground : default_background), (selected_remote == 1 ? default_background : default_foreground), &default_font, 2, display);
	writeString("Remote control is available on:", display->height - 100, 10, background, foreground, &font_winFreeSystem14x16, 1, display);
	writeString(RC_SERVER_DISPLAY, display->height - 65, 10, background, foreground, &font_winFreeSystem14x16, 1, display);

	if ((value >> 25) == 1)
	{
		displayed = SETTINGS;
		delay(SYS_INPUT_DELAY);
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = background;
			}
		}
		return;
	}
	else if ((value >> 26) == 1 && selected_remote == 0)
	{
		remote_allowed = 1;
		update_remote_status();
		displayed = SETTINGS;
		delay(SYS_INPUT_DELAY);
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = background;
			}
		}
		return;
	}
	else if ((value >> 26) == 1 && selected_remote == 1)
	{
		remote_allowed = 0;
		update_remote_status();
		displayed = SETTINGS;
		delay(SYS_INPUT_DELAY);
		for (int r = 0; r < display->height; r++)
		{
			for (int c = 0; c < display->width; c++)
			{
				display->data[r][c] = background;
			}
		}
		return;
	}
}
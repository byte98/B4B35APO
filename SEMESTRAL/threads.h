void *RGB1_display(void *vargp)
{
	unsigned char *phys_mem_base;
	phys_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	if (RC_RGB1_MODE == RC_MODE_VARIABLE)
	{
		RC_RGB1_COLOUR = (255 << 16);

		while (RC_RGB1_MODE == RC_MODE_VARIABLE)
		{
			*(volatile uint32_t*)(phys_mem_base + SPILED_REG_LED_RGB1_o) = RC_RGB1_COLOUR;
			RC_RGB1_COLOUR = getNextColour(RC_RGB1_COLOUR);
			usleep(RC_RGB1_DELAY * 1000);
		}
	}
	else if (RC_RGB1_MODE == RC_MODE_VARIABLE_OPPOSITE)
	{
		RC_RGB1_COLOUR = getOppositeColour(RC_RGB1_COLOUR);
		
		while (RC_RGB1_MODE == RC_MODE_VARIABLE_OPPOSITE)
		{
			*(volatile uint32_t*)(phys_mem_base + SPILED_REG_LED_RGB1_o) = RC_RGB1_COLOUR;
			RC_RGB1_COLOUR = getOppositeColour(RC_RGB1_COLOUR);
		}
	}
	else if (RC_RGB1_MODE == RC_MODE_STATIC)
	{
		*(volatile uint32_t*)(phys_mem_base + SPILED_REG_LED_RGB1_o) = RC_RGB1_COLOUR;
	}
}

void *RGB2_display(void *vargp)
{
	unsigned char *phys_mem_base;
	phys_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	if (RC_RGB2_MODE == RC_MODE_VARIABLE)
	{

		RC_RGB2_COLOUR = (255 << 16);

		while (RC_RGB2_MODE == RC_MODE_VARIABLE)
		{
			*(volatile uint32_t*)(phys_mem_base + SPILED_REG_LED_RGB2_o) = RC_RGB2_COLOUR;
			RC_RGB2_COLOUR = getNextColour(RC_RGB2_COLOUR);
			usleep(RC_RGB1_DELAY * 1000);
		}
	}
	else if (RC_RGB2_MODE == RC_MODE_VARIABLE_OPPOSITE)
	{
		RC_RGB2_COLOUR = getOppositeColour(RC_RGB1_COLOUR);
		while (RC_RGB2_MODE == RC_MODE_VARIABLE_OPPOSITE)
		{
			*(volatile uint32_t*)(phys_mem_base + SPILED_REG_LED_RGB2_o) = RC_RGB2_COLOUR;
			RC_RGB2_COLOUR = getOppositeColour(RC_RGB1_COLOUR);
		}
	}
	else if (RC_RGB2_MODE == RC_MODE_STATIC)
	{
		if (RC_RGB2_BLICK_MODE == RC_BLICK_MODE_NONE)
		{
			*(volatile uint32_t*)(phys_mem_base + SPILED_REG_LED_RGB2_o) = RC_RGB2_COLOUR;
		}
		else if (RC_RGB2_BLICK_MODE == RC_BLICK_MODE_SIMPLE)
		{
			while (RC_RGB2_MODE == RC_MODE_STATIC && RC_RGB2_BLICK_MODE == RC_BLICK_MODE_SIMPLE)
			{
				*(volatile uint32_t*)(phys_mem_base + SPILED_REG_LED_RGB2_o) = RC_RGB2_COLOUR;
				delay(RC_RGB2_BLICK_LIGHT);
				*(volatile uint32_t*)(phys_mem_base + SPILED_REG_LED_RGB2_o) = 0;
				delay(RC_RGB2_BLICK_DARK);
			}
		}

	}
}

void *load_remote(void* vargp)
{
	if (remote_allowed == 1)
	{

	}
}

void connect()
{

}

void delay(int miliseconds)
{
	clock_t start_time = clock();
	while (clock() < start_time + (miliseconds * 1000))
	{
	}

}
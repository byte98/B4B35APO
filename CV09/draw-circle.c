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

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#define WIDTH 480
#define HEIGHT 320

#define GREEN 0x7e0
#define WHITE 0xffff
#define BLUE 0x1f
#define RED 0xf800
#define BLACK 0x0

#define GET_RED(val) (((val) >> 16) & 0xff)
#define GET_GREEN(val) (((val) >> 8) & 0xff)
#define GET_BLUE(val) (((val) >> 0) & 0xff)

void drawCircle(int left, int top, int radius, unsigned char* parlcd_mem_base, uint16_t colour);

int main(int argc, char *argv[])
{
  unsigned char *parlcd_mem_base;
  parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  parlcd_hx8357_init(parlcd_mem_base);

  unsigned char *knobs_mem_base;
  knobs_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);

  uint16_t colour = BLACK;

  while (1)
  {
    uint32_t value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);
    uint32_t blue_val = GET_BLUE(value);
    uint32_t green_val = GET_GREEN(value);
    uint32_t red_val = GET_RED(value);

    *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_LINE_o) = value;
    if ((value >> 24) & 0x1)
    {
      colour = BLUE;
    }
    else if((value >> 25) & 0x1)
    {
      colour = GREEN;
    }
    else if((value >> 26) & 0x1)
    {
      colour = RED;
    }
    drawCircle(red_val, green_val, blue_val, parlcd_mem_base, colour);
    

  }

  return 0;
}


void drawCircle(int left, int top, int radius, unsigned char* parlcd_mem_base, uint16_t colour)
{


  *(volatile uint8_t*)(parlcd_mem_base + PARLCD_REG_CMD_o) = 0x2c;
  for (int r = 0; r < HEIGHT; r++)
  {
    for (int c = 0; c < WIDTH; c++)
    {
      if (((c - left) * (c - left)) + ((r - top) * (r - top)) < (radius * radius))
      {
        *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = WHITE;
      }
      else
      {
        *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = colour;
      }
      

    }
  }
}

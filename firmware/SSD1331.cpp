/*
* SSD1331.cpp
* A library for RGB OLED module
*
* Copyright (c) 2014 seeed technology inc.
* Copyright (c) 2012, Adafruit Industries.
*
* All rights reserved.
*
* This library is based on Adafruit's SSD1331-OLED-Driver-Library. Thanks to 
* their contribution to the code, we modify it and add more interface to 
* support our Seeed's Xadow RGB OLED 96*64 module.
*
* Below is the introduction of Adafruit's Color OLED module, we add it to here
* to express our thanks to them.
*
* ****************************************************************************
* This is a library for the 0.96" 16-bit Color OLED with SSD1331 driver chip
*
*  Pick one up today in the adafruit shop!
*  ------> http://www.adafruit.com/products/684
*
* These displays use SPI to communicate.
*
* Adafruit invests time and resources providing this open source code, 
* please support Adafruit and open-source hardware by purchasing 
* products from Adafruit!
*
* Written by Limor Fried/Ladyada for Adafruit Industries.
* Modifed by lawliet for Seeed Studio's RGB OLED module.
* BSD license, all text above must be included in any redistribution
* ******************************************************************************
*
* Software License Agreement (BSD License)
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holders nor the
* names of its contributors may be used to endorse or promote products
* derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include "SSD1331.h"
#include "application.h"

SSD1331::SSD1331(uint8_t cs, uint8_t dc, uint8_t mosi, uint8_t sck) : SGL(RGB_OLED_WIDTH, RGB_OLED_HEIGHT)
{
    _cs = cs;
    _dc = dc;
    _mosi = mosi;
    _sck = sck;
};

void SSD1331::_sendCmd(uint8_t c)
{
    digitalWrite(_dc,LOW);
    digitalWrite(_cs,LOW);
    SPI.transfer(c);
    digitalWrite(_cs,HIGH);
}

void SSD1331::init(void)
{
    pinMode(_dc, OUTPUT);
    pinMode(_cs, OUTPUT);

    SPI.begin();

    _sendCmd(CMD_DISPLAY_OFF);          //Display Off
    _sendCmd(CMD_SET_CONTRAST_A);       //Set contrast for color A
    _sendCmd(0x91);                     //145
    _sendCmd(CMD_SET_CONTRAST_B);       //Set contrast for color B
    _sendCmd(0x50);                     //80
    _sendCmd(CMD_SET_CONTRAST_C);       //Set contrast for color C
    _sendCmd(0x7D);                     //125
    _sendCmd(CMD_MASTER_CURRENT_CONTROL);//master current control
    _sendCmd(0x06);                     //6
    _sendCmd(CMD_SET_PRECHARGE_SPEED_A);//Set Second Pre-change Speed For ColorA
    _sendCmd(0x64);                     //100
    _sendCmd(CMD_SET_PRECHARGE_SPEED_B);//Set Second Pre-change Speed For ColorB
    _sendCmd(0x78);                     //120
    _sendCmd(CMD_SET_PRECHARGE_SPEED_C);//Set Second Pre-change Speed For ColorC
    _sendCmd(0x64);                     //100
    _sendCmd(CMD_SET_REMAP);            //set remap & data format
    _sendCmd(0x72);                     //0x72              
    _sendCmd(CMD_SET_DISPLAY_START_LINE);//Set display Start Line
    _sendCmd(0x0);
    _sendCmd(CMD_SET_DISPLAY_OFFSET);   //Set display offset
    _sendCmd(0x0);
    _sendCmd(CMD_NORMAL_DISPLAY);       //Set display mode
    _sendCmd(CMD_SET_MULTIPLEX_RATIO);  //Set multiplex ratio
    _sendCmd(0x3F);
    _sendCmd(CMD_SET_MASTER_CONFIGURE); //Set master configuration
    _sendCmd(0x8E);
    _sendCmd(CMD_POWER_SAVE_MODE);      //Set Power Save Mode
    _sendCmd(0x00);                     //0x00
    _sendCmd(CMD_PHASE_PERIOD_ADJUSTMENT);//phase 1 and 2 period adjustment
    _sendCmd(0x31);                     //0x31
    _sendCmd(CMD_DISPLAY_CLOCK_DIV);    //display clock divider/oscillator frequency
    _sendCmd(0xF0);
    _sendCmd(CMD_SET_PRECHARGE_VOLTAGE);//Set Pre-Change Level
    _sendCmd(0x3A);
    _sendCmd(CMD_SET_V_VOLTAGE);        //Set vcomH
    _sendCmd(0x3E);
    _sendCmd(CMD_DEACTIVE_SCROLLING);   //disable scrolling
    _sendCmd(CMD_NORMAL_BRIGHTNESS_DISPLAY_ON);//set display on
}

void SSD1331::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x < 0) || (x >= RGB_OLED_WIDTH) || (y < 0) || (y >= RGB_OLED_HEIGHT))
        return;
    //set column point
    _sendCmd(CMD_SET_COLUMN_ADDRESS);
    _sendCmd(x);
    _sendCmd(RGB_OLED_WIDTH-1);
    //set row point
    _sendCmd(CMD_SET_ROW_ADDRESS);
    _sendCmd(y);
    _sendCmd(RGB_OLED_HEIGHT-1);
    //fill 16bit colour
    digitalWrite(_dc,HIGH);
    digitalWrite(_cs,LOW);
    SPI.transfer(color >> 8);
    SPI.transfer(color);
    digitalWrite(_cs,HIGH);
}

void SSD1331::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    if((x0 < 0) || (y0 < 0) || (x1 < 0) || (y1 < 0))
        return;

    if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_WIDTH - 1;
    if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_HEIGHT - 1;
    if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_WIDTH - 1;
    if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_HEIGHT - 1;

    _sendCmd(CMD_DRAW_LINE);//draw line
    _sendCmd(x0);//start column
    _sendCmd(y0);//start row
    _sendCmd(x1);//end column
    _sendCmd(y1);//end row
    _sendCmd((uint8_t)((color>>11)&0x1F));//R
    _sendCmd((uint8_t)((color>>5)&0x3F));//G
    _sendCmd((uint8_t)(color&0x1F));//B
}

void SSD1331::drawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t outColor, uint16_t fillColor)
{
    if((x0 < 0) || (y0 < 0) || (x1 < 0) || (y1 < 0))
        return;

    if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_WIDTH - 1;
    if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_HEIGHT - 1;
    if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_WIDTH - 1;
    if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_HEIGHT - 1;

    _sendCmd(CMD_FILL_WINDOW);//fill window
    _sendCmd(ENABLE_FILL);
    _sendCmd(CMD_DRAW_RECTANGLE);//draw rectangle
    _sendCmd(x0);//start column
    _sendCmd(y0);//start row
    _sendCmd(x1);//end column
    _sendCmd(y1);//end row
    _sendCmd((uint8_t)((outColor>>11)&0x1F));//R
    _sendCmd((uint8_t)((outColor>>5)&0x3F));//G
    _sendCmd((uint8_t)(outColor&0x1F));//B
    _sendCmd((uint8_t)((fillColor>>11)&0x1F));//R
    _sendCmd((uint8_t)((fillColor>>5)&0x3F));//G
    _sendCmd((uint8_t)(fillColor&0x1F));//B
}

void SSD1331::copyWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2)
{
    _sendCmd(CMD_COPY_WINDOW);//copy window
    _sendCmd(x0);//start column
    _sendCmd(y0);//start row
    _sendCmd(x1);//end column
    _sendCmd(y1);//end row
    _sendCmd(x2);//new column
    _sendCmd(y2);//new row
}

void SSD1331::dimWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    _sendCmd(CMD_DIM_WINDOW);//copy area
    _sendCmd(x0);//start column
    _sendCmd(y0);//start row
    _sendCmd(x1);//end column
    _sendCmd(y1);//end row
}

void SSD1331::clearWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    _sendCmd(CMD_CLEAR_WINDOW);//clear window
    _sendCmd(x0);//start column
    _sendCmd(y0);//start row
    _sendCmd(x1);//end column
    _sendCmd(y1);//end row
}

void SSD1331::setScrolling(ScrollingDirection direction, uint8_t rowAddr, uint8_t rowNum, uint8_t timeInterval)
{
    uint8_t scrolling_horizontal = 0x0;
    uint8_t scrolling_vertical = 0x0;
    switch(direction){
        case Horizontal:
            scrolling_horizontal = 0x01;
            scrolling_vertical = 0x00;
            break;
        case Vertical:
            scrolling_horizontal = 0x00;
            scrolling_vertical = 0x01;
            break;
        case Diagonal:
            scrolling_horizontal = 0x01;
            scrolling_vertical = 0x01;
            break;
        default:
            break;
    }
    _sendCmd(CMD_CONTINUOUS_SCROLLING_SETUP);
    _sendCmd(scrolling_horizontal);
    _sendCmd(rowAddr);
    _sendCmd(rowNum);
    _sendCmd(scrolling_vertical);
    _sendCmd(timeInterval);
    _sendCmd(CMD_ACTIVE_SCROLLING);
}

void SSD1331::enableScrolling(bool enable)
{
    if(enable)
        _sendCmd(CMD_ACTIVE_SCROLLING);
    else
        _sendCmd(CMD_DEACTIVE_SCROLLING);
}

void SSD1331::setDisplayMode(DisplayMode mode)
{
    _sendCmd(mode);
}

void SSD1331::setDisplayPower(DisplayPower power)
{
    _sendCmd(power);
}


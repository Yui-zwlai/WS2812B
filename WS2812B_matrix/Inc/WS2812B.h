#ifndef __WS2812B_H
#define __WS2812B_H

#include "main.h"

#include "tim.h"
#include "spi.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define TYPE_LINE 0
#define TYPE_MATRIX 1

#define MODE_PWM_DMA 0
#define MODE_SPI 1

#define RED_INDEX   0
#define GREEN_INDEX 1
#define BLUE_INDEX  2
#define LED_NUM     16*16
#define GRB_BIT     24   //3*8

#if MODE_PWM_DMA
#define TIMING_ONE  58
#define TIMING_ZERO 22
#elif MODE_SPI
#define TIMING_ONE  0xFC
#define TIMING_ZERO 0xC0
#endif

#if TYPE_LINE
#define BUFF_SIZE (LED_NUM*(GRB_BIT+4))
#elif TYPE_MATRIX
#define BUFF_SIZE ((LED_NUM*GRB_BIT)+2)
#endif
#define SPEED 10

#pragma pack(4)
typedef struct 
{
    uint16_t start;
    struct rgb
    {
        uint16_t G[8];
        uint16_t R[8];
        uint16_t B[8];
    }RGB[LED_NUM];
    uint16_t stop;
}ws2816b_data;
#pragma pack()

void WS2812_Init(void);
#if TYPE_LINE
void WS2812_send(uint8_t *rgb, uint16_t len);
void WS2812_Waterfall_light(uint8_t *rgb,uint16_t len)
void WS2812_Double_Color_Multistage_Waterfall_light(uint8_t *rgb1,uint8_t *rgb2,uint16_t len);//˫ɫ�����ˮ��...���� 
void WS2812_Wave_light(uint8_t *rgb,uint16_t len);//���˵�
#elif TYPE_MATRIX
void WS2812_send(uint8_t *rgb);
void WS2812_Waterfall_light(uint8_t *rgb,uint16_t len);//����ɫ������ˮ��
void WS2812_Display_Chinese_Characters(uint8_t *rgb);//��ʾ����
#endif


void WS2812_Multistage_Waterfall_light(uint8_t *rgb,uint16_t b_len,uint16_t d_len);//����ɫ�����ˮ��...���� 
void WS2812_Breathing_light(uint8_t *rgb);//������
void WS2812_Rainbow_Flow(void);//�ʺ��
void WS2812_RGB_GradualChange_Waterfall(void);//RGB����+��ˮ

void WS2812_Colourful_Waterfall_light(uint8_t *rgb[],uint8_t color_num);//�����ˮ��
void WS2812_Colorful_Jump_light(uint8_t *rgb[],uint8_t color_num);//�������






#endif


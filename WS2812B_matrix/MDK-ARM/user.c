#include "user.h"

uint8_t color_black[] = {0, 0, 0};
uint8_t color_white[] = {255,255,255};    
uint8_t color_red[] = {255, 0, 0};
uint8_t color_yellow[] = {255,255,0};
uint8_t color_green[] = {0, 255, 0};
uint8_t color_cyan[] = {0,255,255};
uint8_t color_blue[] = {0, 0, 255};
uint8_t color_pink[] = {255,0,255};
uint8_t color_orange[] = {255,32,0};
uint8_t color_purple[] = {128,0,128};

uint8_t *color[] = {color_red,color_yellow,color_green,color_cyan,color_blue,color_pink,color_orange,color_purple};

extern uint8_t LED_MODE;
//extern uint8_t R;
//extern uint8_t G;
//extern uint8_t B;
uint8_t Data[1];
uint8_t RGB_color[3] = {0,0,0};
extern ws2816b_data *data;

void user_init(void)
{
    data = (ws2816b_data *)malloc(sizeof(ws2816b_data));
    WS2812_send(color_black, 60);
    HAL_Delay(SPEED);
    printf("WS2812B_TEST\r\n");
    HAL_UART_Receive_IT(&huart1,Data,1);
    
}
void user_main(void)
{
    
    
    switch(LED_MODE)
    {
    case 0x00:
        WS2812_send2(RGB_color, LED_NUM);
    break;

    case 1:WS2812_Wave_light(RGB_color,LED_NUM);break;
    case 2:WS2812_Breathing_light(RGB_color,LED_NUM);break;
    case 3:WS2812_Double_Color_Multistage_Waterfall_light(RGB_color,color_white,LED_NUM);break;

    case 4:WS2812_Rainbow_Flow(LED_NUM);break;
    case 5:WS2812_RGB_GradualChange_Waterfall(LED_NUM);break;
         
    case 6:WS2812_Colorful_Jump_light(color,sizeof(color)/sizeof(uint8_t *),LED_NUM);break;

    case 7:WS2812_Colourful_Waterfall_light(color,sizeof(color)/sizeof(uint8_t *),LED_NUM);break;
    } 
}









#include "WS2812B.h"

uint16_t LED_BYTE_Buffer[BUFF_SIZE];


/* ���������߱� */
const uint16_t index_breathing[] = {0, 0, 0, 0, 0, 0, 0, 0,8,8,8,8,8, 9,9,9,9,9, 10, 10, 10,10,10, 11,11, 11, 12, 12, 
                               13, 13, 14, 14, 15, 15, 16, 16, 17, 18, 18, 19, 20, 20, 21, 22, 23, 24, 25, 25, 26, 27, 28, 30, 31, 32, 33, 
                               34, 36, 37, 38, 40, 41, 43, 45, 46, 48, 50, 52, 54, 56, 58, 60, 62, 65, 67, 70, 72, 75, 78, 81, 84, 87, 90, 
                               94, 97, 101, 105, 109, 113, 117, 122, 126, 131, 136, 141, 146, 152, 158, 164, 170, 176, 183, 190, 197, 205, 
                               213, 221, 229, 238, 247, 256, 256, 247, 238, 229, 221, 213, 205, 197, 190, 183, 176, 170, 164, 158, 152, 146, 
                               141, 136, 131, 126, 122, 117, 113, 109, 105, 101, 97, 94, 90, 87, 84, 81, 78, 75, 72, 70, 67, 65, 62, 60, 58, 
                               56, 54, 52, 50, 48, 46, 45, 43, 41, 40, 38, 37, 36, 34, 33, 32, 31, 30, 28, 27, 26, 25, 25, 24, 23, 22, 21, 20, 
                               20, 19, 18, 18, 17, 16, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 11, 10, 10, 10,10,10, 9,9,9,9,9,8,8,8,8,8, 0, 0, 0, 0, 0, 0, 0, 0};

/* ���˵����߱� */
const uint16_t index_wave[] =  {10,30,50,100,190,256,190,100,50,30,10};

void delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
	{
		;
	}
}




static float min(float r, float g, float b)
{
  float m;
  
  m = r < g ? r : g;
  return (m < b ? m : b); 
}

static float max(float r, float g, float b)
{
  float m;
  
  m = r > g ? r : g;
  return (m > b ? m : b); 
}

void rgb2hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v)
{
  float red, green ,blue;
  float cmax, cmin, delta;
  
  red = (float)r/255;
  green = (float)g/255 ;
  blue = (float)b/255;
  
  cmax = max(red, green, blue);
  cmin = min(red, green, blue);
  delta = cmax - cmin;

  /* H */
  if(delta == 0)
  {
    *h = 0;
  }
  else
  {
    if(cmax == red)
    {
      if(green >= blue)
      {
        *h = 60 * ((green - blue) / delta);
      }
      else
      {
        *h = 60 * ((green - blue) / delta) + 360;
      }
    }
    else if(cmax == green)
    {
      *h = 60 * ((blue - red) / delta + 2);
    }
    else if(cmax == blue) 
    {
      *h = 60 * ((red - green) / delta + 4);
    }
  }
  
  /* S */
  if(cmax == 0)
  {
    *s = 0;
  }
  else
  {
    *s = delta / cmax;
  }
  
  /* V */
  *v = cmax;
}

void hsv2rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
{
    int hi = ((int)h / 60) % 6;
    float f = h / 60 - hi;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1- (1 - f) * s);
    
    switch (hi){
        case 0:
            *r =  255 * v;
            *g =  255 * t;
            *b =  255 * p;
            break;
        case 1:
            *r =  255 * q;
            *g =  255 * v;
            *b =  255 * p;
            break;
        case 2:
            *r =  255 * p;
            *g =  255 * v;
            *b =  255 * t;
            break;
        case 3:
            *r =  255 * p;
            *g =  255 * q;
            *b =  255 * v;
            break;
        case 4:
            *r =  255 * t;
            *g =  255 * p;
            *b =  255 * v;
            break;
        case 5:
            *r =  255 * v;
            *g =  255 * p;
            *b =  255 * q;
            break;
    }
}

void WS281x_Show(uint16_t send_len)
{
#if MODE_PWM_DMA
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_4, (uint32_t *)&LED_BYTE_Buffer, send_len); 
#elif MODE_SPI 
    HAL_SPI_Transmit(&hspi3,(uint8_t *)&LED_BYTE_Buffer,send_len,1000);
#endif
}

//���Σ���Ҫ��ʾ��RGB���ݺ͵Ƶĸ���
//���ܣ���RGB����ת����WS2812��Ҫ��GRB���ݣ�������24bit��ʽ�����ÿ���Ƶ�����
void WS2812_send(uint8_t *rgb, uint16_t len)
{
	uint8_t i;
	uint32_t memaddr = 0;
	//uint32_t buffersize = 0;
	//buffersize = len * GRB_BIT;	// number of bytes needed is #LEDs * 24 bytes
    memset(LED_BYTE_Buffer, 0, BUFF_SIZE); 
    
    LED_BYTE_Buffer[0] = 0;
    LED_BYTE_Buffer[1] = 0;
    LED_BYTE_Buffer[2] = 0;
    memaddr += 3;
	while (len)
	{
        for(i=0; i<8; i++) // GREEN data
        {
            LED_BYTE_Buffer[memaddr] = ((rgb[GREEN_INDEX]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            memaddr++;
        }
        for(i=0; i<8; i++) // RED
        {
            LED_BYTE_Buffer[memaddr] = ((rgb[RED_INDEX]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            memaddr++;
        }
        for(i=0; i<8; i++) // BLUE
        {
            LED_BYTE_Buffer[memaddr] = ((rgb[BLUE_INDEX]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            memaddr++;
        }
        
        len--;
	}
	LED_BYTE_Buffer[memaddr] = 0;
    memaddr++;
    WS281x_Show(memaddr);
}


void WS2812_Waterfall_light(uint8_t *rgb,uint16_t len)
{
	uint32_t i;
    int n = 0;
	uint32_t memaddr = 0;
    uint16_t rgb_buffer[28];
    
    memset(LED_BYTE_Buffer, 0, BUFF_SIZE); 
    
    rgb_buffer[0] = 0;
    rgb_buffer[1] = 0;
    rgb_buffer[2] = 0;
    rgb_buffer[27] = 0;
    for(i = 0; i < 8; i++)
    {
        rgb_buffer[i+3] = ((rgb[GREEN_INDEX]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
        rgb_buffer[i+8+3] = ((rgb[RED_INDEX]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
        rgb_buffer[i+16+3] = ((rgb[BLUE_INDEX]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
    }
    
    while(n != (len+10))
    {
        if(n>=10)
        {
            for(i = 0; i < memaddr-28*9; i++)
            {
                switch(i%28)
                {
                    case 0:
                    case 1:
                    case 2:LED_BYTE_Buffer[i] = 0;break;
                    case 27:LED_BYTE_Buffer[i] = 0;break;
                    default:LED_BYTE_Buffer[i] = TIMING_ZERO;break;
                }  
            }
        }
        
        
        if(n < len)   
        {
            for(i = 0; i < 28; i++)
            {
                LED_BYTE_Buffer[memaddr] = rgb_buffer[i];
                memaddr++;
            }  
            WS281x_Show(memaddr);
        }
        else
        {
            memaddr += 28;
            WS281x_Show(BUFF_SIZE);
        }
        
        HAL_Delay(SPEED+50);     
        n++;
    }
}

void WS2812_Multistage_Waterfall_light(uint8_t *rgb,uint16_t len)
{
    uint32_t i,j,k;
    static int count = LED_NUM;
	uint32_t memaddr = 0;
    uint8_t n1 = 6;
    uint8_t n2 = 4;
    uint32_t index = 0;
    memset(LED_BYTE_Buffer, 0, BUFF_SIZE); 
    
	while (len)
	{
        LED_BYTE_Buffer[memaddr] = 0;
        LED_BYTE_Buffer[memaddr+1] = 0;
        LED_BYTE_Buffer[memaddr+2] = 0;
        memaddr += 3;
        for(i=0; i<8; i++) // GREEN data
        {
            LED_BYTE_Buffer[memaddr] = ((rgb[GREEN_INDEX]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            memaddr++;
        }
        for(i=0; i<8; i++) // RED
        {
            LED_BYTE_Buffer[memaddr] = ((rgb[RED_INDEX]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            memaddr++;
        }
        for(i=0; i<8; i++) // BLUE
        {
            LED_BYTE_Buffer[memaddr] = ((rgb[BLUE_INDEX]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            memaddr++;
        }
        LED_BYTE_Buffer[memaddr] = 0;
        memaddr++;
        len--;
	}
	
    
    
    for(k = 0; k < n1; k++)
    {
        for(i = 0+count; i < n2+count; i++)
        {
            for(j = 0; j < 28; j++)
            {
                if((j+i*28+k*28*10) >= BUFF_SIZE)
                {
                    index = (j+i*28+k*28*10)%BUFF_SIZE;
                }
                else 
                {
                    index = (j+i*28+k*28*10);
                }
                switch(j%28)
                {
                    case 0:
                    case 1:
                    case 2:LED_BYTE_Buffer[index] = 0;break;
                    case 27:LED_BYTE_Buffer[index] = 0;break;
                    default:LED_BYTE_Buffer[index] = TIMING_ZERO;break;
                }  
            }
        }
    }
    count--;
    if(count < 0)
        count = LED_NUM;
    WS281x_Show(BUFF_SIZE);
    HAL_Delay(SPEED+100); 
}

void WS2812_Double_Color_Multistage_Waterfall_light(uint8_t *rgb1,uint8_t *rgb2,uint16_t len)//˫ɫ�����ˮ��...���� 
{
   uint32_t i,j,k;
    static int count = 0;
	uint32_t memaddr = 0;
    uint8_t n1 = 6;
    uint8_t n2 = 4;
    uint32_t index = 0;
    memset(LED_BYTE_Buffer, 0, BUFF_SIZE); 
    
	while (len)
	{
        LED_BYTE_Buffer[memaddr] = 0;
        LED_BYTE_Buffer[memaddr+1] = 0;
        LED_BYTE_Buffer[memaddr+2] = 0;
        memaddr += 3;
        for(i=0; i<8; i++) // GREEN data
        {
            LED_BYTE_Buffer[memaddr] = ((rgb1[GREEN_INDEX]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            memaddr++;
        }
        for(i=0; i<8; i++) // RED
        {
            LED_BYTE_Buffer[memaddr] = ((rgb1[RED_INDEX]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            memaddr++;
        }
        for(i=0; i<8; i++) // BLUE
        {
            LED_BYTE_Buffer[memaddr] = ((rgb1[BLUE_INDEX]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            memaddr++;
        }
        LED_BYTE_Buffer[memaddr] = 0;
        memaddr++;
        len--;
	}
	
    
    
    for(k = 0; k < n1; k++)
    {
        for(i = 0+count; i < n2+count; i++)
        {
            for(j = 0; j < 28; j++)
            {
                if((j+i*28+k*28*10) >= BUFF_SIZE)
                {
                    index = (j+i*28+k*28*10)%BUFF_SIZE;
                }
                else 
                {
                    index = (j+i*28+k*28*10);
                }
                switch(j%28)
                {
                    case 0:
                    case 1:
                    case 2:LED_BYTE_Buffer[index] = 0;break;
                    case 27:LED_BYTE_Buffer[index] = 0;break;
                    default:LED_BYTE_Buffer[index] = TIMING_ZERO;break;
                }  
            }
        }
    }
    
    memaddr = 0;
    for(i = 0; i < n1*2; i++)
    {
        if((count+i*5+28*4) > LED_NUM)
        {
            index = (count+i*5+28*3)%LED_NUM;
        }
        else
        {
            index = (count+i*5+28*3);
        }
        
        for(j = 0; j < 28; j++)
        {
            LED_BYTE_Buffer[index*28] = 0;
            LED_BYTE_Buffer[index*28+1] = 0;
            LED_BYTE_Buffer[index*28+2] = 0;
            for(k = 0; k < 8; k++)
            {
                LED_BYTE_Buffer[index*28+k+3] = ((rgb2[GREEN_INDEX]<<k) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
                LED_BYTE_Buffer[index*28+k+8+3] = ((rgb2[RED_INDEX]<<k) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
                LED_BYTE_Buffer[index*28+k+16+3] = ((rgb2[BLUE_INDEX]<<k) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            }
        }
    }
    
    count++;
    if(count > LED_NUM)
        count = 0;
    WS281x_Show(BUFF_SIZE);
    HAL_Delay(SPEED+100); 
}


void WS2812_Colourful_Waterfall_light(uint8_t *rgb[],uint8_t color_num,uint16_t len)
{
    int i,j,k;
    int n1,n2;
    static uint32_t memaddr = 0;
    memset(LED_BYTE_Buffer, 0, BUFF_SIZE); 
    
    n1 = (int)len/(int)color_num;
    n2 = n1 + (int)len%(int)color_num;
    
    for(i = 0; i < color_num-1; i++)
    {
        for(k = 0; k < n1; k++)
        {
            if(memaddr >= BUFF_SIZE)
                memaddr = 0;
            LED_BYTE_Buffer[memaddr]=0;
            LED_BYTE_Buffer[memaddr+1]=0;
            LED_BYTE_Buffer[memaddr+2]=0;
            memaddr += 3;
            for(j = 0; j < 8; j++)
            {
                LED_BYTE_Buffer[memaddr] = ((rgb[i][GREEN_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
                LED_BYTE_Buffer[memaddr+8] = ((rgb[i][RED_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
                LED_BYTE_Buffer[memaddr+16] = ((rgb[i][BLUE_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
                memaddr++;
            }
            memaddr = memaddr+16;
            LED_BYTE_Buffer[memaddr]=0;
            memaddr++;
            
        }
    }
    
    for(k = 0; k < n2; k++)
    {
        if(memaddr >= BUFF_SIZE)
            memaddr = 0;
        LED_BYTE_Buffer[memaddr]=0;
        LED_BYTE_Buffer[memaddr+1]=0;
        LED_BYTE_Buffer[memaddr+2]=0;
        memaddr += 3;
        for(j = 0; j < 8; j++)
        {
            LED_BYTE_Buffer[memaddr] = ((rgb[i][GREEN_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            LED_BYTE_Buffer[memaddr+8] = ((rgb[i][RED_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            LED_BYTE_Buffer[memaddr+16] = ((rgb[i][BLUE_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            memaddr++;
        }
        memaddr = memaddr+16;
        LED_BYTE_Buffer[memaddr]=0;
        memaddr++;
        
    }
     
    WS281x_Show(BUFF_SIZE);
    if(memaddr >= BUFF_SIZE)
        memaddr = 0;
    memaddr += 28;
    HAL_Delay(SPEED+20);
}

void WS2812_Colorful_Jump_light(uint8_t *rgb[],uint8_t color_num,uint16_t len)
{
    int i,j;
    static int color_index = 0;
    static uint8_t count = 0;
    
    memset(LED_BYTE_Buffer, 0, BUFF_SIZE);
    
    for(i = count; i < len; i++)
    {
        if(i%2 == 0)
        {
            color_index = color_index%color_num;         
            LED_BYTE_Buffer[i*28+0] = 0;
            LED_BYTE_Buffer[i*28+1] = 0;
            LED_BYTE_Buffer[i*28+2] = 0;
            for(j = 0; j < 8; j++)
            {
                LED_BYTE_Buffer[i*28+j+3] = ((rgb[color_index][GREEN_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
                LED_BYTE_Buffer[i*28+j+8+3] = ((rgb[color_index][RED_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
                LED_BYTE_Buffer[i*28+j+16+3] = ((rgb[color_index][BLUE_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            }
            LED_BYTE_Buffer[i*28+27] = 0;
            color_index++;
        }
        else
        {
            for(j = 0; j < 28; j++)
            {
                switch(j%28)
                {
                    case 0:
                    case 1:
                    case 2:LED_BYTE_Buffer[i*28+j] = 0;break;
                    case 27:LED_BYTE_Buffer[i*28+j] = 0;break;
                    default:LED_BYTE_Buffer[i*28+j] = TIMING_ZERO;break;
                }  
            }
        }
    }
    count++;
    count = count%2;
    
   
    WS281x_Show(BUFF_SIZE);
    HAL_Delay(SPEED+200);
}


void WS2812_Breathing_light(uint8_t *rgb,uint16_t len)
{
    float h,s,v;
    uint8_t r,g,b;
    uint8_t nrgb[3] = {0,0,0};
    static int i = 0;
    rgb2hsv(rgb[RED_INDEX],rgb[GREEN_INDEX],rgb[BLUE_INDEX],&h,&s,&v);
    
        v = index_breathing[i]/256.0;
        
        hsv2rgb(h,s,v,&r,&g,&b);
        nrgb[RED_INDEX] = r;
        nrgb[GREEN_INDEX] = g;
        nrgb[BLUE_INDEX] = b;
        
        WS2812_send(nrgb,len);
        HAL_Delay(SPEED);  

    i++;
    if(i >= (sizeof(index_breathing)/sizeof(uint16_t)))
        i = 0;
        
    /*
    v = 0;
    while(v < 0.9)
    {
        nh = h;
        ns = s;
        nv = v;
     
        hsv2rgb(nh,ns,nv,&r,&g,&b);
        nrgb[RED_INDEX] = r;
        nrgb[GREEN_INDEX] = g;
        nrgb[BLUE_INDEX] = b;
        
        WS2812_send(nrgb,len);
        HAL_Delay(SPEED);
      
        
        v = v+0.01;
    }
   
    while(v > 0)
    {
        nh = h;
        ns = s;
        nv = v;
     
        hsv2rgb(nh,ns,nv,&r,&g,&b);
        nrgb[RED_INDEX] = r;
        nrgb[GREEN_INDEX] = g;
        nrgb[BLUE_INDEX] = b;
        
        WS2812_send(nrgb,len);
        HAL_Delay(SPEED);
        
        
        v = v-0.01;
    }
    */
}

void WS2812_Rainbow_Flow(uint16_t len)
{
    static uint8_t rgb[3] = {0,0,0};
    static uint8_t mode = 0;
    
    if((rgb[GREEN_INDEX] < 255) && !mode)
    {
        rgb[GREEN_INDEX]++;


    }
    else if((rgb[RED_INDEX] > 0) && !mode)
    {
        rgb[RED_INDEX]--;


    }
    else if((rgb[BLUE_INDEX] < 255) && !mode)
    {
        rgb[BLUE_INDEX]++;


    }
    else if((rgb[GREEN_INDEX] > 0) && mode)
    {
        rgb[GREEN_INDEX]--;
        

    }
    else if((rgb[RED_INDEX] < 255) && mode)
    {
        rgb[RED_INDEX]++;


    }
    else if((rgb[BLUE_INDEX] > 0) && mode)
    {
        rgb[BLUE_INDEX]--;
    } 
    
    if(rgb[BLUE_INDEX] == 255)
        mode = 1;
    if(rgb[BLUE_INDEX] == 0)
        mode = 0;
    
    
    WS2812_send(rgb,len);
    HAL_Delay(SPEED);
}

void WS2812_Wave_light(uint8_t *rgb,uint16_t len)
{
    int index = 0;
    static int count = 0;
    uint8_t nrgb[3] = {0,0,0};
    int i,j,k;
    int n1 = sizeof(index_wave)/sizeof(uint16_t);
    float h,s,v;
    uint8_t r,g,b;
    
    rgb2hsv(rgb[RED_INDEX],rgb[GREEN_INDEX],rgb[BLUE_INDEX],&h,&s,&v);
    
    for(i = 0; i < n1; i++)
    {
        v = index_wave[i]/256.0;
        hsv2rgb(h,s,v,&r,&g,&b);
        nrgb[RED_INDEX] = r;
        nrgb[GREEN_INDEX] = g;
        nrgb[BLUE_INDEX] = b;
        
        for(k = 0; k < len/n1; k++)
        {          
            index = (k*n1*28+i*28+count*28)%(LED_NUM*28);           
            LED_BYTE_Buffer[index] = 0;
            LED_BYTE_Buffer[index+1] = 0;
            LED_BYTE_Buffer[index+2] = 0;
            for(j = 0; j < 8; j++)
            {
                LED_BYTE_Buffer[index+j+3] = ((nrgb[GREEN_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
                LED_BYTE_Buffer[index+j+3+8] = ((nrgb[RED_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
                LED_BYTE_Buffer[index+j+3+16] = ((nrgb[BLUE_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            }
            LED_BYTE_Buffer[index+27] = 0; 
        }
    }
    /*
    for(i = 0; i < len%n1; i++)
    {
        LED_BYTE_Buffer[(len/n1)*28*n1+i*28] = 0;
        LED_BYTE_Buffer[(len/n1)*28*n1+i*28+1] = 0;
        LED_BYTE_Buffer[(len/n1)*28*n1+i*28+2] = 0;
        for(j = 0; j < 8; j++)
        {
            LED_BYTE_Buffer[(len/n1)*28*n1+i*28+j+3] = ((nrgb[GREEN_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            LED_BYTE_Buffer[(len/n1)*28*n1+i*28+j+3+8] = ((nrgb[RED_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            LED_BYTE_Buffer[(len/n1)*28*n1+i*28+j+3+16] = ((nrgb[BLUE_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
        }
        LED_BYTE_Buffer[(len/n1)*28*n1+i*28+27] = 0; 
    }
    */
    count++;
    if(count > LED_NUM)
        count = 0;
   
    WS281x_Show(BUFF_SIZE);
    HAL_Delay(SPEED+50);
}

void WS2812_RGB_GradualChange_Waterfall(uint16_t len)
{
    static uint16_t r=100,g=100,b=100;
    uint16_t rgb[3] = {0,0,0};
    static uint8_t mode_bak = 0;
    uint8_t mode;
    
    int i;
    
    mode = mode_bak;
    rgb[RED_INDEX] = r;
    rgb[GREEN_INDEX] = g;
    rgb[BLUE_INDEX] = b;
    
    for(i = 0; i < len; i++)
    {
        
        
        LED_BYTE_Buffer[i*28+0] = 0;
        LED_BYTE_Buffer[i*28+1] = 0;
        LED_BYTE_Buffer[i*28+2] = 0;
        for(int j = 0; j < 8; j++)
        {
            LED_BYTE_Buffer[i*28+j+3] = ((rgb[GREEN_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            LED_BYTE_Buffer[i*28+j+3+8] = ((rgb[RED_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
            LED_BYTE_Buffer[i*28+j+3+16] = ((rgb[BLUE_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
        }
        LED_BYTE_Buffer[i*28+27] = 0;
        
        if((rgb[GREEN_INDEX] < 245) && (!mode))
        {
            rgb[GREEN_INDEX] += 10;
        }
        else if((rgb[RED_INDEX] > 10) && (!mode))
        {
            rgb[RED_INDEX] -= 10;
        }
        else if((rgb[BLUE_INDEX] < 245) && (!mode))
        {
            rgb[BLUE_INDEX] += 10;
        }
        else if((rgb[GREEN_INDEX] > 10) && mode)
        {
            rgb[GREEN_INDEX] -= 10;
        }
        else if((rgb[RED_INDEX] < 245) && mode)
        {
            rgb[RED_INDEX] += 10;
        }
        else if((rgb[BLUE_INDEX] > 10) && mode)
        {
            rgb[BLUE_INDEX] -= 10;
            
        } 
        if(rgb[BLUE_INDEX] >= 245)
            mode = 1;
        if(rgb[BLUE_INDEX] <= 10)
            mode = 0;
        
        if(i == 0)
        {
            mode_bak = mode;
            r = rgb[RED_INDEX];
            g = rgb[GREEN_INDEX];
            b = rgb[BLUE_INDEX];
            
        }
    }
    
    WS281x_Show(BUFF_SIZE);
    HAL_Delay(SPEED);
    //while(1);
}
#include "GPIO_Init.h"
#include "platform_config.h"

Type_Input_Port Input_Port ;
Type_Output_Port Output_Port ;
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
***************************************************************************//****/
void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|
                            RCC_APB2Periph_GPIOB|
                            RCC_APB2Periph_GPIOC|
                            RCC_APB2Periph_GPIOD|
                            RCC_APB2Periph_GPIOE|
                            RCC_APB2Periph_GPIOF|
                            RCC_APB2Periph_GPIOG|
                            RCC_APB2Periph_AFIO, ENABLE);		//开时钟
	
    //LED														 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;									
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//时钟速度为50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//端口模式为推拉输出方式	
    GPIO_Init(GPIOA, &GPIO_InitStructure);			
    
		//input ID SET
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3 |
																	GPIO_Pin_4 ;
																	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
    GPIO_Init(GPIOG, &GPIO_InitStructure);	
		
		//input
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3 |
																	GPIO_Pin_4 |GPIO_Pin_5 |
																	GPIO_Pin_6 ;
									
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//端口模式为上拉输入方式	
		GPIO_Init(GPIOE, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |
																	GPIO_Pin_2 |GPIO_Pin_3 |
																	GPIO_Pin_4 |GPIO_Pin_5 |
																	GPIO_Pin_6 ;
							
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//端口模式为上拉输入方式	
		GPIO_Init(GPIOF, &GPIO_InitStructure);

    //Output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8;																	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//时钟速度为50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//端口模式为推拉输出方式	
    GPIO_Init(GPIOG, &GPIO_InitStructure);	
		GPIO_SetBits(GPIOG, GPIO_Pin_5 |GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8);

		//24V Power control
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 |GPIO_Pin_11;        
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//时钟速度为50M		
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
    GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		//PLUSE
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 ;																	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//时钟速度为50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
    GPIO_Init(GPIOC, &GPIO_InitStructure);	
		//DIR
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15 ;																	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//时钟速度为50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
    GPIO_Init(GPIOD, &GPIO_InitStructure);		
		//ENABLE
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 ;																	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//时钟速度为50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
    GPIO_Init(GPIOG, &GPIO_InitStructure);			
		//IC ENABLE
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1;																	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//时钟速度为50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
    GPIO_Init(GPIOE, &GPIO_InitStructure);	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14;																	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//时钟速度为50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
    GPIO_Init(GPIOG, &GPIO_InitStructure);	

		//AXIS 1&2 pen1 H  PE1
		//AXIS 1&2 pen2 H  PE0
		//AXIS 3&4 pen3 H  PG13
		//AXIS 3&4 pen4 H  PG14

		GPIO_SetBits(GPIOE, GPIO_Pin_0|GPIO_Pin_1); 
		GPIO_SetBits(GPIOG, GPIO_Pin_13|GPIO_Pin_14); 
}



///*******************************************************************************
//* Function Name  :
//* Description    : 控制外部输出
//********************************************************************************/

void OutputControl(unsigned int stats)
{
    Output_Port.ALL=stats;

    if(Output_Port.ONE.Y_1==1)
    { Y1_ON; }
    else
    { Y1_OFF;}

    if(Output_Port.ONE.Y_2==1)
    { Y2_ON; }
    else
    { Y2_OFF;}

    if(Output_Port.ONE.Y_3==1)
    { Y3_ON; }
    else
    { Y3_OFF;}
    if(Output_Port.ONE.Y_4==1)
    { Y4_ON; }
    else
    { Y4_OFF;}

    if(Output_Port.ONE.Y_5==1)
    { Y5_ON; }
    else
    { Y5_OFF;}

    if(Output_Port.ONE.Y_6==1)
    { Y6_ON; }
    else
    { Y6_OFF;}
     if(Output_Port.ONE.Y_7==1)
    { Y7_ON; }
    else
    { Y7_OFF;}

    if(Output_Port.ONE.Y_8==1)
    { Y8_ON; }
    else
    { Y8_OFF;}
}

/*******************************************************************************
* Function Name  :
* Description    : 读取输入状态 0L 1H
********************************************************************************/
unsigned int InputRead(void)
{
    Input_Port.ALL=0;           //清空

		if(IN_X1==1)
		{ Input_Port.ONE.X_1=1; }
		else
		{ Input_Port.ONE.X_1=0; }

		if(IN_X2==1)
		{ Input_Port.ONE.X_2=1; }
		else
		{ Input_Port.ONE.X_2=0; }

		if(IN_X3==1)
		{ Input_Port.ONE.X_3=1; }
		else
		{ Input_Port.ONE.X_3=0; }

		if(IN_X4==1)
		{ Input_Port.ONE.X_4=1; }
		else
		{ Input_Port.ONE.X_4=0; }
		
		if(IN_X5==1)
		{ Input_Port.ONE.X_5=1; }
		else
		{ Input_Port.ONE.X_5=0; }

		if(IN_X6==1)
		{ Input_Port.ONE.X_6=1; }
		else
		{ Input_Port.ONE.X_6=0; }

		if(IN_X7==1)
		{ Input_Port.ONE.X_7=1; }
		else
		{ Input_Port.ONE.X_7=0; }

		if(IN_X8==1)
		{ Input_Port.ONE.X_8=1; }
		else
		{ Input_Port.ONE.X_8=0; }

   return Input_Port.ALL;
}
   /*******************************************************************************
* Function Name  :
* Description    :读取输出状态
********************************************************************************/
unsigned int OutputRead(void)
{

        if(OUT_Y1==1)
        { Output_Port.ONE.Y_1=1; }
        else
        { Output_Port.ONE.Y_1=0; }

        if(OUT_Y2==1)
        { Output_Port.ONE.Y_2=1; }
        else
        { Output_Port.ONE.Y_2=0; }

        if(OUT_Y3==1)
        { Output_Port.ONE.Y_3=1; }
        else
        { Output_Port.ONE.Y_3=0; }

        if(OUT_Y4==1)
        { Output_Port.ONE.Y_4=1; }
        else
        { Output_Port.ONE.Y_4=0; }

        if(OUT_Y5==1)
        { Output_Port.ONE.Y_5=1; }
        else
        { Output_Port.ONE.Y_5=0; }

        if(OUT_Y6==1)
        { Output_Port.ONE.Y_6=1; }
        else
        { Output_Port.ONE.Y_6=0; }

        if(OUT_Y7==1)
        { Output_Port.ONE.Y_7=1; }
        else
        { Output_Port.ONE.Y_7=0; }

        if(OUT_Y8==1)
        { Output_Port.ONE.Y_8=1; }
        else
        { Output_Port.ONE.Y_8=0; }

    return ~Output_Port.ALL;
}

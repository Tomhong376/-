//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//Project: TM8C
//Author : CCR
//Date   : 2014-01-08
//================================================================================

//================================================================================

#include	"define.h"

//-------------------------变量定义-------------------------//
.ramadr 0								//地址定义为0
word system_ram;

//-------------------------客制变量-------------------------//
.ramadr system							//以后变量地址由系统自行决定

byte r_main_flag;
bit flag_FPPA0			:r_main_flag.0
bit flag_poweron		:r_main_flag.1


byte main_count;

//-----------------------------------------------------------//
//-------------------------FPPA0程序-------------------------//
//-----------------------------------------------------------//
void	FPPA0 (void)
{
	.ADJUST_IC SYSCLK=IHRC/8, IHRC=16MHz, VDD=3.7V//, Bandgap=on
	.wdreset;
	clkmd.En_WatchDog = 1;				//看门狗使能 WatchDog Enable
	
	//------------------------初始化------------------------//
	i_o_set
	clr_ram	
	ad_set
	timer_set
	//----------------------上电执行段----------------------//
	led_blink_count = blink_poweron*2+1;//上电闪三下
	led_blink_half_circle = 50;
	flag_led_r = 1;
	flag_led_g = 1;
	flag_led_b = 1;
//	flag_poweron_blink = 1;
	led_blink_delay = 30;
//	flag_led_wbr = 1;
//	flag_poweron = 1;

	//------------------------主循环------------------------//
	while(1)
	{
		.wdreset;
		if(flag_FPPA0)
		{
			flag_FPPA0 = 0;
			system_deal();
			pwm_output();
			led_output();
		}
		else
		{
			ad_get();
		}
		if(flag_sleep)
		{
			flag_sleep = 0;
			sleep_set;
//			main_count = 0;
		}
	}
}



//-----------------------------------------------------------//
//-------------------------FPPA1程序-------------------------//
//-----------------------------------------------------------//
//void	FPPA1 (void)
{
//	while(1)
	{
//		ad_get_deal();
	}
}
//-----------------------------------------------------------//
//-----------------------中断服务程序------------------------//
//-----------------------------------------------------------//
void	Interrupt (void)
{
	pushaf;
//	p_led_r = 0;
	if (Intrq.T16)
	{	
		Intrq.T16	=	0;
		stt16 system_ram;
		led_count ++;
		if(led_count <= led_r_duty)
		{
			p_led_r_on;
		}
		else
		{
			p_led_r_off;
		}
		if(led_count <= led_g_duty)
		{
			p_led_g_on;
		}
		else
		{
			p_led_g_off;
		}
		if(led_count <= led_b_duty)
		{
			p_led_b_on;
		}
		else
		{
			p_led_b_off;
		}
		if(led_count >= led_duty_max)
		{
			led_count = 0;
		}
		pwm_count ++;
		if(pwm_count <= mos_duty)
		{
			p_mos = 0;
			p_mos_c = 1;
			p_mos = 0;
			flag_mos = 1;
		}
		else
		{
			p_mos_c = 0;
			flag_mos = 0;
		}
		if(pwm_count >= mos_duty_max)
		{
			pwm_count = 0;
		}
		main_count ++;
		if(main_count >= 50)
		{
			main_count = 0;
			flag_FPPA0 = 1;
		}
	}
	popaf;
}

//-----------------------------------------------------------//
//-------------------------子程序区--------------------------//
//-----------------------------------------------------------//

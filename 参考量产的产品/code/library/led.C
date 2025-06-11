

#include	"../define.h"

//#define blink_keep_time		45//此处调整闪烁周期，T = blink_keep_time*5ms*2
#define degree_delay_time	250

/*
#define degree_max			3

#define degree_data4		415+5
#define degree_data3		380+5
#define degree_data2		350+5
#define degree_data1		320+5
*/
//#define v_charge_off		420



byte led_blink_count;
byte led_blink_delay;
byte led_blink_half_circle;
byte led_duty,led_r_duty,led_g_duty,led_b_duty;
byte led_count,led_delay_count,led_turn;
byte led_duty_delay;


byte duty_change_delay;
byte led_delay;


byte r_led_flag;
bit flag_blink_force	:r_led_flag.0;
bit flag_led			:r_led_flag.1;
bit flag_led_w			:r_led_flag.2;
bit flag_led_b			:r_led_flag.3;
bit flag_led_r			:r_led_flag.4;
bit flag_led_wbr		:r_led_flag.5;
bit flag_led_gb			:r_led_flag.6;
bit flag_led_rb			:r_led_flag.7;

byte r_led_flag1;
bit flag_led_rg			:r_led_flag1.0;
bit flag_led_g			:r_led_flag1.1;
bit flag_poweron_blink	:r_led_flag1.2;


void reset_show_vdd();
void preheat_slow_change(void);
void preheat_blink(void);

//-------------------------------------//
//---------------亮灯------------------//
//-------------------------------------//
void led_on(void)
{
	flag_led = 1;
//	p_led_on;
}
//-------------------------------------//
//---------------灭灯------------------//
//-------------------------------------//
void led_off(void)
{
	flag_led = 0;
//	p_led_off;
}

//-------------------------------------//
//---------------灯输出----------------//
//-------------------------------------//
void led_output(void)
{
	if(led_blink_count)
	{
		if(led_blink_count.0)
		{
			led_duty = 0;
//			p_led_off;
		}
		else
		{
			led_duty = led_duty_max;
//			p_led_on;
		}
		led_blink_delay ++;
		if(led_blink_delay >= led_blink_half_circle)
		{
			led_blink_delay = 0;
			led_blink_count --;
			if(led_blink_count == 0)
			{
				flag_blink_force = 0;	//解除强制闪烁
				led_blink_half_circle = 50;
				flag_led = 0;
				clear_led_select();
				flag_poweron_blink = 0;
			}
		}
	}
	else
	{
		led_blink_delay = 0;
		if(!flag_pre_heat)
		{
//			led_delay ++;
//			if(led_delay.0)
			{
				if(flag_led)
				{
					led_duty = led_duty_max;
//					if(led_duty < led_duty_max)
					{
//						led_duty ++;
					}
				}
				else
				{
					led_duty = 0;
//					if(led_duty)
					{
//						led_duty --;
					}
				}
			}
			led_delay_count = 0;
			led_turn = 0;
		}
		else
		{
			preheat_blink();
//			preheat_slow_change();

		}
	}

	if(led_duty)
	{
		if(flag_led_r)
		{
			led_r_duty = led_duty;
		}
		else
		{
			led_r_duty = 0;
		}
		if(flag_led_g)
		{
			led_g_duty = led_duty;
		}
		else
		{
			led_g_duty = 0;
		}
		if(flag_led_b)
		{
			led_b_duty = led_duty;
		}
		else
		{
			led_b_duty = 0;
		}
/*		if(flag_led_r)
		{
			led_r_duty = led_duty >> 1;
			led_g_duty = 0;
			led_b_duty = 0;
		}
		else if(flag_led_g)
		{
			led_r_duty = 0;
			led_g_duty = led_duty >> 1;
			led_b_duty = 0;
		}
		else if(flag_led_b)
		{
			led_r_duty = 0;
			led_g_duty = 0;
			led_b_duty = led_duty >> 1;
		}
		else if(flag_led_gb)
		{
			a = led_duty >> 1;
			led_r_duty = 0;
			led_b_duty = a;
			led_g_duty = a;
		}
		else if(flag_led_rb)
		{
			a = led_duty >> 1;
			led_r_duty = a;
			led_b_duty = a;
			led_g_duty = 0;
		}
		else if(flag_led_rg)
		{
			a = led_duty >> 1;
			led_r_duty = a;
			led_b_duty = 0;
			led_g_duty = a;
		}
		else if(flag_led_w)
		{
			a = led_duty >> 2;
			led_r_duty = a;
			led_b_duty = a;
			led_g_duty = a;
		}
		else if(flag_led_wbr)
		{
			a = led_duty >> 2;
			led_r_duty = a;
			led_b_duty = a;
			led_g_duty = a >> 1;
		}
*/	}
	else
	{
		led_r_duty = 0;
		led_g_duty = 0;
		led_b_duty = 0;
	}
}




void preheat_blink(void)
{
	led_duty = led_duty_max;
	led_delay_count ++;
	if(led_delay_count >= 40)
	{
		led_delay_count = 0;
		led_turn ++;
//		if(led_turn >= 3)
		{
//			led_turn = 0;
		}
		if(led_turn.0)
		{
			flag_led_g = 1;
			flag_led_b = 1;
		}
		else
		{
			clear_led_select();
		}
	}
/*	if(led_turn)
	{
		if(led_turn == 1)
		{
			flag_led_w = 0;
			flag_led_b = 1;
			flag_led_r = 0;
		}
		else 
		{
			flag_led_w = 1;
			flag_led_b = 0;
			flag_led_r = 0;
		}
	}
	else
	{
		flag_led_w = 0;
		flag_led_b = 0;
		flag_led_r = 1;
	}
*/
}

void preheat_slow_change(void)
{
	led_delay_count ++;
	if((led_delay_count != 1) && (led_delay_count != 3))//这段是预热渐变代码//1.5*5ms duty++
	{
		return;
	}
	if(led_delay_count == 3)
	{
		led_delay_count = 0;
	}
	if(led_turn.0)
	{
		if(led_duty)
		{
			led_duty --;
		}
		else
		{
			led_turn ++;
		}
	}
	else
	{
		if(led_duty < (led_duty_max+4))
		{
			led_duty ++;
		}
		else
		{
			led_turn ++;
		}
	}
/*	if((led_duty == 0) || (led_duty == 1))
	{
		switch(led_turn)
		{
		case 0:
		case 1:
			flag_led_r = 1;
			flag_led_rg = 0;
			flag_led_g = 0;
			flag_led_b = 0;
			flag_led_gb = 0;
			flag_led_rb = 0;
			break;
		case 2:
		case 3:
			flag_led_r = 0;
			flag_led_rg = 1;
			flag_led_g = 0;
			flag_led_b = 0;
			flag_led_gb = 0;
			flag_led_rb = 0;
			break;
		case 4:
		case 5:
			flag_led_r = 0;
			flag_led_rg = 0;
			flag_led_g = 1;
			flag_led_b = 0;
			flag_led_gb = 0;
			flag_led_rb = 0;
			break;
		case 6:
		case 7:
			flag_led_r = 0;
			flag_led_rg = 0;
			flag_led_g = 0;
			flag_led_b = 1;
			flag_led_gb = 0;
			flag_led_rb = 0;
			break;
		case 8:
		case 9:
			flag_led_r = 0;
			flag_led_rg = 0;
			flag_led_g = 0;
			flag_led_b = 0;
			flag_led_gb = 1;
			flag_led_rb = 0;
			break;
		case 10:
		case 11:
			flag_led_r = 0;
			flag_led_rg = 0;
			flag_led_g = 0;
			flag_led_b = 0;
			flag_led_gb = 0;
			flag_led_rb = 1;
			break;
		default:
			led_turn = 0;
		}
	}
	*/
}

void clear_led_select(void)
{
	flag_led_r = 0;
	flag_led_rg = 0;
	flag_led_g = 0;
	flag_led_b = 0;
	flag_led_gb = 0;
	flag_led_rb = 0;
	flag_led_w = 0;
	flag_led_wbr = 0;
}

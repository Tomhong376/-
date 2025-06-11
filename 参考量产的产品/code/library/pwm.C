
#include "../define.h"


byte mos_duty;
byte pwm_count;

byte r_flag_pwm;
bit flag_pwm_deal	:r_flag_pwm.0


void keep3_7v_output(void);
/***************************************/
/****************pwm输出****************/
/***************************************/
void pwm_output(void)
{
	if(flag_smoking || flag_pre_heat)
	{
		if(flag_pwm_deal)
		{
			flag_pwm_deal = 0;
			if(flag_pre_heat)
			{
				if(V_out >= 182)
				{
					math_eword = mos_duty_max*180;
					math_word = V_out;
					Math_Eword_Div_Word();
					mos_duty = math_eword$0;
				}
				else
				{
					mos_duty = mos_duty_max;
				}
			}
			elseif(v_out_degree)
			{
				if(v_out_degree == 1)//第1档
				{
					if(V_out >= 362)
					{
						math_eword = mos_duty_max*362;
						math_word = V_out;
						Math_Eword_Div_Word();
						mos_duty = math_eword$0;
					}
					else
					{
						mos_duty = mos_duty_max;
					}
				}
				else //第2档
				{
					if(V_out >= 272)
					{
						math_eword = mos_duty_max*272;
						math_word = V_out;
						Math_Eword_Div_Word();
						mos_duty = math_eword$0;
					}
					else
					{
						mos_duty = mos_duty_max;
					}
				}
			}
			else//默认档位
			{
				if(V_out >= 312)
				{
					math_eword = mos_duty_max*312;
					math_word = V_out;
					Math_Eword_Div_Word();
					mos_duty = math_eword$0;
				}
				else
				{
					mos_duty = mos_duty_max;
				}
			}
		}
	}
	else if(!flag_charge)
	{
		mos_duty = 0;
		mos_off();
//		flag_pwm_deal = 0;
	}
}
/***************************************/
/****************pwm输出****************/
/*************************************** /
void pwm_output(void)
{
	if(led_pwm_value == 0)
	{
		tm2c = 0x00;		//关pwm
		p_pwm = 0;			//pwm脚输出低
	}
	else if(led_pwm_value_save != led_pwm_value)
	{
		if(led_pwm_value_save == 0)
		{
			tm2ct = 0b_0000_0000;//计数器清零
			tm2b  = led_pwm_value;
			tm2s  = 0b_0110_0000;
			tm2c  = 0b_0001_1010;	//开pwm			
		}
		else
		{
			tm2b = led_pwm_value;
		}
	}
	led_pwm_value_save = led_pwm_value;
}
/**/


/***************************************/
/*************恒压3.7V输出**************/
/*************************************** /
void keep3_7v_output(void)
{
	if(VDD > 370)
	{
		math_eword = 200*370;
		math_word = VDD;
		Math_Eword_Div_Word();
		mos_duty = math_eword$0;
	}
	else
	{
		mos_duty = mos_duty_max;//全功率输出
	}
}
*/
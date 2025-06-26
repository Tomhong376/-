

#include "../define.h"

#define ad_times			18
#define ad_abandon_times	4

#define V_overload			29//待测定，mos高温时的D和S压降，
#define V_lowload			6

byte ad_count, load_delay_count, load_low_count;
word ad_value, VDD, VDD_protect, V_out, VDD_smoking;
eword ad_sum, ad_sum_bandgap;
byte ad_turn, charge_remove_delay;

byte ad_charge_turn;


byte r_ad_flag;
bit flag_mos_pre		:r_ad_flag.0;
bit flag_ad				:r_ad_flag.1;
bit	flag_reset_overload	:r_ad_flag.2;
bit flag_discharge		:r_ad_flag.3;


void short_protect_deal(void);//短路保护处理
void short_io_protect(void);//短路保护监测


void ad_get(void)
{
	if(!AD_Start)
	{
		return;
	}

	ad_value = (adcrh << 8 | adcrl);
 	ad_value =  ad_value >> 4;  
	
	if(ad_count >= ad_abandon_times)
	{
		ad_sum += ad_value;//adcr;
	}

	AD_Start = 1;		//开始

	short_io_protect();//io短路保护


	if((flag_mos && (!flag_mos_pre)) || ((!flag_mos) && flag_mos_pre))
	{
		ad_reset();
		if(flag_mos_pre)
		{
			flag_mos_pre = 0;
		}
		else
		{
			flag_mos_pre = 1;
		}
		return;
	}

	ad_count ++;
	if(ad_count >= ad_times + ad_abandon_times)
	{
		ad_deal();
		ad_count = 0;
		ad_sum = 0;
	}
}



void ad_deal(void)
{
	if(ad_turn == 0)
	{
		if(flag_mos_pre)//采集过程mos状态为打开
		{
			p_check_ad_in();
			$ adcc enable, p_check_ad;
			ad_turn ++;
		}

		   	ad_sum_bandgap = ad_sum;
			math_eword = ad_sum;
			math_dword = 120*4096*ad_times;//1.2V,AD次数
			Math_Dword_Div_Eword();
			if(flag_mos_pre)
			{
				VDD_smoking = math_dword;//吸烟时的板上的VDD电压
			}
			else
			{
				VDD = math_dword;//采集不带载电池电压
			}
	}
	else if(ad_turn == 1)
	{
//	  $ adcc enable, ADC;
	  $ adcc enable, BANDGAP;//363
	 	p_check_io_in();

		math_eword = ad_sum;
		math_byte = 144;			//对应电阻值200+1K,120*12/10=144

		Math_Eword_Mul_Byte();
		math_eword = ad_sum_bandgap;

		Math_Dword_Div_Eword();

		V_out = math_dword;//得到实际输出电压//如果是恒压输出，就在这里取得输出电压，这里是全功率输出
		flag_pwm_deal = 1;
		ad_turn = 0;
	}
}


//-----------------------------------------//
//---------------AD重置处理----------------//
//-----------------------------------------//
void ad_reset(void)
{
	p_check_io_in();
	ad_turn = 0;
	ad_count = 0;
	ad_sum = 0;
//  $ adcc enable, ADC;
  $ adcc enable, BANDGAP; //363
	AD_Start = 1;
}


/******************************************/
//-------开mos就进行短路保护IO监测--------//
/******************************************/
void short_io_protect(void)
{	
	if(!flag_mos)
	{
		return;
	}
	if(ad_turn == 0)
	{
		p_check_out0();
		a = 2;
		while(a--)
		{
			if(!flag_mos)
			{
				p_check_io_in();
				return;
			}
		}
		p_check_io_in();
		nop;
		nop;
		nop;
		nop;
		a = 15;
		do
		{
			if(!flag_mos)
			{
				return;
			}
			if(p_check)
			{
				return;
			}
		}while(a --);
		mos_duty = 0;
		mos_off();			//紧急关闭输出
//		flag_overload = 1;
		flag_pre_heat = 0;
		flag_smoking = 0;
		flag_charge = 0;

		clear_led_select();
		flag_led_r = 1;
		flag_led_g = 1;
		flag_led_b = 1;
//		flag_poweron_blink = 1;
//		V_out_degree = 0;
		led_blink_count = blink_overload*2+1;//闪烁三次
		led_blink_half_circle = 50;
		led_blink_delay = 0;	//闪烁，需要清零闪烁计时
		flag_blink_force = 1;	//设为强制，闪烁过程清除连击状态
//		ad_reset();//那边会复位AD 
	}

}




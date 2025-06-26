

#include "../define.h"

#define ad_times 18
#define ad_abandon_times 4

#define V_overload 1000 // 待测定，mos高温时的D和S压降，
#define V_lowload 6

bit flag_temp_alarm;

byte ad_count, load_delay_count, load_low_count, ad_temp_count;
word ad_value, VDD, VDD_protect, V_out, VDD_smoking, ad_temp_value,V_temp;

eword ad_sum, ad_sum_bandgap, ad_sum_temprature, ad_sum_out, ad_temp_sum;

eword temp_ad_sum, temp_ad_sum_bandgap;
byte temp_ad_count;

byte ad_turn, charge_remove_delay;

byte ad_charge_turn;
byte adc_threshold;

byte r_ad_flag;
bit flag_mos_pre : r_ad_flag.0;
bit flag_ad : r_ad_flag.1;
bit flag_reset_overload : r_ad_flag.2;
bit flag_discharge : r_ad_flag.3;

void short_protect_deal(void); // 短路保护处理
void short_io_protect(void);   // 短路保护监测
void temp_alarm(void);		   // 温度报警测试
void temperature_check();

void ad_get(void)
{
	if (!AD_Start)
	{
		return;
	}

	cnt_ADC++;
	if (cnt_ADC >= 5)
	{
		temp_sample_flag = 1;
		cnt_ADC = 0;
	}


	ad_value = (adcrh << 8 | adcrl);
	ad_value = ad_value >> 4;

	if (ad_count >= ad_abandon_times)
	{
		ad_sum += ad_value; // adcr;
	}
                                                                            
	AD_Start = 1; // 开始

	short_io_protect(); // io短路保护

	if ((flag_mos && (!flag_mos_pre)) || ((!flag_mos) && flag_mos_pre))
	{
		ad_reset();
		if (flag_mos_pre)
		{
			flag_mos_pre = 0;
		}
		else
		{
			flag_mos_pre = 1;
		}
		return;
	}

	ad_count++;
	if (ad_count >= ad_times + ad_abandon_times)
	{
		ad_deal();
		ad_count = 0;
		ad_sum = 0;
	}
}

void ad_deal(void)
{
	if (ad_turn == 0)
	{
		if (flag_mos_pre) // 采集过程mos状态为打开
		{
			p_check_ad_in();
			$ adcc enable, p_check_ad;
			ad_turn++;
		}
		ad_sum_bandgap = ad_sum;
		ad_sum_out = ad_sum;
		math_eword = ad_sum;
		math_dword = 120 * 4096 * ad_times; // 1.2V,AD次数
		Math_Dword_Div_Eword();
		if (flag_mos_pre)
		{
			VDD_smoking = math_dword; // 吸烟时的板上的VDD电压
		}
		else
		{
			VDD = math_dword; // 采集不带载电池电压
		}
		// }
	}
	else if (ad_turn == 1)
	{
		//	  $ adcc enable, ADC;
		$ adcc enable, BANDGAP; // 363
		p_check_io_in();
		// p_check_temprature_in();
		math_eword = ad_sum;
		math_byte = 120; // 对应电阻值200+1K,120*12/10=144

		short_io_protect(); // io短路保护

		Math_Eword_Mul_Byte();

		short_io_protect(); // io短路保护

		math_eword = ad_sum_bandgap;
		Math_Dword_Div_Eword();

		V_out = math_dword; // 得到实际输出电压//如果是恒压输出，就在这里取得输出电压，这里是全功率输出
		flag_pwm_deal = 1;
		if (flag_mos_pre)
		{
			if (VDD_smoking >= math_dword) // 短路和过流保护判断
			{
				math_word = VDD_smoking - math_dword;
				if (math_word >= V_overload)
				{
					//	p_test_1;
					mos_duty = 0;
					mos_off(); // 紧急关闭输出
					//				flag_overload = 1;
					//					flag_charge = 0;
					flag_smoking = 0;
					flag_pre_heat = 0;
					//					V_out_degree = 0;
					clear_led_select();
					flag_led_r = 1;
					flag_led_g = 1;
					flag_led_b = 1;
					//					flag_poweron_blink = 1;
					led_blink_count = blink_overload * 2 + 1; // 闪烁三次
					led_blink_half_circle = 50;
					led_blink_delay = 0;  // 闪烁，需要清零闪烁计时
					flag_blink_force = 1; // 设为强制，闪烁过程清除连击状态
				}
			}
		}

		ad_turn = 0;
	}
}

//-----------------------------------------//
//---------------AD重置处理----------------//
//-----------------------------------------//
void ad_reset(void)
{
	p_check_io_in();
	p_check_temprature_io_in();
	ad_turn = 0;
	ad_count = 0;
	ad_sum = 0;
	//  $ adcc enable, ADC;
	$ adcc enable, BANDGAP; // 363
	AD_Start = 1;
}

/******************************************/
//-------开mos就进行短路保护IO监测--------//
/******************************************/
void short_io_protect(void)
{
	if (!flag_mos)
	{
		return;
	}
	if (ad_turn == 0)
	{
		p_check_out0();
		a = 2;
		while (a--)
		{
			if (!flag_mos)
			{
				p_check_io_in();
				// p_check_temprature_in();
				return;
			}
		}
		p_check_io_in();
		//();
		nop;
		nop;
		nop;
		nop;
		a = 15;
		do
		{
			if (!flag_mos)
			{
				return;
			}
			if (p_check)
			{
				return;
			}
		} while (a--);
		mos_duty = 0;
		mos_off(); // 紧急关闭输出
		//		flag_overload = 1;
		flag_pre_heat = 0;
		flag_smoking = 0;
		//		flag_charge = 0;

		clear_led_select();
		flag_led_r = 1;
		flag_led_g = 1;
		flag_led_b = 1;
		//		flag_poweron_blink = 1;
		//		V_out_degree = 0;
		led_blink_count = blink_overload * 2 + 1; // 闪烁三次
		led_blink_half_circle = 50;
		led_blink_delay = 0;  // 闪烁，需要清零闪烁计时
		flag_blink_force = 1; // 设为强制，闪烁过程清除连击状态     test 00000
							  //		ad_reset();//那边会复位AD
	}
}

void temp_alarm(void)
{
	if (flag_temp_alarm)
	{
		mos_duty = 0;
		mos_off(); // 紧急关闭输出
		//		flag_overload = 1;
		flag_pre_heat = 0;
		flag_smoking = 0;
		//		flag_charge = 0;

		clear_led_select();
		flag_led_r = 1;
		// flag_led_g = 1;
		 flag_led_b = 1;
		//		flag_poweron_blink = 1;
		//		V_out_degree = 0;
		led_blink_count = 7 * 2 + 1; // 闪烁三次
		led_blink_half_circle = 50;

		flag_temp_alarm = 0;
	}
	else
	{
		return;
	}
}
void temperature_check_sample_once(void)
{
	if (!AD_Start)
	{
		return;
	}

	ad_value = (adcrh << 8 | adcrl);
	ad_value = ad_value >> 4;

	if (temp_ad_count >= ad_abandon_times)
	{
		temp_ad_sum += ad_value; // adcr;
	}

	AD_Start = 1; // 开始

	// short_io_protect(); // io短路保护

	if ((flag_mos && (!flag_mos_pre)) || ((!flag_mos) && flag_mos_pre))
	{
		ad_reset();
		if (flag_mos_pre)
		{
			flag_mos_pre = 0;
		}
		else
		{
			flag_mos_pre = 1;
		}
		return;
	}

	temp_ad_count++;
	if (temp_ad_count >= ad_times + ad_abandon_times)
	{
		temp_ad_deal();
		temp_ad_count = 0;
		temp_ad_sum = 0;
	}
}
void temp_ad_deal(void)
{
    if (ad_turn == 0)   // 采完BANDGAP，切到P_temp，处理bandgap
    {
        $ adcc enable, P_check_temprature_ad;   // 下次温度通道
        p_check_temprature_in();
        ad_turn++;

        temp_ad_sum_bandgap = temp_ad_sum;                // 本轮采集到的是BANDGAP结果
        // 不要在这里清ad_sum（下一步P_temp要用ad_sum累加）
    }
    else if (ad_turn == 1) // 采完P_temp，切到BANDGAP，处理温度
    {
        $ adcc enable, BANDGAP;                 // 下次bandgap通道
        p_check_temprature_io_in();             // 设置回IO（如有需要）
        ad_turn = 0;

        // 这里的ad_sum是P_temp（NTC分压）采样结果
        // 用上一次ad_sum_bandgap配合本次ad_sum还原V_temp

        // 还原V_temp = (ad_sum * 1.2 * 100) / ad_sum_bandgap
        math_eword = temp_ad_sum;        // 温度通道AD和
        math_byte = 120;            // 1.2*100
        Math_Eword_Mul_Byte();

        math_eword = temp_ad_sum_bandgap;
        Math_Dword_Div_Eword();

        V_temp = math_dword;        // 单位0.01V
		//V_temp = 286; 测试

        // R6 = R5 * V_temp / (Vout - V_temp)
        math_eword = V_temp;
        math_byte = 7;          // 6.8K
       // EWord_Mul_Word();           // 结果在 math_eword
	   Math_Eword_Mul_Byte();

       // math_eword = V_out > V_temp ? (V_out - V_temp) : 1; // 防止分母为0
	   if (V_out > V_temp)
	   {
		   math_eword = V_out - V_temp;
	   }
	   else
	   {
		   //math_eword = 1;
		   return;
	   }
	
        Math_Dword_Div_Eword();   
        R6 = math_dword;

        // 判断温度阈值
        // if(R6 <= 10) { flag_temp_alarm = 1; }
        // else { flag_temp_alarm = 0; }
    }
}

#include	"../define.h"



byte high_count, high_count_save;
byte check_circle_count, check_disconnect_delay, disconnect_count, check_low_count;
byte sleep_delay_count;
word smoking_time_count;
byte vdd_degree;
byte lowload_blink_count, blink_times_count;
byte lowload_delay_count;
byte V_out_degree;
word charge_circle_count;
byte charge_disconnect_count;
word full_count;

byte charge_circle_count_fast;
byte char_disconnect_check_delay;
byte charge_disconnect_count_fast;
byte charge_disconnect_count_fast_ad;
byte vdd_degree_temp;
word vdd_degree_delay;
word VDD_temp;

#define full_count_protect smoking_time_count//复用内存!!!!!!!!!


byte r_system_flag;
bit flag_sleep			:r_system_flag.0	
bit flag_mos			:r_system_flag.1
bit flag_smoking		:r_system_flag.2
bit flag_overload		:r_system_flag.3//发生短路
bit flag_full			:r_system_flag.4
bit flag_charge			:r_system_flag.5
bit flag_low			:r_system_flag.6//欠压
bit flag_lock			:r_system_flag.7

byte r_system_flag1;
bit flag_charge_active	:r_system_flag1.0	
bit flag_poweron_test_full:r_system_flag1.1
bit flag_VDD_charging	:r_system_flag1.2	
bit flag_finish_show_vdd:r_system_flag1.3
bit flag_show_vdd		:r_system_flag1.4
bit flag_lowload		:r_system_flag1.5
bit flag_open_mos		:r_system_flag1.6
bit flag_pre_heat		:r_system_flag1.7

byte r_system_flag2;
bit flag_char_disconn_check	:r_system_flag2.0	
bit flag_vdd_degree_down	:r_system_flag2.1	
bit flag_vdd_degree_up		:r_system_flag2.2	
bit flag_charge_first_in	:r_system_flag2.3	


void sleep_deal(void);
void charge_scan(void);
void charge_deal(void);
void charge_reset(void);
void check_low(void);
void check_full(void);
void p_check_out0(void);
void p_check_in(void);
void check_vdd_degree(void);

void smoking_lowload_blink(void);
void reset_lowload_blink(void);

void degree_led_select(void);

void charging_circle(void);
void charging_circle_fast(void);
void reset_charge_data(void);
void check_full_fast(void);
void charge_degree_led_select(void);


void system_deal(void)
{
	if(!flag_blink_force)
	{
		key_scan_c_5c();//按键扫描
//		if(led_blink_count) { reset_5click(); }//只要有闪烁，就不执行连击按键的动作
		charge_scan();//充电检测扫描
		///////////////////充电判断，优先处理
		if(flag_charge)
		{
//			charge_deal();
		}
		elseif(flag_5click)
		{
			led_blink_count = blink_lock*2+1;
//			led_blink_half_circle = 40;
			flag_blink_force = 1;						//!!!!!有这句就是闪烁期间不给锁机
			led_blink_delay = 30;
//			degree_led_select();//调压档位选灯
			clear_led_select();//调压档位选灯
			flag_led_r = 1;
			flag_led_g = 1;
			flag_led_b = 1;

			if(flag_lock)
			{
				flag_lock = 0;
			}
			else
			{
				flag_lock = 1;
			}
		}
		elseif(flag_3click)
		{
			flag_pre_heat = 0;
			if(!flag_lock)
			{
				V_out_degree ++;
				if(V_out_degree > 2)
				{
					V_out_degree = 0;
				}
				degree_led_select();//调压档位选灯
				led_blink_count = blink_degree_select*2+1;//调档闪烁
//				led_blink_half_circle = 50;
				flag_blink_force = 1;						//!!!!!有这句就是闪烁期间不给锁机
				led_blink_delay = 20;	
			}
		}
		elseif(flag_2click || (flag_pre_heat && (!flag_key_press)) )
		{
			if(!flag_lock)
			{
				if(flag_pre_heat)
				{
					smoking_time_count ++;
					if(smoking_time_count >= pre_heat_time_up_data)
					{
						flag_pre_heat = 0;
						degree_led_select();//调压档位选灯
						led_duty = 0;
						led_off();
//						led_blink_count = blink_smoke_time_up*2+1;//超时闪烁
//						led_blink_half_circle = 50;
//						flag_blink_force = 1;						//!!!!!有这句就是闪烁期间不给锁机
//						led_r_duty = 0;
//						led_g_duty = 0;
//						led_b_duty = 0;
					}
				}
				if(flag_2click && (!led_blink_count))
				{
//					degree_led_select();//调压档位选灯
					clear_led_select();
					flag_pre_heat = 1;
					led_duty = 0;
					smoking_time_count = 0;
					led_delay_count = 30;
					led_turn = 0;
					mos_duty = mos_duty_max;//*370/420;
					mos_on(); 
				}
			}
		}
		elseif(!led_blink_count)
		{
			///////////////////吸烟判断
//			if(flag_key_press && (flag_low == 0))
			if(flag_key_press)
			{
				flag_pre_heat = 0;
//				if(!flag_smoking)
				{
	//				flag_finish_show_vdd = 0;
	//				flag_show_vdd = 0;
	//				check_vdd_degree();//有吸烟电量指示，加入此句
					degree_led_select();//调压档位选灯
					if(VDD >= VDD_low_data)
					{
						if(!flag_lock)
						{
							flag_smoking = 1;//正常吸烟
//							flag_open_mos = 0;
							mos_duty = mos_duty_max;//*370/420;//以最小占空比开始
							mos_on(); 
							led_on();	
	//						led_duty = led_duty_max/10;
							smoking_time_count = 0;//抽烟计时清零
//							VDD_smoking = 370;//给个初始化，不至于还没采到AD就吸烟时低压保护了
						}
					}
					else
					{
						if(!flag_lock)
						{
//							clear_led_select();
//							flag_led_r = 1;
							led_blink_count = blink_low*2+1;//低压闪烁
							flag_blink_force = 1;			//!!!!!有这句就是闪烁期间不给锁机
//							led_blink_half_circle = 50;
						}
					}
				}
/*				else
				{
					flag_smoking = 0;
					led_off();
				}
*/			}
			///////////////////吸烟过程处理
			elseif(flag_smoking)
			{
				if(flag_key)
				{
					smoking_time_count ++;
					if(smoking_time_count >= smoke_time_up_data)
					{
						led_blink_count = blink_smoke_time_up*2+1;//超时闪烁
						led_blink_half_circle = 50;
//						flag_blink_force = 1;						//!!!!!有这句就是闪烁期间不给锁机
						flag_smoking = 0;
					}
				}
				else
				{
					flag_smoking = 0;
				}
/*				if(smoking_time_count >= 100)
				{
					flag_open_mos = 1;
					mos_duty = mos_duty_max;//*380/420;//以最小占空比开始
					mos_on(); 
				}
				else
				{
					smoking_time_count ++;
				}
				if((!flag_key) && (!flag_open_mos))
				{
					flag_smoking = 0;
					led_off();
				}
*/				
				if(flag_smoking)
				{
					//仍在按按着按键
//					if(flag_lowload)
					{
//						mos_off();		//非全功率输出，交由pwm函数处理mos
//						smoking_lowload_blink();
					}
//					else
					{
//						reset_lowload_blink();
//						mos_on();		//交由pwm函数处理mos
						led_on();
					}
				}
				else
				{
					//松开按键或超时
					led_off();
//					if(flag_lowload)
					{
//						led_duty = 0;
					}
//					mos_off();		//此处全功率//非全功率输出，交由pwm函数处理mos
				}
/*				if(VDD_smoking <= 275)
				{
					flag_smoking = 0;
					led_blink_count = blink_smoking_low*2+1;//低压闪烁
					led_blink_half_circle = 20;
				}
*/			}
		}
	}
	else
	{
		reset_5click();
	}

	flag_key_press = 0;
	flag_5click = 0;
	flag_3click = 0;
	flag_2click = 0;

	///////////////////判断是否睡眠
	if((!flag_pre_heat) && (!led_blink_count) && (!flag_smoking) && ((!flag_charge) || (flag_charge && flag_full)) && (!flag_key_active) && (!flag_charge_active))
		//不闪灯 不吸烟 不充电或者充电已经充满 按键状态稳定 充电接口状态稳定
//	if((!led_blink_count) && (!flag_smoking) && (!flag_charge) && (!flag_key_active) && (!flag_charge_active))
		//不闪灯 不吸烟 不充电 按键状态稳定 充电接口状态稳定
//	if((!led_blink_count) && (!led_duty) && (!flag_smoking) && (!flag_key_active) && (!flag_charge_active))
		//不闪灯 灯已经熄灭 不吸烟 按键状态稳定 充电接口状态稳定
//	if((!led_blink_count) && (!led_duty) && (!flag_smoking) && (!flag_key_active))
		//不闪灯 灯已经熄灭 不吸烟 按键状态稳定 
	{
		sleep_delay_count ++;
		if(sleep_delay_count >= sleep_delay_time)
		{
			sleep_delay_count = 0;
			flag_sleep = 1;
		}
	}
	else
	{
		sleep_delay_count = 0; 
	}
}


/*-------------------------------------//
//------------充电扫描处理-------------//
//-------------------------------------*/
void charge_scan(void)
{
	static byte charge_scan_delay;
	if(flag_smoking || flag_pre_heat)
	{
		charge_scan_delay = 0;
		flag_charge_active = 0;//充电接口状态稳定
		return;
	}
	if(flag_charge)
	{
		if(!led_blink_count)
		{
//			check_full();
//			charging_circle();
			check_full_fast();
			charging_circle_fast();
		}
	}
	else
	{
		if(p_check && (!led_blink_count))
		{
			flag_charge_active = 1;//充电接口有动态
			charge_scan_delay ++;
			if(charge_scan_delay >= 120)
			{
				charge_scan_delay = 0;
				flag_charge_active = 0;//充电接口状态稳定
				flag_charge = 1;
				flag_charge_first_in = 1;
//				degree_led_select();//调压档位选灯
				clear_led_select();
				flag_led_r = 1;
				flag_led_g = 1;
				flag_led_b = 1;
				led_blink_count = blink_get_charge*2+1;//闪烁
				led_blink_delay = 0;	//闪烁，需要清零闪烁计时
//				led_blink_half_circle = 50;	
				reset_charge_data();
			}
		}
		else
		{
			charge_scan_delay = 0;
			flag_charge_active = 0;//充电接口状态稳定
		}
	}
}


void check_charge_degree(void)
{
	check_vdd_degree();
	if(flag_charge_first_in)
	{
		flag_charge_first_in = 0;
		vdd_degree = vdd_degree_temp;
		charge_degree_led_select();
	}
	if(vdd_degree_temp > vdd_degree)
	{
		flag_vdd_degree_down = 0;
		if(flag_vdd_degree_up)
		{
			vdd_degree_delay ++;
			if(vdd_degree_delay >= 600)
			{
				vdd_degree_delay = 0;
				vdd_degree ++;
				charge_degree_led_select();
			}
		}
		else
		{
			flag_vdd_degree_up = 1;
			vdd_degree_delay = 1;
		}
	}
	else if(vdd_degree_temp < vdd_degree)
	{
		flag_vdd_degree_up = 0;
		if(flag_vdd_degree_down)
		{
			vdd_degree_delay ++;
			if(vdd_degree_delay >= 600)
			{
				vdd_degree_delay = 0;
				vdd_degree --;
				charge_degree_led_select();
			}
		}
		else
		{
			flag_vdd_degree_down = 1;
			vdd_degree_delay = 1;
		}
	}
	else
	{
		flag_vdd_degree_up = 0;
		flag_vdd_degree_down = 0;
	}
}

/*-------------------------------------//
//-----------充电过程检测循环----------//
//-------------------------------------*/
void charging_circle_fast(void)
{
	charge_circle_count_fast ++;
	if(charge_circle_count_fast >= 4)//乘以5ms
	{
		charge_circle_count_fast = 0;
		mos_duty = 0;
		mos_off();
		p_check_out0();
//		char_disconnect_check_delay = 20;//乘以100us
	}
	else if(!flag_full)
	{		
		check_charge_degree();
		led_duty = led_duty_max;
		led_on();
		mos_duty = mos_duty_max;
		mos_on();
	}
}


void check_full_fast(void)
{
	if(!flag_full)
	{
		if(VDD > VDD_full_data)
		{
			full_count ++;
			if(full_count > 1000)
			{
				flag_full = 1;
			}
		}
		else 
		{
			full_count = 0;
		}

		if(VDD > (VDD_full_data - 6))
		{
			if(VDD_temp < VDD)
			{
				full_count_protect = 0;
				if(VDD_temp == 0)	VDD_temp = VDD;
				else				VDD_temp ++;
			}
			else
			{
				full_count_protect++;
				if(full_count_protect > 65500)
				{
					flag_full =  1;
				}
			}
		}
		else
		{
			full_count_protect = 0;
		}

		if(flag_full)
		{
			full_count = 0;
			full_count_protect = 0;
//			led_blink_count = blink_full*2+1;//闪烁
//			led_blink_delay = 0;	//闪烁，需要清零闪烁计时
//			led_blink_half_circle = 50;	
			mos_duty = 0;
			mos_off();
			led_off();
			led_duty = 0;
		}
	}
	else
	{
		full_count = 0;
		full_count_protect = 0;
	}
}


/*-------------------------------------//
//-----------充电过程检测循环----------//
//-------------------------------------*/
void charging_circle(void)
{
	charge_circle_count ++;
	if(charge_circle_count < 380)
	{
		if(!flag_full)
		{
			clear_led_select();
			flag_led_r = 1;
			led_on();
			mos_duty = mos_duty_max;
			mos_on();
		}
	}
	elseif(charge_circle_count == 380)
	{
//		led_on();
		mos_duty = 0;
		mos_off();
		p_check_out0();
		charge_disconnect_count = 0;
	}
	elseif(charge_circle_count < 400)
	{
		p_check_io_in();
//		led_on();
		mos_duty = 0;
		mos_off();
		nop;
		nop;
		nop;
		if(!p_check)
		{
			charge_disconnect_count ++;
		}
		p_check_out0();
	}
	else
	{
		p_check_io_in();
		if(flag_full)
		{
			charge_circle_count = 379;
		}
		else
		{
			charge_circle_count = 0;
		}
		if(charge_disconnect_count >= 10)
		{
			mos_duty = 0;
			mos_off();
			flag_charge = 0;
//			degree_led_select();//调压档位选灯
			clear_led_select();
			flag_led_r = 1;
			led_blink_count = blink_off_charge*2+1;//闪烁
			led_blink_delay = 0;	//闪烁，需要清零闪烁计时
//			led_blink_half_circle = 50;	
		}
		else if(!flag_full)
		{
//			degree_led_select();//调压档位选灯
			led_on();
			mos_duty = mos_duty_max;
			mos_on();
		}
	}
}

/*-------------------------------------//
//------------充电过程参数复位---------//
//-------------------------------------*/
void reset_charge_data(void)
{
	flag_full = 0;
	full_count = 0;
//	charge_circle_count = 379;
	charge_disconnect_count_fast = 0;
	flag_vdd_degree_up = 0;
	flag_vdd_degree_down = 0;
	full_count_protect = 0;//复用内存!!!
	VDD_temp = 0;
}


void check_full(void)
{
	if(!flag_full)
	{
		if(VDD > VDD_full_data)
		{
			full_count ++;
			if(full_count > 1000)
			{
				full_count = 0;
				flag_full = 1;
				charge_circle_count = 379;
//				degree_led_select();//调压档位选灯
				clear_led_select();
				flag_led_r = 1;
				led_blink_count = blink_full*2+1;//闪烁
				led_blink_delay = 0;	//闪烁，需要清零闪烁计时
//				led_blink_half_circle = 50;	
				mos_duty = 0;
				mos_off();
			}
		}
		else
		{
			full_count = 0;
		}
	}
	else
	{
		full_count = 0;
	}
}

void check_vdd_degree(void)
{
	vdd_degree_temp = 0;
	if(VDD > vdd_degree01)
	{
		vdd_degree_temp ++;
		if(VDD > vdd_degree12)
		{
			vdd_degree_temp ++;
//			if(VDD > vdd_degree23)
			{
//				vdd_degree_temp ++;
			}
		}
	}
}

void reset_lowload_blink(void)
{
	lowload_blink_count = 0;
	blink_times_count = 0;
}

void smoking_lowload_blink()
{
	lowload_blink_count ++;
	if(lowload_blink_count >= 100)
	{
		lowload_blink_count = 0;
		blink_times_count ++;
	}
	if(blink_times_count.0)
	{
		led_off();
	}
	else
	{
		led_on();
	}
}

void p_check_out0(void)
{
	p_check_c = 1;
	p_check = 0;
}
void p_check_io_in(void)
{
	p_check_c = 0;
	set_check_io;
}
void p_check_ad_in(void)
{
	p_check_c = 0;
	set_check_ad;
}

void mos_on(void)
{
	p_mos = 0;
	p_mos_c = 1;
	p_mos = 0;
	flag_mos = 1;
}

void mos_off(void)
{
	p_mos_c = 0;
	flag_mos = 0;
}

void degree_led_select(void)
{
	clear_led_select();
	if(V_out_degree)
	{
		if(V_out_degree == 1)//第1档
		{
			flag_led_b = 1;
		}
		else//第2档
		{
			flag_led_r = 1;
		}
	}
	else//默认档位
	{
		flag_led_g = 1;
	}
}

void charge_degree_led_select(void)
{
	//--------------------------档位选择
	clear_led_select();
	switch(vdd_degree)
	{
	case 0:
		flag_led_r = 1;
		break;
	case 1:
		flag_led_r = 1;
		break;
	default:
		flag_led_r = 1;
		break;
	}
}


#include "../define.h"

#define p_key0 1
#define p_key1 1

byte key_value, key_click_count;
word double_click_delay_count, long_key_count, key_count;

byte r_key_flag;
bit flag_key			:r_key_flag.0
bit flag_key_press		:r_key_flag.1
bit flag_5click			:r_key_flag.2
bit flag_key_active		:r_key_flag.3
bit flag_3click			:r_key_flag.4
bit flag_2click			:r_key_flag.5

//--------------------------------------------------客制变量
byte click_time_count, click_count;
word key_5c_count;

void key_scan_c_5c(void);
void reset_5click(void);
void charge_scan(void);
//-------------------------------------//
//---------------单击------------------//
//-------------------------------------//
void key_scan_click(void)
{
	if((flag_key && p_key0) || ((!flag_key) && (!p_key0)))
	{
		key_count ++;
		if(key_count >= key_delay_time)
		{
			key_count = 0;
			if(flag_key)
			{
				flag_key = 0;
			}
			else
			{
				flag_key = 1;
				key_value = 1;	//输出单击
			}
		}
	}
	else
	{
		key_count = 0;
	}
}

//-------------------------------------//
//-------------单击 + 双击-------------//
//-------------------------------------//
void key_scan_c_dc(void)
{
	if((flag_key && p_key0) || ((!flag_key) && (!p_key0)))
	{
		key_count ++;
		if(key_count >= key_delay_time)
		{
			key_count = 0;
			if(flag_key)
			{
				flag_key = 0;
			}
			else
			{
				flag_key = 1;
				flag_key_press = 1;
			}
		}
	}
	else
	{
		key_count = 0;
	}
	//-------------------------
	if(flag_key)
	{
		if(flag_key_press && (double_click_delay_count != 0))
		{
			flag_key_press = 0;
			double_click_delay_count = 0;
			key_value = 2;		//输出双击
		}
		else
		{
			double_click_delay_count = 0;
		}
	}
	else
	{
		if(flag_key_press)
		{
			double_click_delay_count ++;
			if(double_click_delay_count >= double_click_delay_time)
			{
				flag_key_press = 0;
				double_click_delay_count = 0;
				key_value = 1;	//输出单击
			}
		}
	}
}

//-------------------------------------//
//-------------单击 + 长按-------------//
//-------------------------------------//
void key_scan_c_lp(void)			//单击 + 长按
{
	if((flag_key && p_key0) || ((!flag_key) && (!p_key0)))
	{
		key_count ++;
		if(key_count >= key_delay_time)
		{
			key_count = 0;
			if(flag_key)
			{
				flag_key = 0;
			}
			else
			{
				flag_key = 1;
				flag_key_press = 1;
			}
		}
	}
	else
	{
		key_count = 0;
	}
	//-------------------------
	if(flag_key)
	{
		if(flag_key_press)
		{
			long_key_count ++;
			if(long_key_count >= long_key_time)
			{
				flag_key_press = 0;
				long_key_count = 0;	
				key_value = 3;		//输出长按
			}
		}
	}
	else
	{
		if(flag_key_press)
		{
			key_value = 1;			//输出单击
		}
		long_key_count = 0;
		flag_key_press = 0;
	}
}

//-------------------------------------//
//-------------双击 + 长按-------------//
//-------------------------------------//
void key_scan_dc_lp(void)			//双击 + 长按
{
	if((flag_key && p_key0) || ((!flag_key) && (!p_key0)))
	{
		key_count ++;
		if(key_count >= key_delay_time)
		{
			key_count = 0;
			if(flag_key)
			{
				flag_key = 0;
			}
			else
			{
				flag_key = 1;
				flag_key_press = 1;
			}
		}
	}
	else
	{
		key_count = 0;
	}
	//-------------------------
	if(flag_key)
	{
		double_click_delay_count = 0;
		if(flag_key_press)
		{
			long_key_count ++;
			if(long_key_count >= long_key_time)
			{
				flag_key_press = 0;
				long_key_count = 0;	
				key_click_count = 0;
				key_value = 3;		//输出长按
			}
		}		
	}
	else
	{
		long_key_count = 0;
		if(flag_key_press)
		{
			key_click_count ++;
		}
		flag_key_press = 0;

		if(key_click_count >= 2)
		{
			double_click_delay_count = 0;
			key_click_count = 0;
			key_value = 2;		//输出双击
		}

		if(key_click_count != 0)
		{
			double_click_delay_count ++;
			if(double_click_delay_count >= double_click_delay_time)
			{
				double_click_delay_count = 0;
				key_click_count = 0;
			}
		}
	}
}
//-------------------------------------//
//---------单击 + 双击 + 长按----------//
//-------------------------------------//
void key_scan_c_dc_lp(void)			//单击 + 双击 + 长按
{
	if((flag_key && p_key0) || ((!flag_key) && (!p_key0)))
	{
		key_count ++;
		if(key_count >= key_delay_time)
		{
			key_count = 0;
			if(flag_key)
			{
				flag_key = 0;
			}
			else
			{
				flag_key = 1;
				flag_key_press = 1;
			}
		}
	}
	else
	{
		key_count = 0;
	}
	//-------------------------
	if(flag_key)
	{
		double_click_delay_count = 0;
		if(flag_key_press)
		{
			long_key_count ++;
			if(long_key_count >= long_key_time)
			{
				flag_key_press = 0;
				long_key_count = 0;	
				key_click_count = 0;
				key_value = 3;		//输出长按
			}
		}		
	}
	else
	{
		long_key_count = 0;
		if(flag_key_press)
		{
			key_click_count ++;
		}
		flag_key_press = 0;

		if(key_click_count >= 2)
		{
			double_click_delay_count = 0;
			key_click_count = 0;
			key_value = 2;		//输出双击
		}

		if(key_click_count != 0)
		{
			double_click_delay_count ++;
			if(double_click_delay_count >= double_click_delay_time)
			{
				double_click_delay_count = 0;
				key_click_count = 0;
				key_value = 1;		//输出单击
			}
		}
	}
}


//-------------------------------------//
//------------客制按键扫描-------------//
//-------------------------------------//
void key_scan_c_5c(void)//单击+5连击
{
	if((flag_key && p_key) || ((!flag_key) && (!p_key)))
	{
		key_count ++;
		if(key_count >= key_delay_time)
		{
			key_count = 0;
			if(flag_key)
			{
				flag_key = 0;
			}
			else
			{
				flag_key = 1;
				flag_key_press = 1;//此处置起标志位，到外面用完就清掉
			}
		}
		flag_key_active = 1;//按键状态有动态
	}
	else
	{
		key_count = 0;
		flag_key_active = 0;//按键状态稳定
	}
	//--------------------------检测5连击
	if(flag_key_press)
	{
		if(click_time_count == 0)//五连击的第一击
		{
			click_time_count ++;
			click_count = 1;			//单击次数为1
			key_5c_count = 0;			//连击总计时清零
		}
		else	//非第一击
		{
			key_5c_count += click_time_count;//2.5S计时
			if(key_5c_count < 500)
			{
				click_count ++;
				if(click_count>4)	//单击计数+1,判断连击次数
				{
					flag_5click = 1;	//确认5连击
					click_time_count = 0;
				}
				else
				{
					click_time_count = 1;
				}
			}
			else//总时间超时
			{
				click_time_count = 0; //单次计时清零
			}
		}
	}
	else
	{
		if(click_time_count)
		{
			click_time_count ++;
			if(click_time_count>110)	//点击间隔超过0.55S视作超时
			{
				click_time_count = 0;
				if(!flag_key)
				{
					if(click_count == 2)
					{
						flag_2click = 1;
					}
					else if((key_5c_count < 270) && (click_count == 3))
					{
						flag_3click = 1;
					}
				}
			}
		}
	}
}

///////////////////////////////////////
//---------重置按键连击扫描----------//
///////////////////////////////////////
void reset_5click(void)
{
	click_time_count = 0;
}


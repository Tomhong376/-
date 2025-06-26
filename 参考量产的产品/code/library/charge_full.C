
#include "../define.h"

void charge_full_check(void)
{
	charge_check();
	full_check();
}


//-------------------------------------//
//-------------³äµç¼ì²â----------------//
//-------------------------------------//
void charge_check(void)
{
	if((flag_charge && (!p_input)) || ((!flag_charge) && p_input))			
	{
		input_count ++;
		if(input_count >= input_delay_time)
		{
			input_count = 0;
			if(flag_charge)
			{
				flag_charge = 0;
			}
			else
			{
				flag_charge = 1;
			}
		}
	}
	else
	{
		input_count = 0;
	}
}


//-------------------------------------//
//-------------³äÂú¼ì²â----------------//
//-------------------------------------//
void full_check(void)
{
	if(flag_charge)
	{
		full_count ++;
		if(full_count >= full_delay_time)
		{
			full_count = 0;
			flag_full = 1;
		}
	}
	else
	{
		flag_full = 0;
	}
}
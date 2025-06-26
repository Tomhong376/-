
#include "../define.h"



byte iic_my_address, iic_bit_count, iic_data, iic_byte_count, iic_temp;

byte r_iic_flag;
bit flag_iic_receive_ok	:r_iic_flag.0
bit flag_iic_offset_ok	:r_iic_flag.1//偏移地址


test_here macro	//测试代码
//	pa.0 = 1;
//	nop;
//	pa.0 = 0;
endm

sda_out0 macro
	pac.3 = 1;
	p_sda = 0;
endm

sda_in macro
//	p_sda = 1;	//先试验不输出高而直接切回输入
	pac.3 = 0;
endm



iic_get_byte macro
	iic_bit_count = 8;
	while(iic_bit_count--)
	{
		iic_data <<= 1;
		while(!p_scl)//等待时钟线变高
		{
			if(intrq.T16)
			{
				goto 	iic_stop;
			}
			.wdreset;
		}
		delay_1us();
		if(p_sda)
		{
			delay_1us();
			if(p_sda)
			{
				iic_data ++;
			}
			else
			{
				goto iic_stop;
			}
		}
		else
		{
			delay_1us();
			if(p_sda)
			{
				goto iic_stop;
			}
		}
		test_here
		if(iic_data.0)
		{
			while(p_scl)//等待时钟线变低
			{
				a = pa & 0b0001_1000;
				if(a == 0b0001_0000)
				{
					delay_1us();
					a = pa & 0b0001_1000;
					if(a == 0b0001_0000)
					{
						stt16	system_ram;
						intrq.t16 = 0;
						goto	iic_start;
					}
				}
				if(!p_scl)
				{
					break;
				}
				if(intrq.T16)
				{
					goto 	iic_stop;
				}
				.wdreset;
			}
		}
		else
		{
			while(p_scl)//等待时钟线变低
			{
				a = pa & 0b0001_1000;
				if(a == 0b0001_1000)
				{
					delay_1us();
					a = pa & 0b0001_1000;
					if(a == 0b0001_1000)
					{
						goto	iic_stop;
					}
				}
				if(!p_scl)
				{
					break;
				}
				if(intrq.T16)
				{
					goto 	iic_stop;
				}
				.wdreset;
			}
		}
		test_here
	}
endm

send_ack macro
	sda_out0
	while(!p_scl)
	{
		if(intrq.T16)
		{
			goto	iic_stop;
		}
		.wdreset;
	}
	while(p_scl)
	{
		if(intrq.T16)
		{
			goto	iic_stop;
		}
		.wdreset;
	}
	stt16 system_ram;
	intrq.T16 = 0;
	sda_in
endm

get_ack macro
	while(!p_scl)
	{
		if(intrq.T16)
		{
			goto	iic_stop;
		}
		.wdreset;
	}
	delay_1us();
	a = pa & 0b0001_1000;
	if(a == 0b0001_0000)
	{
		delay_1us();
		a = pa & 0b0001_1000;
		if(a == 0b0001_0000)
		{
			stt16 system_ram;	//收到正常应答
			while(p_scl)
			{
				if(intrq.t16)
				{
					goto	iic_stop;
				}
				.wdreset;
			}
		}
		else
		{
			goto	iic_stop;
		}
	}
	else
	{
		goto	iic_stop;
	}
endm


//---------------------------------------------------------------//
//---------------------------iic从机-----------------------------//
//---------------------------------------------------------------//
void iic_standby()
{
	while(1)
	{
iic_stop:
		sda_in
		$ t16m stop;	//关计时
		while(1)//---------------------------------------检测开始信号
		{
			test_here
			while(1)//等待两者都为高电平
			{
				F += 55;			//产生随机数
				.wdreset;
				a = pa & 0b0001_1000;
				if(a == 0b0001_1000)
				{
					break;
				}
			}
			while(1)//等待电平变化
			{
				F ++;	//产生随机数
				test_here
				.wdreset;
				if(!p_slp)
				{
					a = pa & 0b0001_1000;
					if(a == 0b0001_0000)
					{
						delay_1us();
						a = pa & 0b0001_1000;
						if(a == 0b0001_0000)
						{
							test_here
							stt16 system_ram;
							$ t16m IHRC, /64, bit15;
							INTEN.T16 = 1;						//开t16中断							 
							INTRQ = 0;
							goto iic_start;//检测到开始信号
						}
					}
					else if(!p_scl)
					{
						break;
					}
				}
				else
				{
					sleep_delay_count ++;
					if(sleep_delay_count >= sleep_delay_time)
					{
						sleep_delay_count = 0;
						flag_sleep = 1;
						return;
					}
				}
			}
		}

iic_start:
		flag_iic_offset_ok = 0;
		while(p_scl)//等待时钟线变低
		{
			if(intrq.T16)
			{
				goto 	iic_stop;
			}
			test_here
			.wdreset;
			test_here
		}

		iic_get_byte//-------------------------------------接收器件地址		
		a = iic_my_address + 1 - iic_data;
		if(a == 0)//------------------------------------主机读从机
		{	
			iic_byte_count = 10;
			point = ck235_des_data;
			send_ack
			while(iic_byte_count--)
			{
				iic_data = *point;
				iic_bit_count = 8;
				while(iic_bit_count--)
				{
					if(iic_data.7)
					{
						sda_in;
					}
					else
					{
						sda_out0;
					}
					while(!p_scl)
					{
						if(intrq.T16)
						{
							goto	iic_stop;
						}
					}
					iic_data <<= 1;
					test_here
					while(p_scl)
					{
						if(intrq.T16)
						{
							goto	iic_stop;
						}
					}
				}
				sda_in
				point$0 ++;
				get_ack
			}
			.wdreset;
			test_here
		}
		else if(iic_data == iic_my_address)//-----------------接收器件地址(主机写从机)
		{
			send_ack;
			while(1)
			{
				iic_get_byte
				if(flag_iic_offset_ok)
				{
					*point = iic_data;
					point$0 ++;
					iic_byte_count --;
					send_ack;
					if(iic_byte_count == 0)
					{
						flag_iic_receive_ok = 1;
						stt16 system_ram;
						intrq.T16 = 0;
						return;
					}
				}
				else
				{
					if(iic_data == iic_my_address_offset)//------接收偏移地址
					{
						iic_byte_count = 8;	//主机写8个字节到从机
						point = data;
						flag_iic_offset_ok = 1;
						send_ack;
					}
					else
					{
						goto iic_stop;
					}
				}
			}
		}
	}
}

//------------------------------------------------------------//
//-----------------------延时子程序---------------------------//
//------------------------------------------------------------//
void delay_1us(void)
{
	nop;
	nop;
	return;	//1us
}



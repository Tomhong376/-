
#include "../define.h"



byte iic_my_address, iic_bit_count, iic_data, iic_byte_count, iic_temp, data[4];
word point;

byte r_iic_flag;
bit flag_iic_receive_ok	:r_iic_flag.0
bit flag_iic_offset_ok	:r_iic_flag.1//偏移地址
bit flag_iic_error		:r_iic_flag.2
bit flag_iic_data_error	:r_iic_flag.3


test_here macro	//测试代码
//	pa.0 = 1;
//	nop;
//	pa.0 = 0;
endm

sda_out0 macro
	p_sda_c = 1;
	p_sda = 0;
endm

sda_in macro
//	p_sda = 1;	//先试验不输出高而直接切回输入
	p_sda_c = 0;
endm


/*
iic_get_byte macro
	iic_bit_count = 8;
	while(iic_bit_count--)
	{
		iic_data <<= 2;
		iic_temp <<<= 1;
		while(!p_scl)//等待时钟线变高
		{
			if(intrq.T16)
			{
				stt16	system_ram;
				intrq.T16 = 0;
				goto 	iic_stop;
			}
			.wdreset;
		}
		if(p_sda)
		{
			iic_data ++;
		}
		delay_1us();
		if(p_sda)
		{
			iic_data ++;
		}
		delay_1us();
		if(p_sda)
		{
			iic_data ++;
		}
		test_here
		if(iic_data.1)
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
					stt16	system_ram;
					intrq.T16 = 0;
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
					stt16	system_ram;
					intrq.T16 = 0;
					goto 	iic_stop;
				}
				.wdreset;
			}
		}
		iic_temp >>>= 1;
		iic_data >>>= 1;
		test_here
	}
endm

send_ack macro
	sda_out0
	while(!p_scl)
	{
		if(intrq.T16)
		{
			stt16 system_ram;
			intrq.T16 = 0;
			goto	iic_stop;
		}
		.wdreset;
	}
	while(p_scl)
	{
		if(intrq.T16)
		{
			stt16 system_ram;
			intrq.T16 = 0;
			goto	iic_stop;
		}
		.wdreset;
	}
	stt16 system_ram;
	sda_in
endm

get_ack macro
	while(!p_scl)
	{
		if(intrq.T16)
		{
			stt16 system_ram;
			intrq.T16 = 0;
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
					stt16 system_ram;
					intrq.t16 = 0;
					goto	iic_stop;
				}
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
		while(1)//---------------------------------------检测开始信号
		{
iic_stop:
			while(1)//等待两者都为高电平
			{
				a = pa & 0b0001_1000;
				if(a == 0b0001_1000)
				{
					break;
				}
				F += 55;		//产生随机数
				stt16 system_ram;	//防止定时器溢出
				.wdreset;
			}
			delay_1us();
			while(1)//等待电平变化
			{
				a = pa & 0b0001_1000;
				if(a == 0b0001_0000)
				{
					delay_1us();
					a = pa & 0b0001_1000;
					if(a == 0b0001_0000)
					{
						if(!p_slp)
						{
							goto iic_start;//检测到开始信号
						}
					}
				}
				F ++;		//产生随机数
				stt16 system_ram;	//防止定时器溢出
				if(p_slp)
				{
					sleep_delay_count ++;
					if(sleep_delay_count >= sleep_delay_time)
					{
						sleep_delay_count = 0;
						flag_sleep = 1;
						return;
					}
				}
				.wdreset;
			}

		}

iic_start:
		flag_iic_offset_ok = 0;
		while(p_scl)//等待时钟线变低
		{
			if(intrq.T16)
			{
				stt16	system_ram;
				intrq.T16 = 0;
				goto 	iic_stop;
			}
			.wdreset;
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
							stt16 system_ram;
							intrq.T16 = 0;
							goto	iic_stop;
						}
						.wdreset;
					}
					iic_data <<= 1;
					test_here
					while(p_scl)
					{
						if(intrq.T16)
						{
							stt16 system_ram;
							intrq.T16 = 0;
							goto	iic_stop;
						}
						.wdreset;
					}
				}
				sda_in
				point$0 ++;
				if(iic_byte_count != 0)
				{
					get_ack
				}
			}
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
*/
//------------------------------------------------------------//
//-----------------------延时子程序---------------------------//
//------------------------------------------------------------//
void delay_1us(void)
{
//	nop;
//	nop;
//	nop;
//	return;	//1us
}

void delay_3us(void)
{
//	nop;
//	nop;
//	nop;

//	nop;
//	nop;

//	return; 
}
void delay_5us(void)
{
//	nop;
//	nop;
//	nop;
//	nop;

//	nop;
//	nop;
//	nop;

//	return
}

//------------------------------------------------------------//
//-------------------------IIC主机----------------------------//
//------------------------------------------------------------//
void iic_host(void)
{
	iic_host_start();
	iic_data = 0xC0;
	iic_host_send_byte();
	iic_host_get_ack();
	delay_1us();
	iic_data = 0xA0;
	iic_host_send_byte();
	iic_host_get_ack();
	point = data;		//8个字节的起始地址
	iic_byte_count = 3;	//发8个字节
	while(iic_byte_count--)
	{
		iic_data = *point;
		iic_host_send_byte();
		iic_host_get_ack();
		point ++;
	}
	iic_host_stop();
	.wdreset;
}

void iic_host_start(void)
{
	sda_in
	p_scl = 1;
	delay_5us();
	delay_1us();
	sda_out0
//	delay_1us();
	delay_3us();
}

void iic_host_stop(void)
{
	p_scl = 0;
	delay_1us();
	sda_out0
	delay_3us();
	p_scl = 1;
	delay_5us();
	sda_in
}

void iic_host_send_byte(void)
{
	iic_bit_count = 8;
	while(iic_bit_count--)
	{
		p_scl = 0;
		delay_1us();
		if(iic_data.7)
		{
			sda_in
		}
		else
		{
			sda_out0
		}
		delay_1us();
		p_scl = 1;
		iic_data <<= 1;
		delay_1us();
	}
}

void iic_host_get_ack(void)
{
	p_scl = 0;
	delay_5us();
	p_scl = 1;
//	flag_iic_error = 1;
//	delay_1us();
//	if(!p_sda)
//	{
//		flag_iic_error = 0;
//	}
}
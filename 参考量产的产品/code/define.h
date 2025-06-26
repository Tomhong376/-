
//131引脚图

//							 ---  ---
//		VDD					-|		|- GND
//		PA7/X1				-|		|- PA0/AD10/INT0
//		PA6/X2				-|		|- PA4/AD9/INT1A
//		PA5/RESET			-|		|- PA3/AD8/PWM1
//		PB7/AD7/PWM2		-|		|- PB3/AD3
//		PB4/AD4/PWM1		-|		|- PB1/AD1/Vref
//		PB5/AD5/INT0A/PWM2	-|		|- PB0/AD0/INT1
//							 --------


#include "library/rf.H"
#include "library/ad.H"
#include "library/led.H"
#include "library/math0.H"
#include "library/math1.H"
#include "library/ad_out_test.h"
#include "library/iic.h"
#include "library/key_scan.h"
#include "library/system.H"
#include "library/pwm.h"


#define	ICE 0					//0:烧录用编译，1:仿真用编译

//------------------------管脚定义------------------------//


#define p_mos			pa.5
#define p_key			pa.6
#define p_check			pb.1
#define p_check_ad		PB1

#define p_led_r_on		{ pa.3 = 0; }
#define p_led_r_off		{ pa.3 = 1; } 
#define p_led_w_on		{ pa.4 = 0; }
#define p_led_w_off		{ pa.4 = 1; } 
#define p_led_b_on		{ pb.7 = 0; }
#define p_led_b_off		{ pb.7 = 1; } 
#define p_led_g_on		p_led_w_on
#define p_led_g_off		p_led_w_off


#define p_mos_c			pac.5
#define p_check_c		pbc.1

#define set_check_ad	{$ pbdier	0b_0000_0000;}
#define set_check_io	{$ pbdier	0b_0000_0010;}

#define p_test_1	//	{pb.7 = 1;}
#define p_test_0	//	{pb.7 = 0;}

//-------------------------参数定义-------------------------//


//-------------------------变量定义-------------------------//
extern word system_ram;
extern word point;
extern byte sleep_delay_count;
extern byte r_main_flag;
extern bit flag_FPPA0;			//	:r_main_flag.0
extern bit flag_poweron;	//	:r_main_flag.1




i_o_set macro
	//-----------------------IO口配置-----------------------//
	//多个FPPA共享一个IO,并且要作Input/Output切换,先设定Control Register再设定Data Register
	//Realchip设定padier=1/0为1表示数字端口/为0表示模拟端口
	pa =	0b_0001_1000;				//0:低电平		1:高电平(输出模式下，输出低时自动关上拉)
	pac =	0b_0001_1000;				//0:输入		1:输出
	pa =	0b_0001_1000;				//0:低电平		1:高电平(输出模式下，输出低时自动关上拉)
	paph =	0b_0100_0000;				//0:关上拉		1:开上拉
   $ padier	0b_0100_0000;				//0:模拟端口	1:数字端口
	
	pb =	0b_1000_0000;				//0:低电平		1:高电平(输出模式下，输出低时自动关上拉)
	pbc =	0b_1000_0000;				//0:输入		1:输出
	pb =	0b_1000_0000;				//0:低电平		1:高电平(输出模式下，输出低时自动关上拉)
	pbph =	0b_0000_0000;				//0:关上拉		1:开上拉
  $ pbdier	0b_0000_0010;				//0:模拟端口	1:数字端口

endm

i_o_sleep_set macro
	//-----------------------IO口配置-----------------------//
	//多个FPPA共享一个IO,并且要作Input/Output切换,先设定Control Register再设定Data Register
	//Realchip设定padier=1/0为1表示数字端口/为0表示模拟端口
	pa =	0b_0001_1000;				//0:低电平		1:高电平(输出模式下，输出低时自动关上拉)
	pac =	0b_0000_0000;				//0:输入		1:输出
	pa =	0b_0001_1000;				//0:低电平		1:高电平(输出模式下，输出低时自动关上拉)
	paph =	0b_0100_0000;				//0:关上拉		1:开上拉
   $ padier	0b_0100_0000;				//0:模拟端口	1:数字端口
	
	pb =	0b_1000_0000;				//0:低电平		1:高电平(输出模式下，输出低时自动关上拉)
	pbc =	0b_0000_0000;				//0:输入		1:输出
	pb =	0b_1000_0000;				//0:低电平		1:高电平(输出模式下，输出低时自动关上拉)
	pbph =	0b_0000_0000;				//0:关上拉		1:开上拉
  $ pbdier	0b_0000_0010;				//0:模拟端口	1:数字端口

endm

clr_ram macro
	//-----------------------RAM  清0-----------------------//
	system_ram = _sys(RAM_SIZE) - 1;
	a = 0;								//清RAM耗时短些
	do
	{
		*system_ram = a;
	}while(--system_ram$0);				//$0只操作低字节，节省ROM和缩短耗时
endm

timer_set macro
	//-----------------------定时配置-----------------------//
	system_ram = 32768 - 100 + 4;			
	stt16 system_ram;					//向上计数(也可以不赋初值)
//	$ t16m stop;
	$ t16m IHRC, /16, bit15;			//定时器时钟源：	STOP, SYSCLK, X, PA4_F, IHRC, EOSC, ILRC, PA0_F
										//分频：			/1, /4, /16, /64
										//计数器中断发生位：bit8, bit9, bit10, bit11, bit12, bit13, bit14, bit15
	INTEN.AD = 0;//不带AD的注释掉
	INTEN.PB0 = 0;//150不带pb
	INTEN.PA0 = 0;
	INTRQ = 0;
	INTEN.T16 = 1;						//开t16中断
	engint;								//开总中断	
endm


pwm_set macro
	//-----------------------PWM 配置-----------------------//
	tm2ct = 0b_0000_0000;				//tm2计数器清0
	tm2b  = 0b_0000_0000;				//tm2上限寄存器，pwm占空比 = (tm2b+1)/(256或64)
	tm2s  = 0b_0110_0000;				//tm2分频寄存器
												//bit7：	0:8位，1:6位
												//bit6-5:	/1，/4，/16，/64
												//bit4-0:	时钟分频器
	tm2c  = 0b_0000_0000;				//tm2控制寄存器
												//bit7-4:|	DIS，	SYSCLK，IHRC，	EOSC，	ILRC，	X，		X，	X（时钟选择）
												//		 |	PA0↑，	PA0↓，	PB0↑，	PB0↓，	PA4↑，	PA4↓，	X，	X
												//bit3-2:	DIS，	PB2，	PA3，	PB4（输出选择）
												//bit1:		周期模式，	PWM模式（模式选择）
												//bit0:		启用，		禁用（反极性输出选择）


//	tm3ct = 0b_0000_0000;				//tm3计数器清0
//	tm3b  = 0b_0001_1111;				//tm3上限寄存器，pwm占空比 = (tm3b+1)/(256或64)
//	tm3s  = 0b_0010_0000;				//tm3分频寄存器
												//bit7：	0:8位，1:6位
												//bit6-5:	/1，/4，/16，/64
												//bit4-0:	时钟分频器
//	tm3c  = 0b_0000_0000;				//tm3控制寄存器
												//bit7-4:|	DIS，	SYSCLK，IHRC，	EOSC，	ILRC，	X，		X，	X（时钟选择）
												//		 |	PA0↑，	PA0↓，	PB0↑，	PB0↓，	PA4↑，	PA4↓，	X，	X
												//bit3-2:	DIS，	PB5，	PB6，	PB7（输出选择）
												//bit1:		周期模式，	PWM模式（模式选择）
												//bit0:		启用，		禁用（反极性输出选择）
endm

ad_set macro
	//-----------------------ADC 配置-----------------------//
#if	ICE					//仿真
	$ adcm 12BIT,/8;	
    $ adcrgc VDD;
    $ adcc enable, PB.7;
	AD_Start = 1;		//开始		

#else					//实际
//    $ adcm 12BIT,/4;		//12bit AD IC
//    $ adcrgc VDD, ADC_BG;	
//    $ adcc enable, ADC;

//	$ adcm 8BIT,/1;			//271
//	$ ADCRHC VDD;
//    $ adcc enable, BANDGAP;

    $ adcm 12BIT,/4;		//363
//    $ adcrgc VDD;	
    $ adcc Enable, BANDGAP;

	AD_Start = 1;		//开始		
#endif
endm

sleep_set macro
	$ adcc disable;					//关闭AD
	$ t16m stop;					//定时器停止
	.disgint;						//关总中断
	i_o_sleep_set
	.clkmd = 0xF4;					//掉电模式前，系统时钟先切换到ILRC，关看门狗
	nop;
	nop;
	nop;
	nop;
	misc = 0x20;					//开唤醒快速启动
	.clkmd.4 = 0;					//关高频振荡
	stopsys;
	nop;
	nop;
	nop;
	nop;
	.clkmd.4 = 1;					//开高频振荡
	nop;
	nop;
	nop;
	nop;
//	.clkmd = 0x1C;					//跑1M
	.clkmd = 0x3C;					//跑2M
//	.clkmd = 0x34;					//跑8M
	misc = 0x00;					//关唤醒快速启动
	.wdreset;
	.clkmd.En_WatchDog = 1;			//看门狗使能
//	clr_ram
	i_o_set
	ad_set
	timer_set
//	$ padier	0b_0001_0001;		//0:关闭唤醒	1:开启唤醒
//	$ pbdier	0b_0100_0000;		//0:关闭唤醒	1:开启唤醒
	engint;							//开总中断
endm
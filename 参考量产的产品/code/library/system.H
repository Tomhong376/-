
#define vdd_degree01			360//0和1等级分界线
#define vdd_degree12			400//1和2等级分界线
//#define vdd_degree23			380//2和3等级分界线



#define VDD_low_data			320	//3.1V低保电压点
#define VDD_full_data 			412	//4.18V充满电压点
#define smoke_time_up_data		2000//*5mS = 吸烟超时时间长度
#define pre_heat_time_up_data	3000//*5mS = 预热时间长度
#define charge_time_up_data		10800//单位是秒，总共3小时，充电强制判满时间长度
#define	blink_lock				3//开机锁机提示闪烁次数
#define blink_get_charge		5//充电连接提示闪烁次数
#define blink_off_charge		5//充电断开提示闪烁次数
#define blink_low				15//低压保护提示闪烁次数
#define blink_smoking_low		0//吸烟过程中的低压保护提示闪烁次数
#define blink_full				0//充满提示闪烁次数
#define blink_overload			3//短路保护提示闪烁次数
#define blink_smoke_time_up		3//吸烟超时提示闪烁次数
#define blink_poweron			3//上电提示闪烁次数
#define blink_lowload			3//无载或低负载闪烁次数
#define blink_degree_select		3//档位选择闪灯次数

#define mos_duty_max			200//mos管pwm最大占空比
#define led_duty_max			100

#define sleep_delay_time 		161	//*5ms延时0.8S睡眠
#define show_vdd_delay_time		160//*5ms延时0.8S进行电量显示

extern byte vdd_degree;
extern byte lowload_delay_count;
extern byte V_out_degree;
extern byte char_disconnect_check_delay;
extern byte charge_disconnect_count_fast;

extern byte r_system_flag;
extern bit flag_sleep;//			:r_system_flag.0	
extern bit flag_mos;//				:r_system_flag.1//当前的mos是否打开，决定过载保护是否要工作
extern bit flag_smoking;//			:r_system_flag.2
extern bit flag_overload;//			:r_system_flag.3
extern bit flag_full;//				:r_system_flag.4
extern bit flag_charge;//			:r_system_flag.5
extern bit flag_charge_ready;//		:r_system_flag.2

extern bit flag_poweron_test_full;//:r_system_flag1.1
extern bit flag_VDD_charging;//:r_system_flag1.2
extern bit flag_finish_show_vdd;//:r_system_flag1.3
extern bit flag_show_vdd;//:r_system_flag1.4
extern bit flag_lowload;//:r_system_flag1.5
extern bit flag_open_mos;//:r_system_flag1.6
extern bit flag_pre_heat;//:r_system_flag1.7

extern bit flag_char_disconn_check;//	:r_system_flag2.0	


void system_deal(void);
void charge_scan(void);
void charge_check_disconnect(void);
void protect_check_deal(void);
void check_low(void);
void check_full(void);
void p_check_out0(void);
void p_check_io_in(void);
void p_check_ad_in(void);

void mos_on(void);
void mos_off(void);


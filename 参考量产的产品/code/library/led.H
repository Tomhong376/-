


#define led_duty_max	mos_duty_max
extern byte led_blink_count;
extern byte led_blink_delay;
extern byte led_blink_half_circle;
extern byte led_duty,led_r_duty,led_g_duty,led_b_duty;
extern byte led_count,led_delay_count,led_turn;

extern bit flag_blink_force;
extern bit flag_blink_slow;

extern bit flag_led;//			:r_led_flag.1;
extern bit flag_led_w;//			:r_led_flag.2;
extern bit flag_led_b;//			:r_led_flag.3;
extern bit flag_led_r;//			:r_led_flag.4;
extern bit flag_led_wbr;//			:r_led_flag.5;
extern bit flag_led_gb;//			:r_led_flag.6;
extern bit flag_led_g;//			:r_led_flag1.1;
extern bit flag_poweron_blink;//	:r_led_flag1.2;



void led_output(void);//µÆÊä³öº¯Êý
void led_on(void);
void led_off(void);
void clear_led_select(void);
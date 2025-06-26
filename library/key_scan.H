

//------------------------参数定义------------------------//
#define key_delay_time		6
#define double_click_delay_time 60
#define long_key_time		394


extern byte key_value;
extern word double_click_delay_count, long_key_count, key_count;

extern byte r_key_flag;
extern bit flag_key;//				:r_key_flag.0
extern bit flag_key_press;//		:r_key_flag.1
extern bit flag_5click;//			:r_key_flag.2
extern bit flag_key_active;//		:r_key_flag.3
extern bit flag_3click;//			:r_key_flag.4
extern bit flag_2click;//			:r_key_flag.5


//-------------------------客制变量-------------------------//
extern byte key_count1, key_count2, key_count3, key_count4;
extern bit flag_key1;//			:r_key_flag.2
extern bit flag_key2;//			:r_key_flag.3
extern bit flag_key3;//			:r_key_flag.4
extern bit flag_key4;//			:r_key_flag.5



//-------------------------函数声明-------------------------//
void key_scan_click(void);			//单击
void key_scan_c_dc(void);			//单击 + 双击
void key_scan_c_lp(void);			//单击 + 长按
void key_scan_dc_lp(void);			//双击 + 长按
void key_scan_c_dc_lp(void);		//单击 + 双击 + 长按

	
//-------------------------客制函数-------------------------//
void key_scan_c_5c(void);			//单击 + 5连击
void reset_5click(void);			//重置按键扫描

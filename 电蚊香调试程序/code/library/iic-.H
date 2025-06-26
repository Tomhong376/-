
#define iic_gnd_address 0xC0
#define iic_vdd_address 0xC4
#define iic_my_address_offset 0xA0


extern byte iic_my_address, iic_bit_count, iic_data, iic_byte_count, iic_temp;
extern byte r_iic_flag;

extern bit flag_iic_receive_ok;//	:r_iic_flag.0
extern bit flag_iic_error;//		:r_iic_flag.1


void iic_standby(void);
void delay_1us(void);


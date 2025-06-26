
#define iic_gnd_address 0xC0
#define iic_vdd_address 0xC4
#define iic_my_address_offset 0xA0


extern byte iic_my_address, iic_bit_count, iic_data, iic_byte_count, iic_temp, data[4];
extern byte r_iic_flag;

extern bit flag_iic_receive_ok;//	:r_iic_flag.0
extern bit flag_iic_error;//		:r_iic_flag.1
extern bit flag_iic_error;//		:r_iic_flag.2
extern bit flag_iic_data_error;//	:r_iic_flag.3


void iic_standby(void);
void delay_1us(void);
void iic_host(void);
void iic_host_start(void);
void iic_host_stop(void);
void iic_host_send_byte(void);
void iic_host_get_ack(void);

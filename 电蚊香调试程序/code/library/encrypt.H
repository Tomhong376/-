

#define key0	0x84
#define key1	0xab
#define key2	0x9c
#define key3	0x6a
#define key4	0x72
#define key5	0x9d
#define key6	0x3b
#define key7	0x2c
#define key8	0xba
#define key9	0xb1
#define key10	0x8d
#define key11	0x9a
#define key12	0xcd
#define key13	0x64
#define key14	0xb1
#define key15	0x8b

extern byte F;					//随机数
extern byte data[8];			//iic收到的随机数
extern byte E[8];
extern byte ck235_des_data[10];
extern byte encrypt_count0, encrypt_count1, encrypt_count2;
extern byte encrypt_temp0, encrypt_temp1, encrypt_temp2, encrypt_temp3;

extern byte r_encrypt_flag;
extern bit flag_encrypt_ok;//	:r_encrypt_flag.0



void En_Crypt(void);
void key(void);	//密钥查表
void bitreverse(void);//位反转

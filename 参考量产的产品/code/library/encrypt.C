
#include "../define.H"


//byte F;					//随机数

byte E[8];
byte data[8];			//iic收到的随机数
byte ck235_des_data[10];
byte encrypt_count0, encrypt_count1, encrypt_count2;
byte encrypt_temp0, encrypt_temp1, encrypt_temp2, encrypt_temp3;

byte r_encrypt_flag;
bit flag_encrypt_ok			:r_encrypt_flag.0


void En_Crypt(void)
{
//-------------------------------测试
/*	flag_iic_receive_ok = 1;
	F = 88;
	data[0] = 0x51; 
	data[1] = 0x52; 
	data[2] = 0x53; 
	data[3] = 0x55; 
	data[4] = 0x56; 
	data[5] = 0x57; 
	data[6] = 0x58; 
	data[7] = 0x59; 

*/
//-------------------------------测试

	
	if(flag_iic_receive_ok)
	{
		//------------------------------------第一步：异或
		encrypt_count0 = 8;
		while(encrypt_count0--)
		{
			point = data + encrypt_count0;
			encrypt_temp0 = *point;		//取得data值

			a = encrypt_count0;
			key();
			encrypt_temp0 ^= a;				//异或key[i]
			a = encrypt_count0 + 8;
			key();
			encrypt_temp0 ^= a;				//异或key[i+8]

			point = E + encrypt_count0;
			a = encrypt_temp0 ^ F;			//异或F
			*point = a;			
		}
		.wdreset;
		//------------------------------------第二步：字节反转
		encrypt_count0 = 4;
		while(encrypt_count0--)
		{
			point = E + encrypt_count0;
			encrypt_temp0 = *point;		//buf = E[i];
			point = E + 7-encrypt_count0;
			encrypt_count1 = *point;	
			*point = encrypt_temp0;		//E[7-i] = buf;
			point = E + encrypt_count0;
			*point = encrypt_count1;	//E[i] = E[7-i];
		}
		.wdreset;
		//------------------------------------第三步：位反转
		encrypt_count0 = 8;
		while(encrypt_count0--)
		{
			point = E + encrypt_count0;
			encrypt_temp0 = *point;	
			bitreverse(); 
			*point = encrypt_temp0;
		}
		.wdreset;
		//------------------------------------第四步：F插值
		ck235_des_data[8] = (E[7]<<1) | (F&0x01);
		encrypt_count0 = 1;			//i
		encrypt_count1 = 7;			//j
		while(encrypt_count0 < 8)
		{
			point = E + encrypt_count0 - 1;
			encrypt_temp0 = *point;
			encrypt_count2 = 9 - encrypt_count0;//9-i
			while(encrypt_count2)
			{
				encrypt_temp0 <<= 1;
				encrypt_count2 --;
			}

			point = E + encrypt_count0;
			encrypt_temp1 = *point;
			encrypt_count2 = encrypt_count0;	//i
			while(encrypt_count2)
			{
				encrypt_temp1 >>= 1;
				encrypt_count2 --;
			}

			encrypt_temp0 |= encrypt_temp1;		//(E[i-9]<<(9-i))|(E[i]>>i)

			encrypt_temp1 = 0x01;
			encrypt_count2 = encrypt_count1;
			while(encrypt_count2)
			{
				encrypt_temp1 <<= 1;
				encrypt_count2 --;
			}
			encrypt_temp1 &= F;					//F&(0x01<<j)
			encrypt_temp0 |= encrypt_temp1;		//(E[i-9]<<(9-i))|(E[i]>>i)|(F&(0x01<<j))

			point = ck235_des_data + encrypt_count0;
			*point = encrypt_temp0;		//ck235_des_data[i] = (E[i-9]<<(9-i))|(E[i]>>i)|(F&(0x01<<j))

			encrypt_count0 ++;
			encrypt_count1 --;
		}
		.wdreset;		
		//------------------------------------第五步：添加E[0]与F
		ck235_des_data[0] = E[0];
		ck235_des_data[9] = F;

		flag_encrypt_ok = 1;
	}
	flag_iic_receive_ok = 0;

}


void key(void)	//密钥查表
{
	a ++;
	pcadd	a;
	ret		key0;
	ret		key1;
	ret		key2;
	ret		key3;
	ret		key4;
	ret		key5;
	ret		key6;
	ret		key7;
	ret		key8;
	ret		key9;
	ret		key10;
	ret		key11;
	ret		key12;
	ret		key13;
	ret		key14;
	ret		key15;
}

void bitreverse(void)
{
	encrypt_temp0 = ((encrypt_temp0<<4)&0xF0) | ((encrypt_temp0>>4)&0x0F);
	encrypt_temp0 = ((encrypt_temp0<<2)&0xCC) | ((encrypt_temp0>>2)&0x33);
	encrypt_temp0 = ((encrypt_temp0<<1)&0xAA) | ((encrypt_temp0>>1)&0x55);
}
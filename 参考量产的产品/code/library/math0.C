

byte math_bytes[5], cnt, math_temp, math_byte;
word math_word, math_word_residual;
eword math_eword, math_eword_residual;
dword math_dword;


/*
void	EWord_Mul_Word (void)
{	//math_bytes[5] = math_eword * math_word;
	math_bytes[4]=	0;
	math_bytes[3]=	0;
	math_bytes[2]=	0;
	cnt	=	16;

	do
	{
		math_word >>= 1;
		if (CF)
		{
			math_bytes[2] += math_eword$0; 
			a = math_eword$1;
			addc math_bytes[3] a
			a = math_eword$2;
			addc math_bytes[4] a
		}
		math_bytes[4]	>>>=	1;
		math_bytes[3]	>>>=	1;
		math_bytes[2]	>>>=	1;
		math_bytes[1]	>>>=	1;
		math_bytes[0]	>>>=	1;
	} while (--cnt);
}

*/


/*

void	Math_5bytes_Div_EWord (void)
{	//	math_bytes[5] / math_eword[E]	=	math_bytes[5] * math_eword[E] + math_eword_residual[E]
	cnt		=	24;
	math_eword_residual	=	0;
	goto	div_shf;

	do
	{
		math_eword_residual	<<<=	1;
		math_temp	<<<=	1;
		cnt++;
		math_eword_residual	-=	math_eword;

		if (!math_temp.0 && CF)
		{
			math_eword_residual	+=	math_eword;
			CF	=	0;
		}
		else	CF	=	1;
div_shf:
		math_bytes[0] <<<= 1;
		math_bytes[1] <<<= 1;
		math_bytes[2] <<<= 1;
		math_bytes[3] <<<= 1;
		math_bytes[4] <<<= 1;
	} while (!cnt.6);
}
*/





void	Math_Eword_Div_Word (void)
{	//	math_eword[E] / math_word[W]	=	math_eword[E] * math_word[W] + math_word_residual[W]
//	BYTE	cnt, math_temp;
	cnt		=	8;
	math_word_residual	=	0;

	do
	{
		math_eword	<<=		1;
		math_word_residual	<<<=	1;
		math_temp		<<<=	1;
		cnt++;
		A	=	(math_word_residual - math_word) >> 8;

		if (math_temp.0 || ! CF)
		{
			math_word_residual$1	=	A;
			math_word_residual$0	-=	math_word$0;
			math_eword.0	=	1;
		}
	} while (! cnt.5);
}



/*

void	Math_Word_Mul_Byte (void)
{	//	math_eword[E]	=	math_byte[B] * math_word[W]
	math_eword$1	=	0;
	math_eword$2	=	0;

	cnt	=	8;

	do
	{
		math_byte	>>=	1;
		if (CF)
		{
			math_eword	+=	(math_word << 8);
		}
		math_eword	>>>=	1;
	} while (--cnt);
}
*/




void	Math_EWord_Mul_Byte (void)
{	//	math_dword[D]	=	math_byte[B] * math_eword[E]
	math_dword$3	=	0;
	math_dword$2	=	0;
	math_dword$1	=	0;
//	BYTE	cnt;
	cnt	=	8;

	do
	{
		math_byte	>>=	1;
		if (CF)
		{
			math_dword	+=	(math_eword << 8);
		}
		math_dword	>>>=	1;
	} while (--cnt);
}





void	Math_DWord_Div_EWord (void)
{	//	math_dword[D] / math_eword[E]	=	math_dword[D] * math_eword[E] + math_eword_residual[E]
	BYTE	div_cnt, div_tmp;
	div_cnt		=	0;
	math_eword_residual	=	0;

	do
	{
		math_dword	<<=		1;
		math_eword_residual	<<<=	1;
		div_tmp		<<<=	1;
		div_cnt++;
		A	=	(math_eword_residual - math_eword) >> 16;

		if (div_tmp.0 || !CF)
		{
			math_eword_residual	=	(A << 16) | ((math_eword_residual - math_eword) & 0xFFFF);
			math_dword.0	=	1;
		}
	} while (! div_cnt.5);
}





DWORD	math_dword1;
EWORD	math_eword1, math_residual_eword1;
WORD	math_word1, math_residual_word1;
BYTE	math_byte1, cnt1, math_temp1;

void Word_Mul_Byte1(void)
{	//	math_eword1[E]	=	math_byte1[B] * math_word1[W]
	math_eword1$1	=	0;
	math_eword1$2	=	0;
	cnt1	=	8;

	do
	{
		math_byte1	>>=	1;
		if (CF)
		{
			math_eword1	+=	(math_word1 << 8);
		}
		math_eword1	>>>=	1;
	} while (--cnt1);
}



void EWord_Div_Byte1(void)
{	//	math_eword1[E] / math_byte1[B]	=	math_eword1[E] * math_byte1[B] + math_residual_word1$0[B]
	cnt1	=	8;
	math_residual_word1$0	=	0;
	goto	div_shf;

	do
	{
		slc		math_residual_word1$0;
		slc		math_temp1;
		cnt1++;
		math_residual_word1$0	-=	math_byte1;

		if (!math_temp1.0 && CF)
		{
			math_residual_word1$0	+=	math_byte1;
			CF	=	0;
		}
		else	CF	=	1;
div_shf:
		math_eword1	<<<=	1;
	} while (! cnt1.5);
}



void DWord_Div_Word1(void)
{	//	math_dword1[D] / math_word1[W]	=	math_dword1[D] * math_word1[W] + math_residual_word1[W]
	cnt1		=	0;
	math_residual_word1	=	0;
	goto	div_shf;

	do
	{
		math_residual_word1	<<<=	1;
		math_temp1		<<<=	1;
		cnt1++;
		math_residual_word1	-=	math_word1;

		if (!math_temp1.0 && CF)
		{
			math_residual_word1	+=	math_word1;
			CF	=	0;
		}
		else	CF	=	1;
div_shf:
		math_dword1	<<<=	1;
	} while (! cnt1.5);
}




void	DWord_Div_EWord1(void)
{	//	math_dword1[D] / math_eword1[E]	=	math_dword1[D] * math_eowrd1[E] + math_residual_eword1[E]
	cnt1	=	0;
	math_residual_eword1	=	0;

	do
	{
		math_dword1	<<=		1;
		math_residual_eword1	<<<=	1;
		math_temp1		<<<=	1;
		cnt1 ++;
		A	=	(math_residual_eword1 - math_eword1) >> 16;

		if (math_temp1.0 || !CF)
		{
			math_residual_eword1 =	(A << 16) | ((math_residual_eword1 - math_eword1) & 0xFFFF);
			math_dword1.0	=	1;
		}
	} while (! cnt1.5);
}




void	EWord_Mul_Byte1 (void)
{	//	math_dword1[E]	=	math_eword1[B] * math_byte1[W]
	math_dword1$3	=	0;
	math_dword1$2	=	0;
	math_dword1$1	=	0;
	cnt1	=	8;

	do
	{
		math_byte1	>>=	1;
		if (CF)
		{
			math_dword1	+=	(math_eword1 << 8);
		}
		math_dword1	>>>=	1;
	} while (--cnt1);
}




extern byte math_bytes[5], cnt, math_temp,math_byte;
extern word math_word;
extern eword math_eword, math_eword_residual;
extern dword math_dword;


//void	EWord_Mul_Word (void);
//void	Math_5bytes_Div_EWord (void);
void	Math_Eword_Div_Word(void);
void	Math_Word_Mul_Byte (void);
void	Math_Dword_Div_EWord(void);
void	Math_Eword_Mul_Byte(void);
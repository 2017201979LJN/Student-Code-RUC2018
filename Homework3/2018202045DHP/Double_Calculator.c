#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include "summon.h"

#define NUM_OF_BYTES 8
#define LEN_OF_BYTES 8
#define POWER_TOP_BYTE 7
#define POWER_TOP_BIT 6
#define POWER_BOT_BYTE 6
#define POWER_BOT_BIT 4
#define FRAC_TOP_BYTE 6
#define FRAC_TOP_BIT 3
#define NUM_OF_POWER_BITS 11
#define NUM_OF_FRAC_BITS 52
#define LEN_OF_NUM 280
#define BIAS 1023
#define TARGET_CASES 100
#define RAW_LINE 800
#define DOT 400
#define RAW_LINE_2 5000
#define DOT_2 2500
#define ENTER 13
#define MAX_NUMBERS 1233
#define DECIMAL_PART 30

struct Double {
	unsigned char bytes[NUM_OF_BYTES];
};

int operator_[MAX_NUMBERS], operator_top;
int reverse_polish[MAX_NUMBERS], top;
int operator_polish[MAX_NUMBERS];
struct Double number_polish[MAX_NUMBERS];

struct Double NaN, INFINITY, ZERO;

struct Double Function_Addition (struct Double x1, struct Double x2);
struct Double Function_Subtraction (struct Double x1, struct Double x2);

//This function uses pointer to copy double to simulator

struct Double Function_Transform_Double_to_Bytes (double x) {
	struct Double Temp;
	unsigned char * start = (unsigned char *) &x;
	for(int i = 0; i < NUM_OF_BYTES; i++) 
		Temp.bytes[i] = start[i];
	return Temp;
}

//This function shows data in the simulator

void Show_Double (struct Double x) {
//	for(int i = 0; i < NUM_OF_BYTES; i++){
//		printf("Ox%.2x\n", x.bytes[i]);
//	}
	for(int i = NUM_OF_BYTES - 1; i >= 0; i--) {
		for(int j = LEN_OF_BYTES - 1; j >= 0; j--) {
			printf("%d",((1 << j) & x.bytes[i]) > 0);
		}
		printf("\n");
	}
	printf("\n");
}

//This function uses pointer to copy simulator to double

double Function_Transform_Bytes_to_Double (struct Double x) {
	double Temp;
	unsigned char * start = (unsigned char *) &Temp;
	for(int i = 0; i < NUM_OF_BYTES; i++)
		start[i] = x.bytes[i];
//	printf("%.320lf\n\n", Temp);
	return Temp;
}

//This function aims at extract the power2 of double simulator

int Lift_Power (struct Double x) {
	int Temp = 0;
	for(int i = POWER_TOP_BYTE; i >= POWER_BOT_BYTE; i--)
		for(int j = ((i == POWER_TOP_BYTE) ? POWER_TOP_BIT : (LEN_OF_BYTES - 1)); j >= ((i == POWER_BOT_BYTE) ? POWER_BOT_BIT : 0); j--)
			Temp = (Temp << 1) | ((x.bytes[i] & (1 << j)) > 0);
	return Temp;
}

//This function can finish the addition operator

struct Double Function_Addition (struct Double x1, struct Double x2){
	int flag = 0;
	struct Double Temp;
	//operating symbol bit

	if ((x1.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1))) && (x2.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1)))) {
		x1.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		x2.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		flag = 1;
//		printf("We find both x1 and x2 < 0\n");
	}
	else if ((x2.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1))) && !(x1.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1)))) {
		x2.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
//		printf("We find x2 < 0\n");
		return Function_Subtraction (x1, x2);
	}
	else if ((x1.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1))) && !(x2.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1)))) {
		x1.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
//		printf("We find x1 < 0\n");
		return Function_Subtraction (x2, x1);
	}
	else {
		flag = 0;
	}
	
	//separating power and number
	int power1, power2;
	power1 = Lift_Power (x1);
	power2 = Lift_Power (x2);
	
//	printf("%d\n%d\n", power1, power2);
	if(power1 < power2) {
		Temp = x1;
		x1 = x2;
		x2 = Temp;
		power1 ^= power2 ^= power1 ^= power2;
	}
	if(power1 - power2 > NUM_OF_FRAC_BITS + 5) {
		if(flag == 1) x1.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		return x1;
	}
	char number1[LEN_OF_NUM], number2[LEN_OF_NUM], answer[LEN_OF_NUM];
	memset (number1, 0, sizeof (number1));
	memset (number2, 0, sizeof (number2));
	memset (answer, 0, sizeof (answer));
	int cnt = 1;
	if(power1 == 0) power1 = 1;
	else number1[1] = 1;
	for (int i = FRAC_TOP_BYTE; i >= 0; i--)
		for (int j = (i == FRAC_TOP_BYTE) ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1); j >= 0; j--)
			number1[++cnt] = (x1.bytes[i] & (1 << j)) > 0;

			//	for(int i = 0; i <= 100; i++) printf("%d", number1[i]);
			//	printf("\n");

	cnt = 1;
	if(power2 == 0) power2 = 1;
	else number2[power1 - power2 + 1] = 1;
	for (int i = FRAC_TOP_BYTE; i >= 0; i--)
		for (int j = (i == FRAC_TOP_BYTE) ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1); j >= 0; j--)
			number2[power1 - power2 + (++cnt)] = (x2.bytes[i] & (1 << j)) > 0;

			//	for(int i = 0; i <= 100; i++) printf("%d", number2[i]);
			//	printf("\n");
	
	//UNCOMPUTABLE NUMBER CHECKING
	if(power1 == BIAS * 2 + 1) 
		for(int i = 2; i <= NUM_OF_FRAC_BITS + 1; i++)
			if(number1[i] == 1){
//				printf("NaN denied\n");
				return x1;
			}

	if(power2 == BIAS * 2 + 1) 
		for(int i = 2; i <= NUM_OF_FRAC_BITS + 1; i++)
			if(number2[i] == 1){
//				printf("NaN denied\n");
				return x2;
			}
	
	if(power1 == BIAS * 2 + 1) {
//		printf("inf denied\n");
		return x1;
	}

	if(power2 == BIAS * 2 + 1) {
//		printf("inf denied\n");
		return x1;
	}


	//adding
	int carry = 0;
	for(int i = LEN_OF_NUM - 1; i >= 0; i--){
		carry += number1[i] + number2[i];
		if(carry >= 2) {
			answer[i] = carry - 2;
			carry = 1;
		}
		else {
			answer[i] = carry;
			carry = 0;
		}
	}

	//the rest of this function is refilling and returning
	int starting;
	memset(Temp.bytes, 0, sizeof(Temp.bytes));
	
	//rounding
	if(power1 == 1 && answer[1] == 0 && answer[0] == 0) power1 = 0;
	if(answer[0] == 0) starting = NUM_OF_FRAC_BITS + 2;
	else starting = NUM_OF_FRAC_BITS + 1;
	int ifcarry = 0;
	if(answer[starting] == 1) {
		for(int	i = starting + 1; i < LEN_OF_NUM; i++) 
			if(answer[i] == 1) ifcarry = 1;
		if(ifcarry == 0 && answer[starting - 1] == 1) 
			ifcarry = 1;
	}
	
	if(ifcarry) {
		carry = 1;
		for(int i = starting - 1; i >= 0; i--) {
			carry += answer[i];
			if(carry == 2) {
				answer[i] = 0;
				carry = 1;
			}else {
				answer[i] = carry;
				break;
			}
		}
	}

	//filling
	if(flag == 1) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
	if(answer[0] == 1) power1++, starting = 1;
	else starting = 2;
	if(power1 == BIAS * 2 + 1){
		Temp = INFINITY;
		if(flag) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		return Temp;
	}
	for(int i = FRAC_TOP_BYTE; i >= 0; i--)
		for(int j = (i == FRAC_TOP_BYTE ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1)); j >= 0; j--)
			Temp.bytes[i] ^= answer[starting++] << j;
	for(int i = POWER_BOT_BYTE; i <= POWER_TOP_BYTE; i++)
		for(int j = (i == POWER_BOT_BYTE ? POWER_BOT_BIT : 0); j <= (i == POWER_TOP_BYTE ? POWER_TOP_BIT : (LEN_OF_BYTES - 1)); j++){
			Temp.bytes[i] ^= (power1 & 1) << j;
			power1 >>= 1;
		}
	
	return Temp;
	
}

//This function can finish the subtraction operation

struct Double Function_Subtraction (struct Double x1, struct Double x2){
	int flag = 0;
	struct Double Temp;
	//operating symbol bit

	if ((x1.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1))) && (x2.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1)))) {
		x1.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		x2.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		flag = 1;
	}
	else if ((x2.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1))) && !(x1.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1)))) {
		x2.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		return Function_Addition (x1, x2);
	}
	else if ((x1.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1))) && !(x2.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1)))) {
		x2.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		return Function_Addition (x1, x2);
	}
	else {
		flag = 0;
	}
	
	//separating power and number
	int power1, power2;
	power1 = Lift_Power (x1);
	power2 = Lift_Power (x2);
	
//	printf("%d\n%d\n", power1, power2);
	if(power1 < power2) {
		Temp = x1;
		x1 = x2;
		x2 = Temp;
		power1 ^= power2 ^= power1 ^= power2;
		flag ^= 1;
	} else if(power1 == power2) {
		for(int i = FRAC_TOP_BYTE; i >= 0; i--) {
			for(int j = (i == FRAC_TOP_BYTE ? FRAC_TOP_BIT : LEN_OF_BYTES - 1); j >= 0; j--) {
				if((x1.bytes[i] & (1 << j)) > (x2.bytes[i] & (1 << j))) goto abd;
				else if((x1.bytes[i] & (1 << j)) == (x2.bytes[i] & (1 << j)));
				else {
					Temp = x1;
					x1 = x2;
					x2 = Temp;
					power1 ^= power2 ^= power1 ^=power2;
					flag ^= 1;
					goto abd;
				}
			}
		}
		abd:;
	}
	if(power1 - power2 > NUM_OF_FRAC_BITS + 5) {
		if(flag == 1) x1.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		return x1;
	}
	char number1[LEN_OF_NUM], number2[LEN_OF_NUM], answer[LEN_OF_NUM];
	memset (number1, 0, sizeof (number1));
	memset (number2, 0, sizeof (number2));
	memset (answer, 0, sizeof (answer));
	int cnt = 1;
	if(power1 == 0) power1 = 1;
	else number1[1] = 1;
	for (int i = FRAC_TOP_BYTE; i >= 0; i--)
		for (int j = (i == FRAC_TOP_BYTE) ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1); j >= 0; j--)
			number1[++cnt] = (x1.bytes[i] & (1 << j)) > 0;

//				for(int i = 0; i <= 100; i++) printf("%d", number1[i]);
//				printf("\n");

	cnt = 1;
	if(power2 == 0) power2 = 1;
	else number2[power1 - power2 + 1] = 1;
	for (int i = FRAC_TOP_BYTE; i >= 0; i--)
		for (int j = (i == FRAC_TOP_BYTE) ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1); j >= 0; j--)
			number2[power1 - power2 + (++cnt)] = (x2.bytes[i] & (1 << j)) > 0;



//				for(int i = 0; i <= 100; i++) printf("%d", number2[i]);
//				printf("\n");

	//UNCOMPUTABLE NUMBER CHECKING
	if(power1 == BIAS * 2 + 1) 
		for(int i = 2; i <= NUM_OF_FRAC_BITS + 1; i++)
			if(number1[i] == 1){
//				printf("NaN denied\n");
				return NaN;
			}

	if(power2 == BIAS * 2 + 1) 
		for(int i = 2; i <= NUM_OF_FRAC_BITS + 1; i++)
			if(number2[i] == 1){
//				printf("NaN denied\n");
				return NaN;
			}
	
	if(power1 == BIAS * 2 + 1 && power2 == BIAS * 2 + 1)
		return NaN;

	if(power1 == BIAS * 2 + 1) {
//		printf("inf denied\n");
		return x1;
	}

	if(power2 == BIAS * 2 + 1) {
//		printf("inf denied\n");
		return x2;
	}

	//subtracting
	int carry = 0;
	for(int i = LEN_OF_NUM - 1; i >= 0; i--){
		carry += number1[i];
		if(carry >= number2[i]) {
			answer[i] = carry - number2[i];
			carry = 0;
		}
		else {
			answer[i] = carry + 2 - number2[i];
			carry = -1;
		}
	}
	
//				for(int i = 0; i <= 100; i++) printf("%d", answer[i]);
//				printf("\n");
	//the rest of this function is refilling and returning
	int starting;
	memset(Temp.bytes, 0, sizeof(Temp.bytes));
	
	//rounding
	cnt = 1;
	for(cnt = 1; cnt <= NUM_OF_FRAC_BITS * 2 + 5; cnt++){
		if(power1 == 0) break;
		if(answer[cnt] == 0) power1--;
		if(answer[cnt] == 1) break;
	}
	if(power1 == 0) cnt--;
	
	int ifcarry = 0;
	if(answer[cnt + NUM_OF_FRAC_BITS + 1] == 1) {
		for(int	i = cnt + NUM_OF_FRAC_BITS + 2; i < LEN_OF_NUM; i++) 
			if(answer[i] == 1) ifcarry = 1;
		if(ifcarry == 0 && answer[cnt + NUM_OF_FRAC_BITS] == 1) 
			ifcarry = 1;
	}
	
	if(ifcarry) {
		carry = 1;
		for(int i = cnt + NUM_OF_FRAC_BITS; i >= 0; i--) {
			carry += answer[i];
			if(carry == 2) {
				answer[i] = 0;
				carry = 1;
			}else {
				answer[i] = carry;
				break;
			}
		}
	}
	
	int ifdemin = 1;
	for(int i = 1; i < LEN_OF_NUM; i++)
		if(answer[i] == 1) ifdemin = 0;
	if(ifdemin) power1 = 0;

	//filling
	if(flag == 1) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
	starting = cnt + 1;
	for(int i = FRAC_TOP_BYTE; i >= 0; i--)
		for(int j = (i == FRAC_TOP_BYTE ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1)); j >= 0; j--)
			Temp.bytes[i] ^= answer[starting++] << j;
	for(int i = POWER_BOT_BYTE; i <= POWER_TOP_BYTE; i++)
		for(int j = (i == POWER_BOT_BYTE ? POWER_BOT_BIT : 0); j <= (i == POWER_TOP_BYTE ? POWER_TOP_BIT : (LEN_OF_BYTES - 1)); j++){
			Temp.bytes[i] ^= (power1 & 1) << j;
			power1 >>= 1;
		}
	return Temp;
}

//This function can finish the multiplication operation

struct Double Function_Multiplication (struct Double x1, struct Double x2){
	int flag = 0;
	int is_zero1 = 1, is_zero2 = 1;
	struct Double Temp;
	//operating symbol bit

	if (x1.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1))) {
		x1.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		flag ^= 1;
	}
	if (x2.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1))) {
		x2.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		flag ^= 1;
	}

	//separating power and number
	int power1, power2;
	power1 = Lift_Power (x1);
	power2 = Lift_Power (x2);
	
//	printf("%d\n%d\n", power1, power2);

	char number1[LEN_OF_NUM], number2[LEN_OF_NUM], answer[LEN_OF_NUM];
	memset (number1, 0, sizeof (number1));
	memset (number2, 0, sizeof (number2));
	memset (answer, 0, sizeof (answer));
	int cnt = 1;
	for (int i = FRAC_TOP_BYTE; i >= 0; i--)
		for (int j = (i == FRAC_TOP_BYTE) ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1); j >= 0; j--)
			number1[++cnt] = (x1.bytes[i] & (1 << j)) > 0;
	if(power1 != 0) number1[1] = 1;
	for(int i = 0; i < LEN_OF_NUM; i++)
		if(number1[i] != 0) is_zero1 = 0;
//				for(int i = 0; i <= 100; i++) printf("%d", number1[i]);
//				printf("\n");

	cnt = 1;
	for (int i = FRAC_TOP_BYTE; i >= 0; i--)
		for (int j = (i == FRAC_TOP_BYTE) ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1); j >= 0; j--)
			number2[++cnt] = (x2.bytes[i] & (1 << j)) > 0;
	if(power2 != 0) number2[1] = 1;
	for(int i = 0; i < LEN_OF_NUM; i++)
		if(number2[i] != 0) is_zero2 = 0;

//				for(int i = 0; i <= 100; i++) printf("%d", number2[i]);
//				printf("\n");

	//UNCOMPUTABLE NUMBER CHECKING
	if(power1 == BIAS * 2 + 1) 
		for(int i = 2; i <= NUM_OF_FRAC_BITS + 1; i++)
			if(number1[i] == 1){
//				printf("NaN denied\n");
				return NaN;
			}

	if(power2 == BIAS * 2 + 1) 
		for(int i = 2; i <= NUM_OF_FRAC_BITS + 1; i++)
			if(number2[i] == 1){
//				printf("NaN denied\n");
				return NaN;
			}
	
	if(power1 == BIAS * 2 + 1 && is_zero2) {
//		printf("inf denied\n");
		return NaN;
	}

	if(power2 == BIAS * 2 + 1 && is_zero1) {
//		printf("inf denied\n");
		return NaN;
	}

	if(power1 == BIAS * 2 + 1 || power2 == BIAS * 2 + 1){
		Temp = INFINITY;
		if(flag) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		return Temp;
	}

	//multiplying
	int carry = 0;

	for(int i = 1; i <= NUM_OF_FRAC_BITS + 1; i++) {
		// add from i to i + LEN_OF_FRAC_BITS
		if(number1[i] == 1){
			carry = 0;
			for(int j = i + NUM_OF_FRAC_BITS; j >= 0; j--){
				carry += number2[j] + answer[j];
				if(carry >= 2) answer[j] = carry - 2, carry = 1;
				else answer[j] = carry, carry = 0;
			}
		}
		for(int j = i + NUM_OF_FRAC_BITS + 1; j >= i + 1; j--){
			number2[j] = number2[j - 1];
		}
		number2[i] = 0;
	}
	
//				for(int i = 0; i <= 100; i++) printf("%d", answer[i]);
//				printf("\n");
	//the rest of this function is refilling and returning
	int starting;
	memset(Temp.bytes, 0, sizeof(Temp.bytes));
	
	//rounding
	if(power1 == 0) power1 = 1;
	if(power2 == 0) power2 = 1;
	power1 = power1 + power2 - BIAS;
	cnt = 1;
	if(answer[0] == 1) cnt = 0, power1++; 
	if(power1 <= 0 && answer[0] == 1) cnt = power1 - 1, power1 = 0;
	else if(power1 <= 0 && answer[0] == 0) cnt = 1 + (power1 - 1), power1 = 0;
	for(; cnt <= NUM_OF_FRAC_BITS * 2 + 5; cnt++){
		if(power1 == 0) break;
		if(answer[cnt] == 0) power1--;
		if(power1 == 0) cnt--;
		if(answer[cnt] == 1) break;
	}
	if(cnt + NUM_OF_FRAC_BITS + 1 < 0) goto FINISHED_MULTIPLICATION;
	
	int ifcarry = 0;
	if(answer[cnt + NUM_OF_FRAC_BITS + 1] == 1) {
		for(int	i = cnt + NUM_OF_FRAC_BITS + 2; i < LEN_OF_NUM; i++) 
			if(answer[i] == 1) ifcarry = 1;
		if(ifcarry == 0 && answer[cnt + NUM_OF_FRAC_BITS] == 1) 
			ifcarry = 1;
	}
	
	if(ifcarry) {
		for(int i = cnt + NUM_OF_FRAC_BITS + 1; i < LEN_OF_NUM; i++)
			answer[i] = 0;
		carry = 1;
		for(int i = cnt + NUM_OF_FRAC_BITS; i >= 0; i--) {
			carry += answer[i];
			if(carry == 2) answer[i] = 0, carry = 1;
			else{ 
				answer[i] = carry;
				carry = 0;
				break;
			}
		}
	}

	if(carry == 1) {
		answer[0] = 1;
		if(cnt < -1) cnt++;
		else power1++;
	}
	if(power1 >= BIAS * 2 + 1){
		if(flag == 0) return INFINITY;
		else {
			Temp = INFINITY;
			Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		}
		return Temp;
	}
	//filling
	FINISHED_MULTIPLICATION:;
	if(flag) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
	starting = cnt + 1;
	for(int i = FRAC_TOP_BYTE; i >= 0; i--)
		for(int j = (i == FRAC_TOP_BYTE ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1)); j >= 0; j--){
			if(starting < 0) Temp.bytes[i] ^= 0 << j;
			else Temp.bytes[i] ^= answer[starting] << j;
			starting++;
		}
	for(int i = POWER_BOT_BYTE; i <= POWER_TOP_BYTE; i++)
		for(int j = (i == POWER_BOT_BYTE ? POWER_BOT_BIT : 0); j <= (i == POWER_TOP_BYTE ? POWER_TOP_BIT : (LEN_OF_BYTES - 1)); j++){
			Temp.bytes[i] ^= (power1 & 1) << j;
			power1 >>= 1;
		}
	return Temp;
}

//This function can finish the division operation

struct Double Function_Division (struct Double x1, struct Double x2){
	int flag = 0;
	int is_zero1 = 0, is_zero2 = 0;
	struct Double Temp;
	//operating symbol bit

	if (x1.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1))) {
		x1.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		flag ^= 1;
	}
	if (x2.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1))) {
		x2.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		flag ^= 1;
	}

	//separating power and moving number to the same step
	int power1, power2;
	power1 = Lift_Power (x1);
	power2 = Lift_Power (x2);

//	printf("%d\n%d\n", power1, power2);

	char number1[LEN_OF_NUM], number2[LEN_OF_NUM], answer[LEN_OF_NUM];
	int cnt = 1;
	memset (number1, 0, sizeof (number1));
	memset (number2, 0, sizeof (number2));
	memset (answer, 0, sizeof (answer));
	for (int i = FRAC_TOP_BYTE; i >= 0; i--)
		for (int j = (i == FRAC_TOP_BYTE) ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1); j >= 0; j--)
			number1[++cnt] = (x1.bytes[i] & (1 << j)) > 0;
	if(power1 != 0) number1[1] = 1;
	else {
		power1 = 1;
		int cnt = 1;
		for(; cnt < LEN_OF_NUM; cnt++, power1--)
			if(number1[cnt] == 1) break;
		if(cnt == LEN_OF_NUM) is_zero1 = 1;
		else {
			for(int i = 1; i <= NUM_OF_FRAC_BITS + 1; i++)
				number1[i] = number1[cnt + i - 1];
			for(int i = NUM_OF_FRAC_BITS + 2; i < LEN_OF_NUM; i++)
				number1[i] = 0;
		}
	}
//				for(int i = 0; i <= 100; i++) printf("%d", number1[i]);
//				printf("\n");
	
	cnt = 1;
	for (int i = FRAC_TOP_BYTE; i >= 0; i--)
		for (int j = (i == FRAC_TOP_BYTE) ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1); j >= 0; j--)
			number2[++cnt] = (x2.bytes[i] & (1 << j)) > 0;
	if(power2 != 0) number2[1] = 1;
	else {
		power2 = 1;
		int cnt = 1;
		for(; cnt < LEN_OF_NUM; cnt++, power2--)
			if(number2[cnt] == 1) break;
		if(cnt == LEN_OF_NUM) is_zero2 = 1;
		else {
			for(int i = 1; i <= NUM_OF_FRAC_BITS + 1; i++)
				number2[i] = number2[cnt + i - 1];
			for(int i = NUM_OF_FRAC_BITS + 2; i < LEN_OF_NUM; i++)
				number2[i] = 0;
		}
	}


//				for(int i = 0; i <= 100; i++) printf("%d", number2[i]);
//				printf("\n");

	//UNCOMPUTABLE NUMBER CHECKING
	if(power1 == BIAS * 2 + 1) 
		for(int i = 2; i <= NUM_OF_FRAC_BITS + 1; i++)
			if(number1[i] == 1){
//				printf("NaN denied\n");
				return NaN;
			}

	if(power2 == BIAS * 2 + 1) 
		for(int i = 2; i <= NUM_OF_FRAC_BITS + 1; i++)
			if(number2[i] == 1){
//				printf("NaN denied\n");
				return NaN;
			}
	
	if(is_zero2) {
		if(!is_zero1){
			Temp = INFINITY;
			if(flag) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
			return Temp;
		} else
			return NaN;
	}

	if(is_zero1) {
		Temp = ZERO;
		if(flag) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		return Temp;
	}

	if(power1 == BIAS * 2 + 1 && power2 == BIAS * 2 + 1){
		return NaN;
	}

	if(power1 == BIAS * 2 + 1) {
		Temp = INFINITY;
		if(flag) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		return Temp;
	}

	if(power2 == BIAS * 2 + 1) {
		Temp = ZERO;
		if(flag) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		return Temp;
	}
	//Dividing
	int carry = 0;

	for(int i = 1; i <= NUM_OF_FRAC_BITS * 4 + 5; i++) {
		int larger = 1;
		for(int j = i; j <= i + NUM_OF_FRAC_BITS; j++) {
			if(number1[j] > number2[j]) 
				break;
			if(number1[j] < number2[j]){
				larger = 0;
				break;
			}
		}
		if(number1[i - 1] > 0) larger = 1;
		if(larger){
			answer[i] = 1;
			for(int j = i + NUM_OF_FRAC_BITS; j >= i - 1; j--){
				carry += number1[j];
				if(carry >= number2[j]){
					number1[j] = carry - number2[j];
					carry = 0;
				} else {
					number1[j] = carry + 2 - number2[j];
					carry = -1;
				}
			}
		}
		for(int j = i + NUM_OF_FRAC_BITS + 1; j > i; j--){
			number2[j] = number2[j - 1];
		}
		number2[i] = 0;
	}
	
//				for(int i = 0; i <= 100; i++) printf("%d", answer[i]);
//				printf("\n");
	//the rest of this function is refilling and returning
	int starting;
	memset(Temp.bytes, 0, sizeof(Temp.bytes));
	
	//rounding
	power1 = power1 - power2 + BIAS;
	cnt = 1;
	if(answer[1] == 0) cnt = 2, power1 --;
	if(power1 <= 0) cnt += power1 - 1, power1 = 0;
	if(cnt + NUM_OF_FRAC_BITS + 1 <= 0) goto FINISH_DIVISION;

	int ifcarry = 0;
	if(answer[cnt + NUM_OF_FRAC_BITS + 1] == 1) {
		for(int	i = cnt + NUM_OF_FRAC_BITS + 2; i < LEN_OF_NUM; i++) 
			if(answer[i] == 1) ifcarry = 1;
		if(ifcarry == 0 && answer[cnt + NUM_OF_FRAC_BITS] == 1) 
			ifcarry = 1;
	}
	
	if(ifcarry) {
		for(int i = cnt + NUM_OF_FRAC_BITS + 1; i < LEN_OF_NUM; i++)
			answer[i] = 0;
		carry = 1;
		for(int i = cnt + NUM_OF_FRAC_BITS; i >= 0; i--) {
			carry += answer[i];
			if(carry == 2) answer[i] = 0, carry = 1;
			else{ 
				answer[i] = carry;
				carry = 0;
				break;
			}
		}
	}

	if(power1 >= BIAS * 2 + 1){
		if(flag == 0) return INFINITY;
		else {
			Temp = INFINITY;
			Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		}
		return Temp;
	}

	//filling
	FINISH_DIVISION:;
	if(flag) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
	starting = cnt + 1;
	for(int i = FRAC_TOP_BYTE; i >= 0; i--)
		for(int j = (i == FRAC_TOP_BYTE ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1)); j >= 0; j--){
			if(starting < 0) Temp.bytes[i] ^= 0 << j;
			else Temp.bytes[i] ^= answer[starting] << j;
			starting++;
		}
	for(int i = POWER_BOT_BYTE; i <= POWER_TOP_BYTE; i++)
		for(int j = (i == POWER_BOT_BYTE ? POWER_BOT_BIT : 0); j <= (i == POWER_TOP_BYTE ? POWER_TOP_BIT : (LEN_OF_BYTES - 1)); j++){
			Temp.bytes[i] ^= (power1 & 1) << j;
			power1 >>= 1;
		}
	return Temp;
}

int compare(int x[], int y[]){
	for(int i = 1; i < RAW_LINE; i++) {
		if(x[i] > y[i]) return 1;
		if(x[i] < y[i]) return -1;
	}
	return 0;
}

struct Double init_a_number() {
	int sym = 1;
	int cnt = 0;
	char ch, raw[RAW_LINE];
	int pow_ten = 0, pow_sym = 1;
	int number[RAW_LINE];
	memset(raw, 0, sizeof(raw));
	memset(number, 0, sizeof(number));
	while(1) {
		ch = getc(stdin);
		if(cnt != 0 && raw[cnt] != 'e' && raw[cnt] != 'E' && (ch == '+' || ch == '-') \
		|| ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == '\n' || ch == ENTER){
			ungetc(ch, stdin);
			break;
		}
		if(cnt != RAW_LINE - 1)  raw[++cnt] = ch;
	}
	
//	printf("%s\n%d", raw + 1, cnt);
	struct Double Temp;
	memset(Temp.bytes, 0, sizeof(Temp.bytes));
	if(raw[1] == '-') sym = -1, raw[1] = '0';
	if(raw[1] == '+') raw[1] = '0';
	int separate = 0;
	while(raw[separate] != '.' && raw[separate] != 'e' && raw[separate] != 'E' && separate <= cnt) 
		separate++;
	
	for(int i = 1; i < separate; i++)
		number[DOT - i + 1] = raw[separate - i] - '0';
	if(raw[separate] == '.')
		for(int j = 1; raw[separate + j] >= '0' && raw[separate + j] <= '9' && separate + j <= cnt; j++){
			number[DOT + j] = raw[separate + j] - '0';
		}
	
	while(raw[separate] != 'e' && raw[separate] != 'E' && separate <= cnt)
		separate ++;
	if(separate != cnt){
		if(raw[separate + 1] == '-') pow_sym = -1, separate++;
		else if(raw[separate + 1] == '+') separate++;
		for(int i = 1; separate + i <= cnt; i++)
			pow_ten = pow_ten * 10 + raw[separate + i] - '0';
		pow_ten *= pow_sym;
	}
	
	if(separate > DOT || pow_ten > DOT){
		Temp = INFINITY;
		if(sym == -1) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
//		printf("INFINITY\n");
		return Temp;
	}
	if(pow_ten < -DOT){
		Temp = ZERO;
		if(sym == -1) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
		return Temp;		
	}
	if(pow_ten > 0){
		for(int i = 0; i + pow_ten < RAW_LINE; i++)
			number[i] = number[i + pow_ten];
		for(int i = RAW_LINE - pow_ten; i < RAW_LINE; i++)
			number[i] = 0;
	} else if(pow_ten < 0) {
		for(int i = RAW_LINE - 1; i + pow_ten >= 0; i--)
			number[i] = number[i + pow_ten];
		for(int i = -pow_ten - 1; i >= 0; i--)
			number[i] = 0;
	}	
	
	int expow[RAW_LINE], index;
	int binary[RAW_LINE_2];
	memset(expow, 0, sizeof(expow));
	memset(binary, 0, sizeof(binary));
	expow[DOT] = 1;
	index = BIAS;
	if(compare(expow, number) == -1) 
		while(compare(expow, number) == -1) {
			index++;
			int carry = 0;
			for(int i = RAW_LINE - 1; i >= 1; i--){
				carry += expow[i] * 2;
				expow[i] = carry % 10;
				carry /= 10;
			}
		}
	if(compare(expow, number) == 1)
		while(compare(expow, number) == 1 && index >= 2) {
			index--;
			int carry = 0;
			for(int i = 1; i < RAW_LINE; i++) {
				carry = carry * 10 + expow[i];
				expow[i] = carry / 2;
				carry %= 2;
			}
		}
/*	for(int i = 1; i < RAW_LINE; i++){
		printf("%d", number[i]);
	}
	printf("\n");
	for(int i = 1; i < RAW_LINE; i++){
		printf("%d", expow[i]);
	}
	printf("\n");
	printf("%d\n", index); 
*/	for(int i = 1; i < NUM_OF_FRAC_BITS * 2; i++){
		if(compare(number, expow) == 1){
			binary[i] = 1;
			int carry = 0;
			for(int j = RAW_LINE - 1; j >= 1; j--){
				carry += number[j];
				if(carry < expow[j]){
					number[j] = carry + 10 - expow[j];
					carry = -1;
				}
				else {
					number[j] = carry - expow[j];
					carry = 0;
				}
			}
		}
		else binary[i] = 0;
		int carry = 0;
		for(int j = 1; j < RAW_LINE; j++){
			carry = carry * 10 + expow[j];
			expow[j] = carry / 2;
			carry %= 2;
		}
	}
/*	for(int i = 1; i < NUM_OF_FRAC_BITS * 2; i++){
		printf("%d", binary[i]);
	}
	printf("\n");
*/	int ifcarry = 0;
	if(binary[NUM_OF_FRAC_BITS + 2] == 0);
	else
		for(int i = NUM_OF_FRAC_BITS + 3; i < NUM_OF_FRAC_BITS * 2; i++)
			if(binary[i] == 1){
				ifcarry = 1;
				break;
			}
	if(ifcarry) {
		int carry = 1;
		for(int i = NUM_OF_FRAC_BITS + 1; i >= 0; i--){
			carry += binary[i];
			if(carry == 2) binary[i] = 0, carry = 1;
			else {binary[i] = 1, carry = 0; break;}
		}
	}
	
	if(sym == -1) Temp.bytes[NUM_OF_BYTES - 1] ^= (1 << (LEN_OF_BYTES - 1));
	int starting = 2;
	if(binary[0]) index++, starting = 1;
	else if(index == 1 && binary[1] == 0) index = 0;
	
	for(int i = FRAC_TOP_BYTE; i >= 0; i--)
		for(int j = (i == FRAC_TOP_BYTE ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1)); j >= 0; j--){
			Temp.bytes[i] ^= binary[starting] << j;
			starting++;
		}
	for(int i = POWER_BOT_BYTE; i <= POWER_TOP_BYTE; i++)
		for(int j = (i == POWER_BOT_BYTE ? POWER_BOT_BIT : 0); j <= (i == POWER_TOP_BYTE ? POWER_TOP_BIT : (LEN_OF_BYTES - 1)); j++){
			Temp.bytes[i] ^= (index & 1) << j;
			index >>= 1;
		}
	return Temp;
}

void outit_a_number(struct Double x1){
	if((x1.bytes[NUM_OF_BYTES - 1] & (1 << (LEN_OF_BYTES - 1))) > 0) 
		printf("-");
	int index = Lift_Power(x1);
	int binary[NUM_OF_FRAC_BITS * 2];
	int expow[RAW_LINE];
	int answer[RAW_LINE];
	memset(binary, 0, sizeof(binary));
	memset(expow, 0, sizeof(expow));
	memset(answer, 0, sizeof(answer));
	int cnt = 1;
	for (int i = FRAC_TOP_BYTE; i >= 0; i--)
		for (int j = (i == FRAC_TOP_BYTE) ? FRAC_TOP_BIT : (LEN_OF_BYTES - 1); j >= 0; j--)
			binary[++cnt] = (x1.bytes[i] & (1 << j)) > 0;
	if(index == 0) index = 1;
	else binary[1] = 1;
	if(index == BIAS * 2 + 1){
		for(int i = 1; i <= NUM_OF_FRAC_BITS + 1; i++)
			if(binary[i] == 1){
				printf("nan\n");
				return;
			}
		printf("inf");
		return;
	}
	index -= BIAS;
	expow[DOT] = 1;
	while(index > 0){
		index--;
		int carry = 0;
		for(int i = RAW_LINE - 1; i > 0; i--){
			carry += expow[i] * 2;
			expow[i] = carry % 10;
			carry /= 10;
		}
	}
	while(index < 0){
		index++;
		int carry = 0;
		for(int i = 1; i < RAW_LINE; i++){
			carry = carry * 10 + expow[i];
			expow[i] = carry / 2;
			carry &= 1;
		}
	}
	for(int i = 1; i <= NUM_OF_FRAC_BITS + 1; i++){
		if(binary[i] == 1){
			int carry = 0;
			for(int j = RAW_LINE - 1; j > 0; j--){
				carry += answer[j] + expow[j];
				answer[j] = carry % 10;
				carry /= 10;
			}
		}
		int carry = 0;
		for(int j = 1; j < RAW_LINE; j++){
			carry = carry * 10 + expow[j];
			expow[j] = carry / 2;
			carry &= 1;
		}
	}
	int pre = 0;
	int ifcarry = 0;
	if(answer[DOT + DECIMAL_PART + 1] > 5) ifcarry = 1;
	else if(answer[DOT + DECIMAL_PART + 1] < 5) ifcarry = 0;
	else 
		for(int i = DOT + DECIMAL_PART + 2; i < RAW_LINE; i++)
			if(answer[i] > 0) ifcarry = 1;
	if(ifcarry == 0 && answer[DOT + DECIMAL_PART + 1] == 5 && answer[DOT + DECIMAL_PART] % 2 == 1) 
		ifcarry = 1;
	if(ifcarry){
		int carry = 1;
		for(int i = DOT + DECIMAL_PART; i > 0; i--){
			carry += answer[i];
			answer[i] = carry % 10;
			carry /= 10;
		}
	}
	for(int i = 1; i <= DOT + DECIMAL_PART; i++){
		if(i == DOT) pre = 1;
		if(answer[i] != 0 || pre != 0) printf("%d",answer[i]);
		if(answer[i] != 0) pre = 1; 
		if(i == DOT) printf(".");
	}
	printf("\n");

}

struct Double Calculate(){
	struct Double regist[MAX_NUMBERS];
	struct Double Temp1, Temp2;
	int regist_top = 0;
	for(int i = 1; i <= top; i++){
		if(reverse_polish[i] == 1)
			regist[++regist_top] = number_polish[i];
		else {
			Temp2 = regist[regist_top];
			Temp1 = regist[regist_top - 1];
			if(operator_polish[i] == '+') Temp1 = Function_Addition(Temp1, Temp2);
			if(operator_polish[i] == '-') Temp1 = Function_Subtraction(Temp1, Temp2);
			if(operator_polish[i] == '*') Temp1 = Function_Multiplication(Temp1, Temp2);
			if(operator_polish[i] == '/') Temp1 = Function_Division(Temp1, Temp2);
			regist_top -= 2;
			regist[++regist_top] = Temp1;
		}
	}
	return regist[regist_top];
}

int main () {
	freopen("Double_Calculator.in", "r", stdin);
	freopen("Double_Calculator.out", "w", stdout);
	struct Double Temp;
	char ch, lastch;
	struct Double receiver;
	ZERO = Function_Transform_Double_to_Bytes (0.0);
	NaN = Function_Transform_Double_to_Bytes (0.0 / 0.0);
	INFINITY = Function_Transform_Double_to_Bytes (1.0 / 0.0);	
/*	Show_Double(init_a_number());
	double realone;
	char testone;
	testone = getc(stdin);
	if(testone == '+') printf("HA");
	scanf("%lf", &realone);
	Show_Double(Function_Transform_Double_to_Bytes(realone));
*/	
	int cases = TARGET_CASES;
//	scanf("%d\n", &cases);
	while(cases--){
		memset(operator_, 0, sizeof(operator_)); operator_top = 0;
		memset(reverse_polish, 0, sizeof(reverse_polish)); top = 0;
		memset(operator_polish, 0, sizeof(operator_polish));
		memset(number_polish, 0, sizeof(number_polish));
		lastch = -1;
		while((ch = getc(stdin)) != ENTER && ch != '\n'){
//			printf("%c\n", ch);
			if(ch == '('){
				operator_[++operator_top] = ch;
				lastch = ch;
			}
			else if(ch == ')') {
				while(operator_[operator_top] != '('){
					reverse_polish[++top] = 0;
					operator_polish[top] = operator_[operator_top];
					operator_[operator_top--] = 0;
				}
				operator_[operator_top--] = 0;
				lastch = ch;
			}
			else if((ch == '+' || ch == '-') && lastch == '(' || lastch == -1 || ch >= '0' && ch <= '9' || ch == '.'){
				ungetc(ch, stdin);
				receiver = init_a_number();
				reverse_polish[++top] = 1;
				number_polish[top] = receiver;
				lastch = 0;
			}
			else{
				while(operator_top != 0){
					if(operator_[operator_top] == '(') break;
					if((operator_[operator_top] == '+' || operator_[operator_top] == '-') && (ch == '*' || ch == '/')) break;
					reverse_polish[++top] = 0;
					operator_polish[top] = operator_[operator_top];
					operator_[operator_top--] = 0;
				}
				operator_[++operator_top] = ch;
				lastch = ch;
			}
		}
		while(operator_top != 0){
			reverse_polish[++top] = 0;
			operator_polish[top] = operator_[operator_top];
			operator_[operator_top--] = 0;
		}
		Temp = Calculate();
//		printf("%.30lf\n", Function_Transform_Bytes_to_Double(Temp));
		outit_a_number(Temp);
	}
	return 0;
}

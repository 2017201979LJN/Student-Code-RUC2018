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
#define TARGET_CASES 1

struct Double {
	unsigned char bytes[NUM_OF_BYTES];
};

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

int main () {
//	freopen("Result.out","w",stdout);
	srand (time (NULL));
	ZERO = Function_Transform_Double_to_Bytes (0.0);
	NaN = Function_Transform_Double_to_Bytes (0.0 / 0.0);
	INFINITY = Function_Transform_Double_to_Bytes (1.0 / 0.0);	
/*** begin of the loop ***/
	for(int Test_Case = 1; 1; Test_Case++) {
/*** begin of the loop ***/
	char operator;
	double Real_x1, Tran_x1, Real_x2, Tran_x2;
	struct Double Simu_x1, Simu_x2;
	double Simu_Sum, Simu_Difference, Simu_Product, Simu_Quotient;
	struct Double Sum, Difference, Product, Quotient;
	if(Test_Case & 1) Double_Rand_Combining (&Real_x1, &Real_x2);
	else Real_x1 = Double_Rand(), Real_x2 = Double_Rand();
//	scanf("%lf %c %lf\n", &Real_x1, &operator, &Real_x2);
	Simu_x1 = Function_Transform_Double_to_Bytes (Real_x1);
	Simu_x2 = Function_Transform_Double_to_Bytes (Real_x2);

//	Show_Double (Simu_x1);
//	Show_Double (Simu_x2);
	Tran_x1 = Function_Transform_Bytes_to_Double (Simu_x1);
	Tran_x2 = Function_Transform_Bytes_to_Double (Simu_x2);
	if( 1 ) {
		Sum = Function_Addition (Simu_x1, Simu_x2);
//		Show_Double (Sum);
		Simu_Sum = Function_Transform_Bytes_to_Double (Sum);
//		printf("%.320lf\n\n", Real_x1 + Real_x2);
//		printf("%.25lf\n", Simu_Sum);
	}
	if( 1 ) {
		Difference = Function_Subtraction (Simu_x1, Simu_x2);
//		Show_Double (Difference);
		Simu_Difference = Function_Transform_Bytes_to_Double (Difference);
//		printf("%.320lf\n\n", Real_x1 - Real_x2);
//		printf("%.25lf\n", Simu_Difference);
	}
	if( 1 ) {
		Product = Function_Multiplication (Simu_x1, Simu_x2);
//		Show_Double (Product);
		Simu_Product = Function_Transform_Bytes_to_Double (Product);
//		printf("%.320lf\n\n", Real_x1 * Real_x2);
//		printf("%.25lf\n", Simu_Product);
	}
	if( 1 ) {
		Quotient = Function_Division (Simu_x1, Simu_x2);
//		Show_Double (Quotient);
		Simu_Quotient = Function_Transform_Bytes_to_Double (Quotient);
//		printf("%.320lf\n\n", Real_x1 / Real_x2);
//		printf("%.25lf\n", Simu_Quotient);
	}

/*	printf("%.320lf\n\n", Simu_Sum);
	printf("%.320lf\n\n", Real_x1 + Real_x2);
	printf("%.320lf\n\n", Simu_Difference);
	printf("%.320lf\n\n", Real_x1 - Real_x2);
	printf("%.320lf\n\n", Simu_Product);
	printf("%.320lf\n\n", Real_x1 * Real_x2);
	printf("%.320lf\n\n", Simu_Quotient);
	printf("%.320lf\n\n", Real_x1 / Real_x2);
*/		

	if(Real_x1 + Real_x2 != Simu_Sum){
		if(!(Simu_Sum == Simu_Sum) && !(Real_x1 + Real_x2 == Real_x1 + Real_x2)) goto LAB1;
		printf("Oops in Addition function\n");
		printf("%.320lf\n\n", Simu_Sum);
		printf("%.320lf\n\n", Real_x1 + Real_x2);
		Show_Double (Simu_x1);
		Show_Double (Simu_x2);
		Show_Double (Sum);
		Show_Double (Function_Transform_Double_to_Bytes (Real_x1 + Real_x2));
		getchar();
	}
	LAB1:;
	if(Real_x1 - Real_x2 != Simu_Difference){
		if(!(Simu_Difference == Simu_Difference) && !(Real_x1 - Real_x2 == Real_x1 - Real_x2)) goto LAB2;
		printf("Oops in Subtraction function\n");
		printf("%.320lf\n\n", Simu_Difference);
		printf("%.320lf\n\n", Real_x1 - Real_x2);
		getchar();
	}
	LAB2:;
	if(Real_x1 * Real_x2 != Simu_Product){
		if(!(Simu_Product == Simu_Product) && !(Real_x1 * Real_x2 == Real_x1 * Real_x2)) goto LAB3;
		printf("Oops in Multiplication function\n");
		printf("%.320lf\n\n", Simu_Product);
		printf("%.320lf\n\n", Real_x1 * Real_x2);
		Show_Double (Simu_x1);
		Show_Double (Simu_x2);
		Show_Double (Product);
		Show_Double (Function_Transform_Double_to_Bytes (Real_x1 * Real_x2));
	
		getchar();
	}
	LAB3:;
	if(Real_x1 / Real_x2 != Simu_Quotient){
		if(!(Simu_Quotient == Simu_Quotient) && !(Real_x1 / Real_x2 == Real_x1 / Real_x2)) goto LAB4;
		printf("Oops in Division function\n");
		printf("%.320lf\n\n", Simu_Quotient);
		printf("%.320lf\n\n", Real_x1 / Real_x2);
		Show_Double (Simu_x1);
		Show_Double (Simu_x2);
		Show_Double (Quotient);
		Show_Double (Function_Transform_Double_to_Bytes (Real_x1 / Real_x2));
		getchar();	
	}
	LAB4:;
	if(Test_Case % 10000 == 0) printf("%d cases checked\n", Test_Case);


/*** end of the loop ***/
	}
	return 0;
/*** end of the loop ***/
}

// a = 0x78a9aa225201c8b5;
// b = 0x5b805e795a3d03ae;
// mult
// c = 

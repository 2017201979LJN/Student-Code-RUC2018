#define BYTES_OF_DOUBLE 8
#define LEN_OF_BYTES 8
#define LEN_OF_FORMULAR 100
#define NUM_OF_BRACKETS 8

/*	Please pay attention that:
	If you want to use the functions below,
	you must define your random seed ahead of time.
	The seed won't be changed in this .h file.
*/

void Double_Rand_Combining (double *Result1, double *Result2){
	unsigned char Temp;
	unsigned char * start1 = (unsigned char *) Result1;
	unsigned char * start2 = (unsigned char *) Result2;
	Temp = rand() ^ ((rand() & 1) << (LEN_OF_BYTES - 1));
	start1[BYTES_OF_DOUBLE - 1] = Temp;
	Temp ^= ((rand() & 1) << (LEN_OF_BYTES - 1));
	start2[BYTES_OF_DOUBLE - 1] = Temp;
	for(int i = 0; i < BYTES_OF_DOUBLE - 1; i++){
		Temp = rand() ^ ((rand() & 1) << (LEN_OF_BYTES - 1));
		start1[i] = Temp;
		Temp = rand() ^ ((rand() & 1) << (LEN_OF_BYTES - 1));
		start2[i] = Temp;
	}
}

/*
double Double_Rand () {
	unsigned char Temp;
	double Result;
	unsigned char * start = (unsigned char *) &Result;
	for(int i = 0; i < BYTES_OF_DOUBLE; i++){
		Temp = rand() ^ ((rand() & 1) << (LEN_OF_BYTES - 1));
		start[i] = Temp;
	}
	return Result;
}*/

unsigned long long rand64() {
    return (((unsigned long long)rand()) << 32) | ((unsigned long long)rand());
}

double Double_Rand () {
    // The first method can generate more edge case while the second
    // method can prevent one number is less/greater than too many times
    // of another.
    if (rand() & 1) {
        union {
            unsigned long long uint64_t_value;
            double double_value;
        } u;
        u.uint64_t_value = rand64();
        return u.double_value;
    } else {
        double a = rand64() * (1ull << 63) + rand64(),
               b = rand64() * (1ull << 63) + rand64();
        //int sign = (rand() & 1) ? 1 : -1;
        return a / b /* sign */;
    }
}

void formular_printer() {
	double number[LEN_OF_FORMULAR * 2];
	char operator_[LEN_OF_FORMULAR * 2];
	int bracket[LEN_OF_FORMULAR * 2];
	memset(number, 0, sizeof(number));
	memset(operator_, 0, sizeof(operator_));
	memset(bracket, 0, sizeof(bracket));
	for(int i = 1; i <= LEN_OF_FORMULAR; i++){
		number[i] = (rand()%100 + 1) * 1.0;
	}
	for(int i = 1; i < LEN_OF_FORMULAR; i++){
		operator_[i] = rand() % 4 + 1;
		if(operator_[i] == 1) operator_[i] = '+';
		if(operator_[i] == 2) operator_[i] = '-';
		if(operator_[i] == 3) operator_[i] = '*';
		if(operator_[i] == 4) operator_[i] = '/';
	}
	for(int i = 1; i < NUM_OF_BRACKETS; i++){
		int x1 = rand()%100 + 1;
		int x2 = rand()%100 + 1;
		if(x1 > x2) x1 ^= x2 ^= x1 ^= x2;
		if(x1 == x2) continue;
		if(bracket[x1] == 0 && bracket[x2] == 0){
			bracket[x1] = 1;
			bracket[x2] = 2;
		}
	}
	for(int i = 1; i < LEN_OF_FORMULAR; i++){
		if(bracket[i] == 1) printf("(");
		printf("%.1lf", number[i]);
		if(bracket[i] == 2) printf(")");
		printf("%c",operator_[i]);
	}
	
	printf("%.1lf", number[LEN_OF_FORMULAR]);
	if(bracket[LEN_OF_FORMULAR] == 2) printf(")");
	printf("\n");
}

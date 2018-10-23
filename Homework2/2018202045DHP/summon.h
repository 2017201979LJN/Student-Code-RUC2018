#define BYTES_OF_DOUBLE 8
#define LEN_OF_BYTES 8

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
        int sign = (rand() & 1) ? 1 : -1;
        return a / b * sign;
    }
}


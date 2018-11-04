#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ORDER_NUM 25

char sym[4]={'+','*'};

void out_vari(int x){
	if(x<=5) printf("x%d",x);
	else printf("t%d",x-5);
}

void random_formular_noT(int ti){
	char temp;
	int form_type = rand() % 2;
//	if(form_type == 2 && ti == 1) form_type = 1;
	if(form_type == 0 || form_type == 2) {
		out_vari(rand()%(ti+4)+1);
		printf("%c",temp=sym[rand()%2]);
		if(temp == '+') printf("%d",rand()%32768+1);
		else printf("%d",rand()%327+1);		
		printf(";\n");
	}
	if(form_type == 1) {
		out_vari(rand()%(ti+4)+1);
		printf("%c",temp=sym[rand()%2]);
		out_vari(rand()%(ti+4)+1);
		printf(";\n");
	}
}

int main(){
	srand(time(NULL));
	freopen("test.c", "w", stdout);
	printf("#include<stdio.h>\n\n");
	printf("long long play(long long x1, long long x2, long long x3, long long x4, long long x5) {\n");
	for(int i = 1; i <= 5; i++){
		printf("long long t%d=", i);
		random_formular_noT(i);
	}
	for(int i = 1; i <= ORDER_NUM; i++){
		out_vari(rand()%10+1);
		printf("=");
		random_formular_noT(6);
	}printf("return ");
	out_vari(rand()%10+1);
	printf(";\n}\n");
	printf("int main(){\nlong long x1, x2, x3, x4, x5;\nscanf(\"%%lld %%lld %%lld %%lld %%lld\", &x1, &x2, &x3, &x4, &x5);\nprintf(\"%%lld\\n\",play(x1, x2, x3, x4, x5));\nreturn 0;\n}");
	return 0;
}

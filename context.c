#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include<unistd.h>
#include<signal.h>
#include<sys/time.h>
#include<time.h>
#include<string.h>

void my_Alarm_Handler();
void func1(void);
void simulating(void);
void tt();
int F; 
struct itimerval t;

ucontext_t simulate;
ucontext_t child;

int main(){
	F=0;
	simulating();
	return 0;
}

void my_Alarm_Handler()
{
	/*// Set interval
        t.it_interval.tv_sec = 0;
        t.it_interval.tv_usec = 0;

        // Set timer initial value
        t.it_value.tv_sec = 0;
        t.it_value.tv_usec = 0;

        setitimer(ITIMER_REAL, &t, NULL);//3210 543210 543210 543210..*/
	printf("Alarm!\n");
	//swapcontext(&child,&simulate);
}

void func1(void){
	int i=1;

	clock_t s,f;
	s=clock();
	//for(i=1;i<=10;i++)
	while(1)
	{
		f=clock();
		if(((double)(f-s))/(CLOCKS_PER_SEC) > 1)
		{
			s=clock();
			printf("%d\n",i++);
			//sleep(1);
		}
	}
	/*while(1)
	{
		i=getitimer(ITIMER_REAL, &t);
		printf("%d\n",i);
		usleep(1000000);
	}*/

	return;
}

void tt(){
	func1();
	F=1;
	swapcontext(&child,&simulate);
}
void simulating(void)
{
	printf("123\n");

	// Set interval
	t.it_interval.tv_sec = 5;
	t.it_interval.tv_usec = 0;

	// Set timer initial value
	t.it_value.tv_sec = 3;
	t.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &t, NULL);//3210 543210 543210 543210..

	if(signal(SIGALRM, my_Alarm_Handler)<0)
	{
		//0的時候印出來
		printf("Failed\n");
	}

	getcontext(&simulate);
	simulate.uc_stack.ss_sp=malloc(SIGSTKSZ);
	simulate.uc_stack.ss_size=SIGSTKSZ;
	simulate.uc_link=&simulate;
	simulate.uc_stack.ss_flags=0;
	makecontext(&simulate,(void (*)(void))simulating,0);

	getcontext(&child); //獲取現在的context
	child.uc_stack.ss_sp=malloc(SIGSTKSZ); //指定stack
	child.uc_stack.ss_size=SIGSTKSZ; //指定stack大小
	child.uc_stack.ss_flags=0;
	child.uc_link=&simulate; //執行完後回到simulating
	makecontext(&child,(void (*)(void))tt,0);//修改context指向tt
	while(1)
	{
		if(F==0)
			swapcontext(&simulate,&child); //保存simulate context,切換child context
		printf("simulating\n");
	}
	// func1執行完後回到這裏
	//puts("simulating");
}

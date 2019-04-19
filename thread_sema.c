#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/ipc.h>

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *_buf;
}arg;
struct semid_ds my_semid;
struct sembuf my_sembuf;

int semid;
char *buf;

void* fun1(void * arg1)
{
	/* locking */
	printf("entered in 1\n");
	my_sembuf.sem_num=0;
	my_sembuf.sem_op=-1;
	my_sembuf.sem_flg=0;
	semop(semid,&my_sembuf,1);

	printf("waiting in 1\n");

	printf("%s\n",buf);

	printf("thread 1 finished\n");
	/*unlocking */
	my_sembuf.sem_num=0;
	my_sembuf.sem_op=1;
	my_sembuf.sem_flg=0;
	semop(semid,&my_sembuf,1);
}
void* fun2(void * arg2)
{
	/* locking */
	printf("entered in 2\n");
	my_sembuf.sem_num=0;
	my_sembuf.sem_op=-1;
	my_sembuf.sem_flg=0;
	semop(semid,&my_sembuf,1);

	printf("waiting in 2\n");

	printf("%s\n",buf);

	printf("thread 2 finished\n");
	/*unlocking */
	my_sembuf.sem_num=0;
	my_sembuf.sem_op=1;
	my_sembuf.sem_flg=0;
	semop(semid,&my_sembuf,1);
}
int main()
{
	semid=semget(1500,1,IPC_CREAT|0644);
	arg.val=1;
	semctl(semid,0,SETVAL,arg);
        

	pthread_t thread1,thread2;

	pthread_attr_t attribute1;
	pthread_attr_t attribute2;

	int *ptr1,*ptr2;

	pthread_attr_init(&attribute1);
	pthread_attr_init(&attribute2);


	/* locking */
	my_sembuf.sem_num=0;
	my_sembuf.sem_op=-1;
	my_sembuf.sem_flg=0;
	semop(semid,&my_sembuf,1);

	pthread_create(&thread1,&attribute1,fun1,NULL);
	printf("thread 1 running\n");
	pthread_create(&thread2,&attribute2,fun2,NULL);
	printf("thread 2 running\n");

	buf=(char *)malloc(100);
	scanf("%[^\n]s",buf);

	/* unlocking */
	my_sembuf.sem_num=0;
	my_sembuf.sem_op=1;
	my_sembuf.sem_flg=0;
	semop(semid,&my_sembuf,1);
	

	pthread_join(thread1,(void *)&ptr1);
	pthread_join(thread2,(void *)&ptr2);
	return 0;
}

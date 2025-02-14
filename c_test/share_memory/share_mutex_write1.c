// /写共享内存
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h> //declare O_CREAT O_RDWR

typedef struct _Teacher
{
	char name[64];
	int age;
}Teacher;

int main()
{
	int shmid = -1;
	key_t key = 0x2234;
	Teacher *p = NULL;
	int count = 0;
	sem_t *semw = NULL;
	semw = sem_open("sem_w", O_CREAT | O_RDWR, 0666, 1 );
	if (semw == SEM_FAILED)
	{
		printf("errno = %d\n", errno );
		return -1;
	}
	int value;
	sem_getvalue(semw, &value);
	printf("write: semw:%d\n", value);
	shmid = shmget(key, sizeof(Teacher), 0666 | IPC_CREAT );
	if ( shmid == -1 )
	{
		perror("shmget");
		return -1;
	}
	p = (Teacher*)shmat(shmid, NULL, 0);
	if (p == (Teacher*)(-1))
	{
		perror("shmat");
		return -1;
	}
	// while(1)
	// {
		sem_wait(semw);
		strcpy(p->name, "aaaa");
		p->age = count;
		++count;
		sleep(10);
		sem_post(semw);
	// }
	return 0;
}
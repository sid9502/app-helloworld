#include <uk/atomic.h>
#include <uk/spinlock.h>
#include <uk/mutex.h>
#include <uk/semaphore.h>
#include <uk/rwlock.h>
#include <uk/plat/lcpu.h>
#include <uk/sched.h>
#include <uk/schedcoop.h>
#include <stdlib.h>
#include <uk/vmem.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define STACK_SIZE (4096)
#define ALLOCATION_ROUNDS 100
#define OVERALL_ROUNDS 1000
#define N_CPUS 8

struct uk_spinlock print_lock = UK_SPINLOCK_INITIALIZER();



void map_test(struct __regs *regs, void *arg)

{
   uk_spin_lock(&print_lock);
   printf("iN read file \n");
       
   char buf[42];
   int fd;

   fd = open("/westworld.txt", O_RDWR | O_CREAT, 0777);
   write(fd, "These violent delights have violent ends.", 41);
   close(fd);

   fd = open("/westworld.txt", O_RDWR | O_CREAT, 0777);
   read(fd, buf, 41);
   buf[41]='\0';
   printf("%s\n", buf);
   close(fd);
    uk_spin_unlock(&print_lock);
	return ;
       

}


int main() {
	printf("in main\n");
	int fd;
        fd = open("/westworld2.txt", O_RDWR | O_CREAT, 0777);
        write(fd, "These violent delights have violent ends.", 41);
        close(fd);
	int i;
	unsigned int lcpuid_start[] = {0,1, 2, 3, 4, 5, 6, 7};
	unsigned int num_start = N_CPUS;
	unsigned int lcpuid_run[] =  {0,1, 2, 3, 4, 5, 6, 7};
	unsigned int num_run = N_CPUS;

	ukplat_lcpu_entry_t entry[] = {0, 0, 0, 0, 0, 0, 0, 0};

	struct ukplat_lcpu_func map_test_func;
	map_test_func.fn = map_test;

	void *stack[N_CPUS];
	for (i = 0; i < 8; i++) {
		stack[i] = malloc(PAGE_SIZE * 20);
	}

	

	ukplat_lcpu_start(lcpuid_start, &num_start, stack, entry, 0);
	ukplat_lcpu_wait(NULL, 0, 0);

	ukplat_lcpu_run(lcpuid_run, &num_run, &map_test_func, 0);
	ukplat_lcpu_wait(NULL, 0, 0);

	return 0;
}

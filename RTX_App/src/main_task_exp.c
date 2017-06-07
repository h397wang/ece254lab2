/**
 * @brief: ECE254 Keil ARM RL-RTX Task Management Lab starter file that calls os_tsk_get()
 * @file: main_task_exp.c
 * @date: 2015/09/13
 */
/* NOTE: This release is for students to use */

#include <LPC17xx.h>
#include "uart_polling.h" 
#include <RTL.h>
#include "../../RTX_CM3/INC/RTL_ext.h" /* extended interface header file */
#include <stdio.h>
#include <string.h>
#include "../../RTX_CM3/SRC/CM/rt_MemBox_ext.h"

#define numBlocks 4
#define blockSize 4

_declare_box (our_mempool, blockSize, numBlocks);
void * address_array[numBlocks];
int index = 0;

#define NUM_FNAMES 4

struct func_info {
  void (*p)();      /* function pointer */
  char name[32];    /* name of the function, was 16 */
};

extern void os_idle_demon(void);
__task void task1(void);
__task void task_print_states(void);
__task void init (void);

__task void task_allocate_mem(void);

__task void task_allocate_high(void);
__task void task_allocate_low(void);
    
char *state2str(unsigned char state, char *str);
char *fp2name(void (*p)(), char *str);


OS_MUT g_mut_uart;
OS_TID g_tid = 255;

int  g_counter = 0;  // a global counter
char g_str[16];
char g_tsk_name[16];

struct func_info g_task_map[NUM_FNAMES] = \
{
  /* os_idle_demon function ptr to be initialized in main */
  {NULL,  "os_idle_demon"}, \
  {task1, "task1"},   \
  {task_print_states, "task_print_states"},   \
  {init,  "init" }
};

/* no local variables defined, use one global var */
__task void task1(void) {
	for (;;) {
		g_counter++;
	}
}


/*--------------------------- task2 -----------------------------------*/
/* checking states of all tasks in the system                          */
/*---------------------------------------------------------------------*/
__task void task_print_states(void) {
	U8 i = 1;
	RL_TASK_INFO task_info;
    int numTasks = 0;
	while(1) {
        numTasks = rt_tsk_count_get();
        os_mut_wait(g_mut_uart, 0xFFFF);
        printf("TID\tNAME\t\t\tPRIO\tSTATE   \t%%STACK\n");
        os_mut_release(g_mut_uart);
        for(i = 0; i < numTasks; i++) { 
            if (os_tsk_get(i+1, &task_info) == OS_R_OK) {
                os_mut_wait(g_mut_uart, 0xFFFF);  
                printf("%d\t%s\t\t\t%d\t%s\t%d%%\n", \
                       task_info.task_id, \
                       fp2name(task_info.ptask, g_tsk_name), \
                       task_info.prio, \
                       state2str(task_info.state, g_str),  \
                       task_info.stack_usage);
                os_mut_release(g_mut_uart);
            } 
        }     
        if (os_tsk_get(0xFF, &task_info) == OS_R_OK) {
            os_mut_wait(g_mut_uart, 0xFFFF);  
            printf("%d\t%s\t\t%d\t%s\t%d%%\n", \
                   task_info.task_id, \
                   fp2name(task_info.ptask, g_tsk_name), \
                   task_info.prio, \
                   state2str(task_info.state, g_str),  \
                   task_info.stack_usage);
            os_mut_release(g_mut_uart);
        } 
        os_dly_wait(20);
    }
}


__task void task_allocate_mem(void) {
  #if 0
  void * box_address;
  OS_RESULT os_result;
  
  box_address = rt_alloc_box_s(our_mempool);
  if (box_address == NULL) {
    printf("Allocation not successful\n");
  } else {
    printf("Allocation successful\n");
  }
  
  os_result = rt_free_box_s (our_mempool, box_address);
  if (os_result == OS_R_NOK) {
    printf("Deallocation not successful\n");
  } else {
    printf("Deallocation successful\n");
  }
  os_tsk_delete_self();
  #endif
}


__task void task_allocate_high(void) {
    void * box;
    while(1) {
        os_mut_wait(g_mut_uart, 0xFFFF);
        box = rt_alloc_box_s(our_mempool);
        if (box != NULL) {
            // mutex
            address_array[index] = box; 
            index++;
            printf("index: %d\n", index);
        } 
        os_mut_release(g_mut_uart); 
        //os_dly_wait(10);        
    }
}

__task void task_allocate_low(void) {
    void * box;
    while(1) {
       os_mut_wait(g_mut_uart, 0xFFFF);
        box = rt_alloc_box_s(our_mempool);
        if (box != NULL) {
            // mutex
            address_array[index] = box; 
            index++;
            printf("index: %d\n", index);
        }
        os_mut_release(g_mut_uart);  
        //os_dly_wait(10);    
    }
}    
       
__task void task_deallocate(void) {
   while(1) {
       os_mut_wait(g_mut_uart, 0xFFFF);
       printf("index %d\n", index);
       if (index >= numBlocks-1) {
            index--;
            rt_free_box_s(our_mempool, address_array[index]);
       }
       os_mut_release(g_mut_uart);
       //os_dly_wait(10);  
    }
}



/*--------------------------- init ------------------------------------*/
/* initialize system resources and create other tasks                  */
/*---------------------------------------------------------------------*/
__task void init(void)
{
	os_mut_init(&g_mut_uart);
  
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: TID = %d\n", os_tsk_self());
	//os_mut_release(g_mut_uart);
  
	g_tid = os_tsk_create(task1, 1);  /* task 1 at priority 1 */
	//os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task1 with TID %d\n", g_tid);
	//os_mut_release(g_mut_uart);

	g_tid = os_tsk_create(task_print_states, 10);  /* task 2 at priority 1 */
	//os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task_print_states with TID %d\n", g_tid);
	//os_mut_release(g_mut_uart);
  
    g_tid = os_tsk_create(task_allocate_mem, 1);
	//os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task_allocate_mem with TID %d\n", g_tid);  
    //os_mut_release(g_mut_uart);
    
    g_tid = os_tsk_create(task_allocate_high, 4);
	printf("init: created task_allocate_high with TID %d\n", g_tid);  

    g_tid = os_tsk_create(task_allocate_low, 2);
	printf("init: created task_allocate_low with TID %d\n", g_tid);
    
    // must be in between those pri 2 and 4
    g_tid = os_tsk_create(task_deallocate, 3);
	printf("init: created task_deallocate with TID %d\n", g_tid);  
    
    // Dont forget to free this shit
    os_mut_release(g_mut_uart);
    
	os_tsk_delete_self();     /* task MUST delete itself before exiting */
}

/**
 * @brief: convert state numerical value to string represenation      
 * @param: state numerical value (macro) of the task state
 * @param: str   buffer to save the string representation, 
 *               buffer to be allocated by the caller
 * @return:the string starting address
 */
char *state2str(unsigned char state, char *str) {
	switch (state) {
	case INACTIVE:
		strcpy(str, "INACTIVE");
		break;
	case READY:
		strcpy(str, "READY   ");
		break;
	case RUNNING:
		strcpy(str, "RUNNING ");
		break;
	case WAIT_DLY:
		strcpy(str, "WAIT_DLY");
		break;
	case WAIT_ITV:
		strcpy(str, "WAIT_ITV");
		break;
	case WAIT_OR:
		strcpy(str, "WAIT_OR");
		break;
	case WAIT_AND:
		strcpy(str, "WAIT_AND");
		break;
	case WAIT_SEM:
		strcpy(str, "WAIT_SEM");
		break;
	case WAIT_MBX:
		strcpy(str, "WAIT_MBX");
		break;
	case WAIT_MUT:
		strcpy(str, "WAIT_MUT");
		break;
	default:
		strcpy(str, "UNKNOWN");    
	}
	return str;
}

/** 
 * @brief: get function name by function pointer
 * @param: p the entry point of a function (i.e. function pointer)
 * @param: str the buffer to return the function name
 * @return: the function name string starting address
 */
char *fp2name(void (*p)(), char *str)
{
	int i;
	unsigned char is_found = 0;
  
	for ( i = 0; i < NUM_FNAMES; i++) {
		if (g_task_map[i].p == p) {
			str = strcpy(str, g_task_map[i].name);  
			is_found = 1;
			break;
		}
	}
	if (is_found == 0) {
		strcpy(str, "ghost");
	}
	return str;
}


int main(void) {
    int init_box_status;
     
    SystemInit();         /* initialize the LPC17xx MCU */
	uart0_init();         /* initilize the first UART */
    
	/* fill the fname map with os_idle_demon entry point */
	g_task_map[0].p = os_idle_demon;
  
    init_box_status = _init_box (our_mempool, sizeof(our_mempool), numBlocks);  
	printf("init_box_status: %d, 0 for success\n", init_box_status);
    
    init_blocked_pq();
	os_sys_init(init);    /* initilize the OS and start the first task */
}

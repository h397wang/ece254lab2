/*----------------------------------------------------------------------------
 *      ECE254 Lab Task Management
 *----------------------------------------------------------------------------
 *      Name:    RT_MEMBOX_ext.C
 *      Purpose: Interface functions for blocking 
 *               fixed memory block management system
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code is extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *      Includes
 *---------------------------------------------------------------------------*/
 
#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_MemBox.h"
#include "rt_HAL_CM.h"
#include "rt_Task.h"       /* added in ECE254 lab keil_proc */ 
#include "rt_MemBox_ext.h" /* added in ECE254 lab keil_proc */   
#include "rt_List.h"
//#include "../../INC/RTL_ext.h"

/* ECE254 Lab Comment: You may need to include more header files */

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/

#define NOT_ALLOCATED 0
#define ALLOCATED 1
#define NO_TIMEOUT 0xffff

#define WAIT_MEM        10 // Figure this shit out later too

P_XCB  rt_ready_pq;
P_XCB  rt_blocked_pq;

_declare_box (our_mempool, blockSize, numBlocks);

U32 isPoolInitialized = 0;
U32 blockCounter = 0;
U32 blockStatus[numBlocks] = {NOT_ALLOCATED};


/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/

/*==========================================================================*/
/*  The following are added for ECE254 Lab Task Management Assignmet       */
/*==========================================================================*/

U32 isBlockedQEmpty(void){
    if (rt_blocked_pq->p_lnk == NULL){
      return 1;
    } else {
      return 0;
    }
}

/*---------------- rt_alloc_box_s, task blocks when out of memory-----------*/

/**  
   @brief: Blocking memory allocation routine.
The primitive allocates a fixed-size of memory to the calling task from the memory
pool pointed by box_mem and returns a pointer to the allocated memory.

 */
 
void *rt_alloc_box_s (void *p_mpool) {
  U32 returnAddress;

  if (blockCounter == numBlocks) {
      rt_put_prio(rt_blocked_pq, os_tsk.run);
      rt_block(NO_TIMEOUT, WAIT_MEM);
      printf("Has been blocked\n");
      return NULL; 
  } else {
    int i;
    for (i = 0; i < numBlocks; i++) {
      if (blockStatus[i] == NOT_ALLOCATED) {      
        blockStatus[i] = ALLOCATED;  
        returnAddress = (U32) p_mpool + i*blockSize;
        blockCounter++; 
        break;
      }      
    }      
  }
  return (void*) returnAddress;
}

/*----------- rt_free_box_s, pair with _s memory allocators ----------------*/
/**
 * @brief: free memory pointed by ptr, it will unblock a task that is waiting
 *         for memory.
 * @return: OS_R_OK on success and OS_R_NOK if ptr does not belong to gp_mpool 
 */
OS_RESULT rt_free_box_s (void *p_mpool, void *box) {
    P_TCB p_tcb_unblocked;
  // TODO(more robust check required)
  // the address given by box must be incremented by 12 bytess..
  U32 boxIndex = ((U32) box - (U32) p_mpool) / blockSize;
  if (boxIndex >= numBlocks || blockStatus[boxIndex] == NOT_ALLOCATED) {
    return OS_R_NOK;
  }
  
  // This block is causing a seg fault
  // If the pri Q is NOT empty then dispatch
  #if 0
  if (isBlockedQEmpty() == 0) { 
    p_tcb_unblocked = rt_get_first(rt_blocked_pq);
    rt_dispatch(p_tcb_unblocked); 
    printf("Dispatched"\n);
  }
  #endif
  blockStatus[boxIndex] = NOT_ALLOCATED;
  blockCounter--;
  return (OS_R_OK);
}
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

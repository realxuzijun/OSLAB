/*
 * contains the implementation of all syscalls.
 */

#include <stdint.h>
#include <errno.h>

#include "util/types.h"
#include "syscall.h"
#include "string.h"
#include "process.h"
#include "util/functions.h"
#include "pmm.h"
#include "vmm.h"
#include "spike_interface/spike_utils.h"


#define PART_SIZE 512
#define PAGE_NUM 20
#define PART_NUM (PGSIZE/PART_SIZE)

typedef struct part {
  uint64 va;
  bool state;
} part;

typedef struct page {
  part part[PART_NUM];
  bool state;
  uint64 va;
} page;

static page page_list[PAGE_NUM];
static bool initial;

//
// implement the SYS_user_print syscall
//
ssize_t sys_user_print(const char* buf, size_t n) {
  // buf is now an address in user space of the given app's user stack,
  // so we have to transfer it into phisical address (kernel is running in direct mapping).
  assert( current );
  char* pa = (char*)user_va_to_pa((pagetable_t)(current->pagetable), (void*)buf);
  sprint(pa);
  return 0;
}

//
// implement the SYS_user_exit syscall
//
ssize_t sys_user_exit(uint64 code) {
  sprint("User exit with code:%d.\n", code);
  // in lab1, PKE considers only one app (one process). 
  // therefore, shutdown the system when the app calls exit()
  shutdown(code);
}

//
// maybe, the simplest implementation of malloc in the world ... added @lab2_2
//
uint64 sys_user_allocate_page() {
  if (initial == FALSE) {
    initial = TRUE;
    for (int i = 0; i < PAGE_NUM; i++) {
      page_list[i].va = g_ufree_page + i * PGSIZE;
      page_list[i].state = FALSE;
      for (int j = 0; j < PART_NUM; j++) {
        page_list[i].part[j].va = page_list[i].va + PART_SIZE * j;;
        page_list[i].part[j].state = FALSE;
      }
    }
  }
  for (int i = 0; i < PAGE_NUM; i++) {
    if (page_list[i].state == TRUE ) {
      for (int j=0; j< PART_NUM; j++) {
        if(page_list[i].part[j].state == FALSE){
          page_list[i].part[j].state == TRUE;
          return page_list[i].part[j].va;
        }
      }
    }
  }
  void* pa = alloc_page();
  uint64 va = g_ufree_page;
  g_ufree_page += PGSIZE;
  user_vm_map((pagetable_t)current->pagetable, va, PGSIZE, (uint64)pa,
         prot_to_type(PROT_WRITE | PROT_READ, 1));
  for (int i = 0; i < PAGE_NUM; i++) {
    if (page_list[i].va == va) {
      page_list[i].state = TRUE;
      page_list[i].part[0].state = TRUE;
      return va;
    }
  }
  return va;
}

//
// reclaim a page, indicated by "va". added @lab2_2
//
uint64 sys_user_free_page(uint64 va) {
  uint64 check_page = -1;
  for (int i = 0; i < PAGE_NUM; i++) {
    for (int j = 0; j < PART_NUM; j++) {
      if (page_list[i].part[j].va == va && page_list[i].part[j].state == TRUE) {
        page_list[i].part[j].state = FALSE;
        check_page = i;
      }
    }
  }
  if(check_page != -1){
    for (int i = 0; i < PART_NUM; i++)
      if (page_list[check_page].state == TRUE) {
        return 0;
      }
    user_vm_unmap((pagetable_t)current->pagetable, page_list[check_page].va, PGSIZE, 1);
    page_list[check_page].state = FALSE;
    }
  return 0;
}

//
// [a0]: the syscall number; [a1] ... [a7]: arguments to the syscalls.
// returns the code of success, (e.g., 0 means success, fail for otherwise)
//
long do_syscall(long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7) {
  switch (a0) {
    case SYS_user_print:
      return sys_user_print((const char*)a1, a2);
    case SYS_user_exit:
      return sys_user_exit(a1);
    // added @lab2_2
    case SYS_user_allocate_page:
      return sys_user_allocate_page();
    case SYS_user_free_page:
      return sys_user_free_page(a1);
    default:
      panic("Unknown syscall %ld \n", a0);
  }
}

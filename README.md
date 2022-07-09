# 操作系统实验

```
cd /mnt/e/Desktop/riscv-pke
```

## lab1_1
```
git checkout lab1_1_syscall
spike ./obj/riscv-pke ./obj/app_helloworld
git commit -a -m "my work on lab1_1 is done."
```
- 就改了一行代码，它这strap.c中handle_syscal()中根本没调用do_syscal(),直接panic了，改了就成功了。

## lab1_2
```
git checkout lab1_2_exception
git merge lab1_1_syscall -m "continue to work on lab1_2"
spike ./obj/riscv-pke ./obj/app_illegal_instruction
git commit -a -m "my work on lab1_2 is done."
```
- 和上一问如出一辙，machine/mtrap.c中case CAUSE_ILLEGAL_INSTRUCTION，直接panic了，改成调用handle_illegal_instruction()就通过了。

## lab1_3
```
git checkout lab1_3_irq
git merge lab1_2_exception -m "continue to work on lab1_3"
spike ./obj/riscv-pke ./obj/app_long_loop
git commit -a -m "my work on lab1_3 is done."
```
- 这次终于不是一行代码了哈哈，strap.c中handle_mtimer_trap()又又又panic了，按文档要求的g-tricks++，然后将sip置0。

## lab2_1
```
git checkout lab2_1_pagetable
git merge lab1_3_irq -m "continue to work on lab2_1"
spike ./obj/riscv-pke ./obj/app_helloworld_no_lds
git commit -a -m "my work on lab2_1 is done."
```
- 有现成的函数，lookup_pa()可以直接返回对应的物理页面地址，加上页内偏移就行了。
  
## lab2_2
```
git checkout lab2_2_allocatepage
git merge lab2_1_pagetable -m "continue to work on lab2_2"
spike ./obj/riscv-pke ./obj/app_naive_malloc
git commit -a -m "my work on lab2_2 is done."
```
- 调用free_page()函数，在free不为0是清零，然后有效位置为0

## lab2_3
```
git checkout lab2_3_pagefault
git merge lab2_2_allocatepage -m "continue to work on lab2_3"
spike ./obj/riscv-pke ./obj/app_sum_sequence
git commit -a -m "my work on lab2_3 is done."
```
- 主要是map_pages()函数的运用，先分配一个物理页，再将虚拟地址映射上去，权限给了读写。

## lab3_1
```
git checkout lab3_1_fork
git merge lab2_3_pagefault -m "continue to work on lab3_1"
spike ./obj/riscv-pke ./obj/app_naive_fork
git commit -a -m "my work on lab3_1 is done."
```
- 还是对map_pages()的运用，由于子进程的va和pa与父进程一样，只需要在子进程的页表中再映射一次就行。

## lab3_2
```
git checkout lab3_2_yield
git merge lab3_1_fork -m "continue to work on lab3_2"
spike ./obj/riscv-pke ./obj/app_yield
git commit -a -m "my work on lab3_2 is done."
```
- 文档说的比较清楚，挨个写代码就行
    - `current->status = READY`将当前进程置为就绪状态（READY）
    - `insert_to_ready_queue( current )`将当前进程加入到就绪队列的队尾
    - `schedule()`转进程调度

## lab3_3
```
git checkout lab3_3_rrsched
git merge lab3_2_yield -m "continue to work on lab3_3"
spike ./obj/riscv-pke ./obj/app_two_long_loops
git commit -a -m "my work on lab3_3 is done"
```
- 运行时间达到TIME_SLICE_LEN就重新插入就绪序列，然后调度

## lab3_c2
```
git checkout lab3_challenge2_semaphore
git merge lab3_3_rrsched -m "continue to work on lab3_challenge1"
spike ./obj/riscv-pke ./obj/app_semaphore
git commit -a -m "my work on lab3_challenge2 is done"
```
- 写三个函数sem_new(),sem_P(),sem_V()
  - sem_new(num):新建一个信号量，num为S值
  - sem_P(id),S--,如果S>=0,继续；如果S<0，把当前进程扔进等待队列，重新调度；
  - sem_V(id),S++,如果S>0,继续；如果S<=0，把被该信号阻塞的一个信号扔进就绪队列；
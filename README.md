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
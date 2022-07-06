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
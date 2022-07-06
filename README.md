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

## lab1_c2
```
git checkout lab1_challenge2_errorline
git merge lab1_3_irq -m "continue to work on lab1_challenge1"
spike ./obj/riscv-pke ./obj/app_errorline
git commit -a -m "my work on lab1_challenge2 is done."
```
- 感觉没上一题难，大部分东西都给你了
- 首先是获得出错的代码地址，read_csr(mepc)直接可以得到
- 然后就是找这个代码在哪，这就要用到文档中的make_addr_line，按照要求给入参数之后，可以在line数组中的addr进行对比，进而得到索引，然后依次在file和dir中查找，最后可以得到文件的地址和出错的行数。
- 最后就是读出错的代码，spike_file_open打开文件，spike_file_pread读代码就可以完成。
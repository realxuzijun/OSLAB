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

## lab1_c1
```
git checkout lab1_challenge1_backtrace
git merge lab1_3_irq -m "continue to work on lab1_challenge1"
spike ./obj/riscv-pke ./obj/app_print_backtrace
git commit -a -m "my work on lab1_challenge1 is done."
```
- 首先是补充print_backtrace()这个函数，代码甚至基本的定义，可以参考其他函数按照系统调用的模式写出来。
- 其次是帧栈的结构，文档中又说明，基本上就是fp和ra互相叠，这样很轻松的就能得到所有函数的return address
- 最后就是找函数名称了，这届涉及到elf的结构了，它的结构很复杂但这个实验需要了解的东西不多，主要是明白怎么把strtab和symtab整出来。
  - 先是读section_header_table中的name属性，它是一个索引，拿它去shstrtab去查找，就可以得到这个section的类型，这样可以将所有的symtab和strtab找出来。
  - 然后就是找函数名了，将return address与symtab中保存的地址范围相比对，就可以找到函数对应的symtab，最后那symtab中的索引在strtab中查找，就可以得到函数的名字。
# Memory Allocation

Pseudo-implementation of the C **malloc** and **free** commands, the code for which can be seen in the following files:

* *my_mem.h*
* *my_mem.c*

This doesn't reflect how the packages actually work, as the code relies on a second memory system allocated just for the program. In realiy, **malloc** and **free**
directly access the virtually allocated address space.

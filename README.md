# Minesweeper-project
This project is dedicated to the design and development of a Minesweeper game through the integration of x86-64 Assembly and C programming languages. It was developed as part of my Computer Science degree for the course “Estructura de Computadors” (Computer Architectures).

### Parts
**BMLegacyVer** was completed for the first submission and uses global Assembly variables as a mechanism for passing parameters between subroutines and C.
**BMFinalVer**, in contrast, was developed for the final submission and uses CPU registers as the primary communication method.

Both parts are independent and can be played without relying on each other.

### Compilation
Inside the directory of the selected version, first compile the Assembly code using:
> yasm -f elf -m amd64 -g dwarf2 BMp[ver].asm
Then, compile the C code and link it with the Assembly object file by executing:
> gcc -no-pie -mincoming-stack-boundary=3 -o BMp[ver] -g BMp[ver].o BMp[ver]c.c

###### Compilation dependencies
Both Yasm and GCC are required to compile the program.

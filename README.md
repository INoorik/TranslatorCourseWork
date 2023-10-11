# TranslatorCourseWork
This repository contains coursework on the discipline "programming languages and translation methods".
This program translate language you can see in `examples` directory to assembler language (at&t syntax, linux 32 bit)
# Compile compiler
```bash
gcc *.c -o compiler
cd link
gcc -c io.c -o io.o -nostdinc -nostdlib -m32 
cd ../
```
# Compile example program
## Translate to assembler
```bash
./compiler ./examples/three_bits_adder.cwl | as -32 -o three_bits_adder.o -I ./include
```
Or, if you want out assembler code to `three_bits_adder.s`
```bash
./compiler ./examples/three_bits_adder.cwl ./three_bits_adder.s
```
## Build program
```bash
ld ./three_bits_adder.o ./link/io.o -m elf_i386 -o three_bits_adder
```

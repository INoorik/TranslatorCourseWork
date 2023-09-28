# TranslatorCourseWork
This repository contains coursework on the discipline "programming languages and translation methods".
This program translate language you can see in `examples` directory to assembler language (at&t syntax, linux 32 bit)
# Compile compiler
```bash
gcc *.c -o compiler
```
# Compile example program
## Translate to assembler
```bash
./compiler ./examples/three_bits_adder.cwl ./three_bits_adder.s
```
Or, if you want out assembler code to `stdout`
```bash
./compiler ./examples/three_bits_adder.cwl
```
## Build program
```bash
gcc ./three_bits_adder.s ./link/io.c -I ./include -m32
```

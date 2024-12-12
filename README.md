# Trabajo Final Sistemas Operativos

### Comandos para compilar
El archivo para el boot solo se necesita compilar una vez.
~~~
nasm -f elf32 -o multiboot.o multiboot.asm
~~~

El resto de comandos se ejecutan cada vez que se hacen cambios
~~~
cd kernel/
gcc -m32 -c -o kernel.o kernel.c
gcc -m32 -T linker.ld -o kernel.ELF -nostdlib kernel.o multiboot.o

cd ../
grub-mkrescue -o SO.iso ./
~~~

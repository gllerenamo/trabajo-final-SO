OUTPUT_FORMAT(elf32-i386)
ENTRY(kernel_entry)

SECTIONS{
	. = 0x00100000;

	.text :{
		*(.multiboot)
		*(.text)
	}

	.rodata ALIGN (0x1000) : {
		*(.rodata)
	}

	.data ALIGN(0x1000) : {
		*(.data)
	}

	.bss : {
		sbss = .;
		*(COMMON)
		*(.bss)
		ebss = .;
	}	
}
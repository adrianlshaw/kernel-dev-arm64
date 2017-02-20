OUTPUT_ARCH(aarch64)
TARGET(binary)
OUTPUT_FORMAT("elf64-littleaarch64")
STACKTOP = 0x80000000 + 0xfff8;

SECTIONS
{
	. = 0x80000000;
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }

	. = STACKTOP;
	stacktop = .;
}

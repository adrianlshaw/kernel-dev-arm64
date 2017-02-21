# To compile rust code, run: rustup target add aarch64-unknown-linux-gnu

IMAGE := kernel.elf

CROSS_COMPILE = aarch64-linux-gnu-

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJDUMP = $(CROSS_COMPILE)objdump

CFLAGS = -Wall -fno-common -O0 -g \
         -nostdlib -nostartfiles -ffreestanding \
         -march=armv8-a

OBJS = main.o


all: $(IMAGE)

boot.o: boot.S
	$(CC) boot.S -c -o boot.o

$(IMAGE): kernel.ld boot.o $(OBJS)
	$(LD) boot.o $(OBJS) -T kernel.ld -o $(IMAGE)
	$(OBJDUMP) -d kernel.elf > kernel.list
	$(OBJDUMP) -t kernel.elf | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernel.sym

qemu: $(IMAGE)
	@qemu-system-aarch64 -M ? | grep virt >/dev/null || exit
	@echo
	@echo "To exit press Ctrl-A + X"
	qemu-system-aarch64 -machine virt \
			    -cpu cortex-a57 \
	                    -smp 4 -m 4096 \
			    -nographic -serial mon:stdio \
	                    -kernel $(IMAGE)

rust:
	aarch64-linux-gnu-gcc boot.S -c -o boot.o
	rustc --target aarch64-unknown-linux-gnu -O --emit=obj main.rs
	$(LD) boot.o main.o -T kernel.ld -o kernel.elf
	$(OBJDUMP) -d kernel.elf > kernel.list
	$(OBJDUMP) -t kernel.elf | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernel.sym

docker-qemu:
	@docker build -t aarch64-test .
	@docker run --rm -ti --entrypoint make aarch64-test qemu

docker-foundation:
	@docker build -t aarch64-test .
	@docker run -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix --rm --entrypoint "make" -ti aarch64-test "foundation"

foundation: $(IMAGE)
	@cp kernel.elf kernel.axf
	@rm -f *\.so*
	@chmod +x ./fvp/*
	@ln -s fvp/*so* .
	@./fvp/Foundation_Platform --cores=1 --image kernel.axf

clean:
	rm -f $(IMAGE) *.o *.so.* *.list *.sym

.PHONY: all qemu clean rust

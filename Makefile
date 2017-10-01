IMAGE := kernel.elf

CROSS_COMPILE = aarch64-linux-gnu-

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJDUMP = $(CROSS_COMPILE)objdump

CFLAGS = -Wall -fno-common -O0 -g \
         -nostdlib -nostartfiles -ffreestanding \
         -march=armv8-a -mgeneral-regs-only

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
	@echo "Waiting for GDB to connect on 1234"
	# For GICv3 change to: -machine virt,gic_version=3
	@echo "(To run secure mode add: -machine virt,secure=on)"
	qemu-system-aarch64 -machine virt,gic_version=3 \
			    -cpu cortex-a57 \
	            -smp 4 -m 4096 \
			    -nographic -serial mon:stdio \
			    -gdb tcp::1234 \
			    -S \
				-sandbox on \
	            -kernel $(IMAGE)

gdb:
	@gdb-multiarch $(IMAGE) -x gdbfile

docker-qemu:
	@docker build -t aarch64-test .
	@docker run --rm -ti --entrypoint make aarch64-test qemu

docker-foundation:
	@docker build -t aarch64-test .
	@docker run -e DISPLAY=$DISPLAY \
		-v /tmp/.X11-unix:/tmp/.X11-unix \
	       	--rm --entrypoint "make" \
		-ti aarch64-test "foundation"

foundation: $(IMAGE)
	@cp kernel.elf kernel.axf
	@rm -f *\.so*
	@chmod +x ./fvp/*
	@ln -s fvp/*so* .
	@./fvp/Foundation_Platform --cores=1 --image kernel.axf

clean:
	rm -f $(IMAGE) *.o *.so.* *.list *.sym

test:
	@tmux new-session 'make qemu' \; \
		split-window 'make gdb' \; \
		select-layout even-horizontal

.PHONY: all qemu clean test

SRC_DIR ?= ./
OBJ_DIR ?= ./
SOURCES ?= $(shell find $(SRC_DIR) -name '*.c' -or -name '*.S')
OBJECTS ?= $(addsuffix .o, $(basename $(notdir $(SOURCES))))
LINKER ?= $(SRC_DIR)/dtekv-script.lds

TOOLCHAIN ?= riscv32-unknown-elf-
CFLAGS ?= -Wall -O3 -mabi=ilp32 -march=rv32imzicsr
LDFLAGS ?= -lc -lgcc  # Adjust as needed

build: clean main.bin

main.elf:
	$(TOOLCHAIN)gcc $(CFLAGS) -c $(SOURCES)
	$(TOOLCHAIN)gcc $(CFLAGS) -T $(LINKER) -o $@ $(OBJECTS) softfloat.a $(LDFLAGS)

main.bin: main.elf
	$(TOOLCHAIN)objcopy --output-target binary $< $@
	$(TOOLCHAIN)objdump -D $< > $<.txt

clean:
	rm -f *.o *.elf *.bin *.txt

TOOL_DIR ?= ./tools
run: main.bin
	make -C $(TOOL_DIR) "FILE_TO_RUN=$(CURDIR)/$<"

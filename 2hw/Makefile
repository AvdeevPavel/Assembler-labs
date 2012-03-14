SOURCES = kernel.S kernel_interrupts.S kernel_data.S kernel_display.S
OBJS = $(SOURCES:.S=.o)

LD = ld
COMMON_LDFLAGS =  -Ttext 0x0
LDFLAGS_BIN = --oformat binary $(COMMON_LDFLAGS)
LDFLAGS_ELF = $(COMMON_LDFLAGS)

CC = gcc

PRODUCT_ELF = kernel
PRODUCT_BIN = kernel.mnt

all: $(OBJS)
	$(LD) $(LDFLAGS_ELF) -o $(PRODUCT_ELF) $(OBJS)
	$(LD) $(LDFLAGS_BIN) -o $(PRODUCT_BIN) $(OBJS)

rebuild: clean all

$(OBJS): $(SOURCES) 
	$(CC) -c  $(SOURCES)

clean:
	rm -f *~
	rm -f $(OBJS) $(PRODUCT_ELF) $(PRODUCT_BIN)

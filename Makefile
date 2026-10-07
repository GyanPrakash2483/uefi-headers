CC			= clang
CFLAGS		?= -std=c23 -Wall -Wextra -O2
CPPFLAGS	+=  -Iinclude

EFI_CFLAGS	:= -target x86_64-unknown-windows \
			   -ffreestanding -fshort-wchar -mno-red-zone \
			   -fno-stack-protector -mno-stack-arg-probe

EFI_LDFLAGS	:= -nostdlib -fuse-ld=lld-link \
			   -Wl,-subsystem:efi_application -Wl,-entry:efi_main

SRCS := $(wildcard examples/*.c)
BINS := $(patsubst examples/%.c,build/%.efi,$(SRCS))

.PHONY: all clean

all: $(BINS)

build/%.efi: examples/%.c | build
	$(CC) $(CPPFLAGS) -MMD -MP -MF $(@:.efi=.d) -MT $@ \
	      $(EFI_CFLAGS) $(CFLAGS) $< -o $@ $(EFI_LDFLAGS) $(LDFLAGS)

build:
	mkdir -p build

clean:
	rm -rf build

-include $(BINS:.efi=.d)
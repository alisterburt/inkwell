# RISC-V frame spike

Checks that allocation-free Mojo can run on a RISC-V microcontroller CPU with no
Mojo runtime. `fill.mojo` fills a full-size 4-bit frame, and `start.c` is a
minimal no-libc harness that calls it and prints the results.

Tested with Mojo 1.2.0, clang, lld, and `qemu-riscv32`.

```sh
mojo build --target-triple riscv32-unknown-none-elf --target-cpu sifive-e31 \
  --emit object -o fill.o fill.mojo
clang --target=riscv32 -march=rv32imac -mabi=ilp32 -O1 -ffreestanding \
  -fno-stack-protector -c start.c -o start.o
ld.lld -m elf32lriscv -e _start -static start.o fill.o -o fill.elf
qemu-riscv32 fill.elf
```

Expected output:

```text
167553360
50
169
```

`llvm-nm -u fill.o` prints nothing, because the object needs no external symbols.

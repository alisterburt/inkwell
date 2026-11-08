# Mojo on RISC-V

inkwell's firmware runs on an ESP32-C5, a 32-bit RISC-V microcontroller. This
records what's known and what's left.

## Why RISC-V

The compiler already supports it. Mojo's test suite covers
`riscv32-unknown-none-elf` and `riscv32-unknown-unknown`, and `RISCV` is in the
LLVM targets it builds. Xtensa, used by the ESP32-S3 and older chips, isn't, so
those would need a new compiler backend.

## Status

| Piece | Status |
| --- | --- |
| Compile Mojo to RISC-V objects | Works in Mojo 1.2.0 |
| Run allocation-free Mojo | Works under emulation; see [spike](../spikes/riscv-frame) |
| `String`, `List`, heap, `print` | Need a replacement runtime |
| Link into an ESP-IDF firmware | Not attempted |
| Run on real ESP32-C5 hardware | Not attempted |

## Spike result

A function that fills a full 1872×1404 packed frame was built for
`riscv32-unknown-none-elf` with `--target-cpu sifive-e31` (RV32IMAC), and run
under `qemu-riscv32`.

- The checksum and sampled bytes matched an independent calculation.
- The compiled function is 76 bytes.
- It has no undefined symbols.

A normal program with `main` and `print` needs `KGEN_CompilerRT_*` functions from
the Mojo runtime, which don't exist on this chip.

## Plan

1. **Confirm flags.** Check the exact `-march` and ABI ESP-IDF uses for the C5
   against what `mojo build` emits, especially its atomics.
2. **Link.** Add a Mojo object to an ESP-IDF component and call it from
   `app_main`.
3. **Bind.** Declare the ESP-IDF functions needed through `external_call`.
4. **Handle the runtime.** In order of preference:
   - Write allocation-free code wherever practical.
   - Stub or implement the small set of symbols that are still required, mapping
     allocation to ESP-IDF's heap.
   - Work toward a proper freestanding mode in Mojo.

## Prior art

Someone has [run pure Mojo on an RP2040](https://forum.modular.com/t/bare-metal-mojo-on-a-4-microcontroller-a-pure-mojo-rp2040-firmware-sdk/3298)
by stubbing the runtime symbols. That project had to retarget LLVM IR to reach
32-bit ARM, which RISC-V doesn't need. It also noted that Mojo has no supported
freestanding profile.

## Upstream opportunity

A documented freestanding mode would make this much cleaner: a way to build
without the hosted runtime, with an explicit allocator hook.

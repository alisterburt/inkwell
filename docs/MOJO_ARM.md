# Mojo on ARMv8.0

Mojo 1.2.0 programs don't start on the Raspberry Pi Zero 2 W, Pi 3, or Pi 4.
This records what's known and the proposed fix in `modular/modular`.

The battery-powered frame uses a Zero 2 W, so this fix is required for the
product.

## Test results

A small program was cross-compiled with `--target-cpu cortex-a53`, linked against
the runtime from the aarch64 Mojo wheel, and run under `qemu-aarch64`.

| Emulated CPU | Boards | Result |
| --- | --- | --- |
| Cortex-A53 | Pi Zero 2 W, Pi 3 | Illegal instruction |
| Cortex-A72 | Pi 4 | Illegal instruction |
| Cortex-A76 | Pi 5 | Runs |
| Neoverse N1 | Graviton2 class | Runs |

Emulation tests the instruction set only. None of this has run on real hardware.

## Cause

The program's own object code contains no LSE atomics. The three prebuilt runtime
libraries do:

| Library | LSE atomics | `ldapr` | `udot` |
| --- | --- | --- | --- |
| `libKGENCompilerRTShared.so` | 534 | 575 | 20 |
| `libAsyncRTRuntimeGlobals.so` | 519 | 307 | 8 |
| `libMSupportGlobals.so` | 5 | 3 | 0 |

All Linux aarch64 C++ in the repo is built with `-mcpu=neoverse-n1`, at
`bazel/internal/cc-toolchain/args/BUILD.bazel:145`. The exact faulting
instruction wasn't captured, but the A53 and A72 lack all of these.

The Mojo compiler binary also crashes on the A53. That's out of scope: compile
on another machine and ship the binary.

## Proposed fix

1. **Prove it.** On a throwaway branch, build the three libraries with
   `-mcpu=cortex-a53` on arm64 Linux. Also change `--target-cpu` at
   `bazel/internal/BUILD.bazel:173` if any Mojo code ends up in them. Swap them
   into the wheel and rerun the test on an emulated A53. This hasn't been done.
2. **Add a baseline build setting**, defaulting to `neoverse-n1` so existing
   builds don't change.
3. **Decide packaging.** Outline atomics let one binary use LSE when available.
   Measure the cost on N1, especially in AsyncRT. If it's negligible, ship one
   set of libraries. If not, ship a separate ARMv8.0 runtime package.
4. **Guard it** with a CI smoke test under `qemu-aarch64 -cpu cortex-a53`, and
   update the system requirements.

Why not simpler options:

- Lowering the flag globally would remove these instructions from the compiler
  and MAX.
- Per-target flags don't reach dependencies such as tcmalloc and absl.
- A Bazel transition would fork those dependencies, and `rules_cc` rejects the
  same dynamic library in two configurations.

## Related limitation

tcmalloc assumes a 48-bit virtual address space. A forum user hit a crash from
this on 64-bit Raspberry Pi OS. Use Ubuntu Server 24.04, or rebuild with
`TCMALLOC_ADDRESS_BITS`. Test this on the real board.

## Contribution notes

`Mojo/lib/CompilerRT`, `AsyncRT`, and `bazel/` aren't listed as open to external
contributions in the repo's `CLAUDE.md`. Its AI policy requires an `Assisted-by:`
trailer and a human-written PR description, and commits need `-s`.

## Sources

- [Mojo system requirements](https://mojolang.org/docs/manual/install/)
- [Cross-compilation docs](https://mojolang.org/docs/tools/compilation/)
- [Raspberry Pi install thread](https://forum.modular.com/t/raspberry-pi-installation-failing/2001)

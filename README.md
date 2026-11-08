# inkwell

A wall-mounted e-ink picture frame you manage from your phone.

![Concept render of the frame](docs/images/frame_hero.png)

*Concept render at the panel's real dimensions. The picture is a drawn
placeholder, processed through a simulation of the panel.*

## How it works

1. Press the button on the top edge of the frame.
2. The frame wakes up and shows a loading overlay.
3. When its web server is up, it shows a QR code.
4. Scan it to open a page served by the frame.
5. Add, crop, reorder, or delete photos on your phone.
6. Tap **Send to frame**.
7. The frame shuts its web server down and goes back to deep sleep.

## Principles

- **Looks like a print.** No status lights or visible electronics on the front.
- **No power cable.** Battery powered and rechargeable over USB-C. The frame
  sleeps between updates, and the web server only runs while someone is
  managing photos.
- **Local first.** Photos stay on the frame, with no cloud service or accounts
  in the first version.
- **Mojo on a microcontroller, no Python.** Firmware logic is Mojo running on a
  small RISC-V chip, on top of Espressif's ESP-IDF for WiFi and drivers. The
  phone page is HTML and JavaScript.

## Status

Planning, with one early result: Mojo code compiled for RV32IMAC ran correctly
under emulation with no runtime symbols. See
[`spikes/riscv-frame`](spikes/riscv-frame). No hardware has been bought yet.

## Docs

| Document | Contents |
| --- | --- |
| [Plan](docs/PLAN.md) | Milestones, exit criteria, open decisions |
| [Hardware](docs/HARDWARE.md) | Parts, prices, power, frame and enclosure design |
| [Architecture](docs/ARCHITECTURE.md) | Firmware components and data flow |
| [Mojo on RISC-V](docs/MOJO_RISCV.md) | Status and plan for running Mojo on the ESP32-C5 |
| [Archive](docs/archive/MOJO_ARM.md) | Findings from the earlier Raspberry Pi approach |

# eink-frame

A wall-mounted e-ink picture frame you manage from your phone. It's a working
name.

![Concept render of the frame](docs/images/frame_hero.png)

*Concept render at the panel's real dimensions. The picture is a drawn
placeholder, processed through a simulation of the panel.*

## How it works

1. Press the button on the top edge of the frame.
2. The frame draws a QR code on its own screen.
3. Scan it to open a page served by the frame.
4. Add, crop, reorder, or delete photos on your phone.
5. Tap **Send to frame**.

The same hardware can also show a one-week calendar.

## Principles

- **Looks like a print.** No status lights or visible electronics on the front.
- **Responds immediately.** A button press gets a visible response in under a
  second.
- **Local first.** Photos stay on the frame, with no cloud service or accounts
  in the first version.
- **Mojo on the device, no Python.** The phone page is HTML and JavaScript.
  Everything on the frame is Mojo plus a few C libraries through FFI.

## Status

Planning. No hardware has been bought and no firmware has been written.

## Docs

| Document | Contents |
| --- | --- |
| [Plan](docs/PLAN.md) | Milestones, exit criteria, open decisions |
| [Hardware](docs/HARDWARE.md) | Parts, prices, frame and enclosure design |
| [Architecture](docs/ARCHITECTURE.md) | Software components and data flow |
| [Mojo on ARMv8.0](docs/MOJO_ARM.md) | Why Mojo doesn't run on a Pi Zero 2 W, and the fix |

# Plan

## Goal

A frame that someone would believe was bought, not built, and that a
non-technical person can update from their phone.

## Not in the first version

- Remote access or a cloud service
- User accounts
- Battery power
- Color

## Milestones

Each milestone ends with something that can be checked.

### M0: Renderer on a dev machine

No hardware needed.

- Draw shapes and text in Mojo into a 1872×1404, 4-bit buffer.
- Write the buffer to a PNG for preview.
- Exit: the calendar layout and a photo render correctly to PNG, using FreeType
  over FFI for text.

### M1: Panel bring-up

- Buy the panel and a Pi. See [Hardware](HARDWARE.md).
- Install Ubuntu Server 24.04.
- Write the IT8951 driver in Mojo using `spidev` and GPIO.
- Exit: a Mojo binary built off the device draws a test image on the panel in
  16 greys, and the stock Mojo runtime is confirmed on real hardware.

### M2: Photo loop on the home network

- HTTP server in Mojo.
- Phone page: pick, crop, preview, delete, reorder.
- Phone converts each photo to a panel frame and uploads it.
- Scheduled rotation.
- Exit: a photo goes from a phone to the panel over WiFi, and the phone preview
  matches the panel.

### M3: Button and QR

- Momentary switch on GPIO, with debounce.
- QR generator and expiring access token.
- QR drawn with a fast partial update, then cleaned up with a full-quality
  refresh.
- Exit: the QR appears within a second of a press, and an expired token is
  rejected.

### M4: Enclosure

See the staged approach in [Hardware](HARDWARE.md#frame-and-enclosure).

- Exit: a finished frame hangs on a wall with one thin cable and no visible
  electronics.

### M5: Calendar mode

- Fetch events using `libcurl` over FFI and a JSON parser.
- Reuse the week layout from M0, including side-by-side overlapping events.
- Exit: the week view updates within minutes of a calendar change.

### M6: Setup for other people

Only needed if a frame will be given to someone else.

- Hotspot setup mode for entering WiFi details from a phone.
- Exit: someone can set up a frame without a computer.

### Parallel track: Mojo on ARMv8.0

See [Mojo on ARMv8.0](MOJO_ARM.md). This unlocks the Pi Zero 2 W and doesn't
block M0 to M3, which can run on a Pi 5.

## Open decisions

| # | Decision | Options | Leaning | Blocks |
| --- | --- | --- | --- | --- |
| 1 | Compute board | Pi 5 now, or wait for the runtime fix and use a Zero 2 W | Develop on Pi 5, target Zero 2 W for the thin frame | M1, M4 |
| 2 | Panel size | 7.8" or 10.3", both 1872×1404 | 7.8" for the prototype | M1 |
| 3 | Access | Home network only, or remote too | Home only for the first version | M2 |
| 4 | Frame size and mat | 8×10" with wide mat, or a tighter frame | Decide after seeing the real panel | M4 |
| 5 | Calendar source | Google, or iCloud/iCal | Google, because its API expands recurring events | M5 |
| 6 | Photo dithering | None, or fine dither | Fine dither for photos, none for text | M2 |

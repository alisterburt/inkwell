# Plan

## Goal

A cable-free frame that someone would believe was bought, not built, and that a
non-technical person can update from their phone.

## Not in the first version

- Remote access or a cloud service
- User accounts
- Color

## Decided

| Decision | Choice | Why |
| --- | --- | --- |
| Power | Battery, with the Pi fully off between updates | No visible cable, and the web server isn't always running |
| Compute | Raspberry Pi Zero 2 W in the product, Pi 5 for early development | Keeps Mojo on the device and fits a thin frame |
| Button experience | Press, loading screen, then QR when the server is up | Slower to respond, accepted in exchange for battery power |

## Milestones

Each milestone ends with something that can be checked.

### M0: Renderer on a dev machine

No hardware needed.

- Draw shapes and text in Mojo into a 1872×1404, 4-bit buffer.
- Write the buffer to a PNG for preview.
- Exit: the calendar layout and a photo render correctly to PNG, using FreeType
  over FFI for text.

### M1: Panel bring-up

- Buy the panel and a Pi 5. See [Hardware](HARDWARE.md).
- Install Ubuntu Server 24.04.
- Write the IT8951 driver in Mojo using `spidev` and GPIO.
- Exit: a Mojo binary built off the device draws a test image on the panel in
  16 greys, and the stock Mojo runtime is confirmed on real hardware.

### M2: Photo loop on the home network

- HTTP server in Mojo.
- Phone page: pick, crop, preview, delete, reorder.
- Phone converts each photo to a panel frame and uploads it.
- Exit: a photo goes from a phone to the panel over WiFi, and the phone preview
  matches the panel.

### M3: Button, loading screen, and QR

Developed on an always-powered Pi.

- Momentary switch on GPIO, with debounce.
- Loading overlay as soon as the program starts, before WiFi.
- QR generator and expiring access token.
- QR overlay once the server is reachable.
- Session ends on "Done" or after inactivity, the photo is restored with a
  full-quality refresh, and the server stops.
- Exit: the full sequence works repeatedly, and an expired token is rejected.

### M4: Battery and power control

Requires the [Mojo ARMv8.0 runtime fix](MOJO_ARM.md).

- Move to a Zero 2 W, power controller, battery, and boost stage.
- Timer wake: draw the next photo with no WiFi, then power off.
- Button wake: run the session from M3, then power off.
- Power off the IT8951 board along with the Pi.
- Reduce boot time, moving to a minimal OS if needed.
- Low-battery warning on the phone page.
- Exit: measured press-to-QR time, with a target under 15 seconds. Measured
  energy per photo change and per session, and a battery-life estimate based on
  those numbers.

### M5: Enclosure

See the staged approach in [Hardware](HARDWARE.md#frame-and-enclosure).

- Exit: a finished frame hangs on a wall with no cable and no visible
  electronics, and can be recharged without tools.

### M6: Calendar mode

- Fetch events using `libcurl` over FFI and a JSON parser.
- Reuse the week layout from M0, including side-by-side overlapping events.
- Exit: the week view updates on a schedule of a few times a day.

### M7: Setup for other people

Only needed if a frame will be given to someone else.

- Hotspot setup mode for entering WiFi details from a phone.
- Exit: someone can set up a frame without a computer.

### Parallel track: Mojo on ARMv8.0

See [Mojo on ARMv8.0](MOJO_ARM.md). It doesn't block M0 to M3, which can run on a
Pi 5, but M4 depends on it.

## Open decisions

| # | Decision | Options | Leaning | Blocks |
| --- | --- | --- | --- | --- |
| 1 | Panel size | 7.8" or 10.3", both 1872×1404 | 7.8" for the prototype | M1 |
| 2 | Access | Home network only, or remote too | Home only for the first version | M2 |
| 3 | Photo dithering | None, or fine dither | Fine dither for photos, none for text | M2 |
| 4 | Power controller | Witty Pi 4, or a smaller custom board | Witty Pi 4 to prototype | M4 |
| 5 | Operating system | Ubuntu Server, or a minimal Buildroot image | Start on Ubuntu, move if boot time requires it | M4 |
| 6 | Frame size and mat | 8×10" with wide mat, or a tighter frame | Decide after seeing the real panel | M5 |
| 7 | Calendar source | Google, or iCloud/iCal | Google, because its API expands recurring events | M6 |

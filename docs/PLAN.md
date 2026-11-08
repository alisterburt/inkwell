# Plan

## Goal

A cable-free frame that someone would believe was bought, not built, and that a
non-technical person can update from their phone.

## Not in the first version

- Remote access or a cloud service
- User accounts
- Color
- Calendar mode. See [Later](#later).

## Decided

| Decision | Choice | Why |
| --- | --- | --- |
| Power | Battery, with the Pi fully off between updates | No visible cable, and the web server isn't always running |
| Compute | Raspberry Pi Zero 2 W from the start, no Pi 5 | Develop on the hardware that ships, with Mojo on the device |
| Button experience | Press, loading screen, then QR when the server is up | Slower to respond, accepted in exchange for battery power |
| Panel | Waveshare 7.8", 1872×1404 | Matches the concept render and keeps the frame compact |

## Milestones

Each milestone ends with something that can be checked.

### M0: Frame pipeline on a dev machine

No hardware needed.

- Browser code that crops a photo and converts it to a 1872×1404, 4-bit frame.
- Mojo code that loads a frame, draws the loading and QR overlays, and writes a
  PNG for preview.
- Exit: a real photo goes through the full pipeline, and the PNG preview matches
  the browser preview.

### M1: Panel bring-up

Requires the [Mojo ARMv8.0 runtime build](MOJO_ARM.md).

- Buy the panel and a Zero 2 W. See [Hardware](HARDWARE.md).
- Install Ubuntu Server 24.04.
- Write the IT8951 driver in Mojo using `spidev` and GPIO.
- Exit: a Mojo binary built off the device draws a test image on the panel in
  16 greys, and the rebuilt Mojo runtime is confirmed on real hardware.

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

- Add a power controller, battery, and boost stage.
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

### M6: Setup for other people

Only needed if a frame will be given to someone else.

- Hotspot setup mode for entering WiFi details from a phone.
- Exit: someone can set up a frame without a computer.

### Prerequisite for M1: Mojo on ARMv8.0

Build the runtime libraries for the Zero 2 W. See [Mojo on ARMv8.0](MOJO_ARM.md).
M0 doesn't depend on it.

## Open decisions

| # | Decision | Options | Leaning | Blocks |
| --- | --- | --- | --- | --- |
| 1 | Access | Home network only, or remote too | Home only for the first version | M2 |
| 2 | Photo dithering | None, or fine dither | Fine dither for photos, none for text | M2 |
| 3 | Power controller | PiSugar family, Witty Pi 4 with separate charger and boost, or a custom board | Test a PiSugar first for simplicity; check standby draw and pack size | M4 |
| 4 | Operating system | Ubuntu Server, or a minimal Buildroot image | Start on Ubuntu, move if boot time requires it | M4 |
| 5 | Frame size and mat | 8×10" with wide mat, or a tighter frame | Decide after seeing the real panel | M5 |

## Later

**Calendar mode.** A one-week view on the same hardware. It would add text
rendering, HTTPS, and JSON. Notes for when it comes back:

- Prefer Google Calendar with `singleEvents=true` and a `timeZone` parameter, so
  Google expands recurring events and returns local times.
- On battery, update a few times a day. Polling every few minutes would drain it
  within weeks to a few months.
- Text would use FreeType through FFI, and HTTPS would use `libcurl`.

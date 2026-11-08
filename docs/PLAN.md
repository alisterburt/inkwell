# Plan

## Goal

A cable-free frame that someone would believe was bought, not built, and that a
non-technical person can update from their phone, with its firmware written in
Mojo.

## Not in the first version

- Remote access or a cloud service
- User accounts
- Color
- Calendar mode. See [Later](#later).

## Decided

| Decision | Choice | Why |
| --- | --- | --- |
| Compute | Seeed XIAO ESP32-C5 | RISC-V, which Mojo can target, with 8 MB PSRAM, built-in LiPo charging, and very low sleep current |
| Language | Mojo on top of ESP-IDF | WiFi and drivers stay in Espressif's C code. C is the fallback for anything that resists |
| Power | Battery with USB-C charging, deep sleep between updates | No visible cable, and the web server isn't always running |
| Button experience | Press, full loading screen, then QR screen when the server is up | The QR needs the frame's network address anyway |
| Panel | Waveshare 7.8", 1872×1404 | Matches the concept render and keeps the frame compact |

## Milestones

Each milestone ends with something that can be checked.

### M0: Frame pipeline on a dev machine

No hardware needed.

- Browser code that crops a photo and converts it to a 1872×1404, 4-bit frame.
- Mojo code that loads a frame, draws the loading and QR screens, and writes a
  PNG for preview. Keep it free of heap use where practical, so it moves to the
  chip unchanged.
- Exit: a real photo goes through the full pipeline, and the PNG preview matches
  the browser preview.

### M1: Mojo on the real chip

See [Mojo on RISC-V](MOJO_RISCV.md).

- Buy the board. See [Hardware](HARDWARE.md).
- Create an ESP-IDF project with a Mojo object linked in.
- Call Mojo from C, and C from Mojo.
- Decide how to handle the Mojo runtime.
- Exit: a Mojo function runs on the XIAO and logs over USB, in a firmware built
  from a repeatable script.

### M2: Panel bring-up

- Wire the IT8951 HAT to the XIAO, with the HAT set to SPI mode.
- Write the IT8951 driver in Mojo, over ESP-IDF's SPI driver.
- Exit: a frame streamed from storage appears on the panel in 16 greys.

### M3: Photo loop on the home network

- WiFi and HTTP through ESP-IDF, with request handlers in Mojo.
- Phone page served from flash: pick, crop, preview, delete, reorder.
- Uploads streamed straight to storage.
- Exit: a photo goes from a phone to the panel over WiFi, and the phone preview
  matches the panel.

### M4: Button, deep sleep, and QR

- Deep sleep, with wake on button and on timer.
- Timer wake: draw the next photo at least once a day with no WiFi, then sleep.
- Button wake: loading screen, WiFi, server, QR screen.
- QR generator and expiring access token.
- Failure screen if WiFi doesn't connect.
- Session ends on "Done" or after inactivity, the photo is restored at full
  quality, and the chip sleeps.
- Exit: the full sequence works repeatedly, an expired token is rejected, and
  press-to-QR time is measured.

### M5: Battery and power

- Battery, with a 5 V boost for the panel that's fully off during sleep.
- Power off the SD card during sleep.
- Low-battery warning on the phone page.
- Exit: measured sleep current and energy per photo change and per session, and
  a battery-life estimate based on those numbers.

### M6: Enclosure

See the staged approach in [Hardware](HARDWARE.md#frame-and-enclosure).

- Exit: a finished frame hangs on a wall with no cable and no visible
  electronics, and recharges without tools.

### M7: Setup for other people

Development builds have WiFi details flashed in, so this comes last.

- Hotspot setup mode for entering WiFi details from a phone. See
  [Architecture](ARCHITECTURE.md#first-time-wifi-setup).
- Long press to forget the network.
- Exit: someone can set up a frame from a phone, with no computer or app.

## Open decisions

| # | Decision | Options | Leaning | Blocks |
| --- | --- | --- | --- | --- |
| 1 | Mojo runtime | Stub it, avoid the heap, or add a proper freestanding mode | Avoid the heap and stub the rest first; pursue freestanding upstream | M1 |
| 2 | Photo storage | microSD, or the 8 MB flash | microSD, because flash only holds a few frames | M2 |
| 3 | Access | Home network only, or remote too | Home only for the first version | M3 |
| 4 | Portrait photos | Crop to landscape, add paper-colored bars, or hang the frame either way | Hang either way as a setting, with bars for mismatched photos | M3 |
| 5 | Photo dithering | None, or fine dither | Fine dither for photos, none for text | M3 |
| 6 | Battery size | About 2,000 to 5,000 mAh | Decide from M5 measurements and available depth | M5 |
| 7 | Frame size and mat | 8×10" with wide mat, or a tighter frame | Decide after seeing the real panel | M6 |

## Later

**Calendar mode.** A one-week view on the same hardware. It would add text
rendering, HTTPS, and JSON. Notes for when it comes back:

- Prefer Google Calendar with `singleEvents=true` and a `timeZone` parameter, so
  Google expands recurring events and returns local times.
- Battery cost is mostly WiFi and TLS on each fetch, so update a few times a
  day.

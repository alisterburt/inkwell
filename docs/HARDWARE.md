# Hardware

Prices checked 2026-11-08, before tax and shipping unless noted.

## Parts

| Part | Choice | Price | Notes |
| --- | --- | --- | --- |
| Panel and driver | Waveshare 7.8" e-Paper HAT | $143.99 (1-9 units) | Includes the IT8951 driver board |
| Compute | Seeed XIAO ESP32-C5, pre-soldered for development | $6.90 at Seeed; $8.31 including US tariffs at RobotShop | Few units in stock at RobotShop when checked |
| Storage | microSD card and SPI breakout | Not priced | Shares the SPI bus |
| Battery | Flat 3.7 V LiPo with protection, 2,000-5,000 mAh | Not priced | Connects to the XIAO's battery pads |
| Panel power | 5 V boost converter with an enable pin | Not priced | Must be fully off during sleep |
| Button | Tactile switch and machined cap | A few dollars, more for custom caps | Alps or Omron class switch |
| Prototyping | Jumper wires, breadboard | A few dollars | The HAT doesn't plug into the XIAO |
| Frame | See below | Roughly $40-80 | Estimate |

Charging needs no extra parts, since the XIAO's USB-C port and charger handle it.

## Compute: XIAO ESP32-C5

| Property | Value |
| --- | --- |
| CPU | 32-bit RISC-V, up to 240 MHz |
| Memory | 384 KB SRAM, 8 MB PSRAM |
| Flash | 8 MB |
| WiFi | Dual-band WiFi 6 |
| Size | 21×17.8 mm |
| Power and programming | USB-C |
| Charging | Built in, for 3.7 V LiPo |
| Deep sleep | 15 µA on battery, 375 µA on USB (listed) |
| GPIO | 11 on headers, 5 on back pads |

To check: which pins can wake the chip from deep sleep, since the button must use
one of them.

## Panel: Waveshare 7.8"

| Property | Value |
| --- | --- |
| Resolution | 1872×1404 |
| Greys | 2 to 16 (1 to 4 bit) |
| Active area | 158.18×118.64 mm |
| Outline | 173.8×127.6×0.78 mm |
| Dot pitch | 0.0845 mm, about 300 ppi |
| Full refresh | 450 ms listed |
| Partial refresh | About 0.3 s listed |
| Refresh power | 1.2 W typical, listed |
| Controller | IT8951 |
| HAT supply | 5 V |

Handling notes:

- The glass and ribbon cable are fragile. Waveshare recommends reinforcing the
  ribbon with tape during development.
- Each panel has its own VCOM calibration voltage, which must be set to the
  printed value.
- Waveshare rates the panel for about 1,000,000 refreshes, recommends redrawing
  at least every 24 hours, and says not to update it in direct sunlight.

## Wiring

The HAT is designed for a Raspberry Pi header, so it's wired by hand. Set its
switch to SPI mode.

| Signal | Connects to |
| --- | --- |
| SCK, MOSI, MISO | XIAO SPI pins, shared with microSD |
| CS | XIAO GPIO |
| HRDY (busy) | XIAO GPIO |
| RST | XIAO GPIO |
| 5 V | Boost converter output |
| GND | Common ground |

Other GPIOs: microSD CS, button, boost enable. That's 9 of the 11 header pins.

The button goes between a wake-capable GPIO and ground.

Compatibility notes:

- Waveshare only documents Raspberry Pi and STM32 hosts for this HAT, so the
  XIAO ESP32-C5 combination is unproven. Test it early, in M2.
- Other projects report 3.3 V data lines with a 5 V supply on this HAT family.
- The 5 V supply has to be exact and strong. A USB 5 V rail through a diode is
  reported not to work, so use a proper regulated supply.
- Waveshare lists about 0.1 W standby, so the HAT must lose power during sleep.
- Connect all cables before powering on, since the HAT can't be hot-plugged.
- MISO is required, because the driver reads registers from the IT8951.

## Power

A 5,000 mAh LiPo holds about 18.5 Wh. Deep sleep at 15 µA and 3.7 V uses about
1.3 mWh per day, so sleep is close to negligible as long as the panel, its boost,
and the SD card are truly off.

Rough energy budget, to be replaced by M5 measurements:

| Event | Estimate |
| --- | --- |
| Daily photo change | A few mWh |
| Five-minute phone session with WiFi on | Tens of mWh |

That points to a battery life of a year or more, likely limited by the cell's
self-discharge, which may allow a smaller and thinner battery.

## Frame and enclosure

**Decision: yes, custom eventually, but not first.** Most of the custom work is
the part you can't see: something has to hold the panel, boards, and button
precisely. The wood should wait until the electronics have been measured.

### Stage 1: Retail frame

- Off-the-shelf 8×10" shadow-box frame.
- Custom-cut mat from a framer.
- Drill the button hole by hand.
- Purpose: judge size, mat color, depth, and viewing distance cheaply.

### Stage 2: Custom internal carrier

- 3D-printed or laser-cut carrier that holds the panel, boards, button, and
  charging port. A makerspace or online print service avoids needing a printer.
- Stiff backing board with no pressure on the glass or ribbon.
- Purpose: get the internals right. The carrier can later move to a nicer frame
  unchanged.

### Stage 3: Custom wood frame

- Commission from a framer or small CNC shop, built around the measured carrier.
- Walnut or similar, with a machined bore for the button on the top edge.
- Machined brass or aluminum cap, sitting slightly proud with a thin shadow gap.

### Dimensions used in the render

| Item | Size |
| --- | --- |
| Mat outer | 254×203 mm (10×8") |
| Rail width | 24 mm |
| Frame outer | 302×251 mm |
| Frame depth | 40 mm |
| Button | 10.4 mm diameter, on the top edge toward the right |

### Constraints to resolve

- The mat opening should be slightly smaller than the 158×119 mm active area so
  no panel edge shows. The render uses the exact size.
- The HAT is the tallest electronics part. Measure the stack before committing
  to a depth, which may be well under 40 mm.
- The panel is greyer than a white mat. Test warmer or darker mats.
- USB-C charging port on the bottom edge, recessed, so the frame can charge
  while hanging.
- LiPo cells can swell. Give the battery its own pocket, clear of the panel.
- No glass in front at first, since reflections hurt a low-contrast panel. Test
  it with museum glass if protection is needed.

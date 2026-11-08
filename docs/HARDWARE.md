# Hardware

Prices checked 2026-11-08, before tax, shipping, and tariffs.

## Parts

| Part | Choice | Price | Notes |
| --- | --- | --- | --- |
| Panel and driver | Waveshare 7.8" e-Paper HAT | $143.99 (1-9 units) | Includes the IT8951 driver board |
| Compute, development | Raspberry Pi 5, 1 GB | $45 | Runs stock Mojo in emulation; compile elsewhere; not used in the product |
| Compute, product | Raspberry Pi Zero 2 W with header | $19.80 | Needs the [runtime fix](MOJO_ARM.md); out of stock at Adafruit when checked |
| Storage | microSD card | Not priced | |
| Power controller | Witty Pi 4 or similar | About $33 | Timer and button wake, 0.5 mA standby listed |
| Battery | Flat LiPo, about 5,000 mAh | Not priced | Needs protection circuitry |
| Boost stage | LiPo to 5 V | Not priced | The Witty Pi 4 takes 5 V or 6-30 V, not a single cell directly |
| Charging | USB-C on the back, with a charge controller | Not priced | |
| Button | Tactile switch and machined cap | A few dollars, more for custom caps | Alps or Omron class switch |
| Frame | See below | Roughly $40-80 | Estimate |

Alternative panel: Waveshare 10.3" HAT at about $200, with the same resolution
over a larger area.

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
| Controller | IT8951 |
| Interface used | SPI |

Handling notes:

- The glass and ribbon cable are fragile. Waveshare recommends reinforcing the
  ribbon with tape during development.
- Each panel has its own VCOM calibration voltage, which must be set to the
  printed value.

Panels with Spectra 6 color were rejected: they can't show grey and take about
25 seconds to refresh.

## Button wiring

One momentary switch between a free GPIO pin and ground, using the internal
pull-up. Confirm which pins the HAT uses before choosing one. The HAT covers the
Pi's header, so solder to the underside of the pins or use a stacking header.

The button must also wake the power controller while the Pi is off. To check on
the real board: how a tap behaves while the Pi is running, since the Witty Pi 4
treats it as a graceful shutdown, and how the software tells a button wake from
a timer wake.

## Power

| State | Draw | Source |
| --- | --- | --- |
| Zero 2 W idle, WiFi on | About 600 mW | CNX Software measurement |
| Zero 2 W during WiFi transfer | About 187 mA above idle, on average | CNX Software measurement |
| Off, controller standby | About 0.5 mA | Witty Pi 4 listing |
| Panel refresh | 1.2 W typical | Reseller listing |

A 5,000 mAh LiPo holds about 18.5 Wh. Always-on Linux would drain it in about a
day. With the Pi off between updates and a daily photo change, the estimate is
roughly six months, mostly lost to standby and conversion. This is an estimate
to be replaced by measurements in M4.

The IT8951 board's idle draw is unknown, so it has to lose power along with the
Pi.

## Frame and enclosure

**Decision: yes, custom eventually, but not first.** Most of the custom work is
the part you can't see: something has to hold the panel, boards, and button
precisely. The wood should wait until the electronics stack has been measured.

### Stage 1: Retail frame

- Off-the-shelf 8×10" shadow-box frame.
- Custom-cut mat from a framer.
- Drill the button hole by hand.
- Purpose: judge size, mat color, depth, and viewing distance cheaply.

### Stage 2: Custom internal carrier

- 3D-printed or laser-cut carrier that holds the panel, boards, button, and
  cable exit.
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
- The Zero 2 W, HAT, power controller, and battery all have to fit behind the
  panel. Measure the stack before committing to 40 mm.
- The panel is greyer than a white mat. Test warmer or darker mats.
- The USB-C charging port goes on the back, reachable by taking the frame off
  the wall.
- No glass in front at first, since reflections hurt a low-contrast panel. Test
  it with museum glass if protection is needed.

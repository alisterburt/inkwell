# Hardware

Prices checked 2026-11-08, before tax, shipping, and tariffs.

## Parts

| Part | Choice | Price | Notes |
| --- | --- | --- | --- |
| Panel and driver | Waveshare 7.8" e-Paper HAT | $143.99 (1-9 units) | Includes the IT8951 driver board |
| Compute, development | Raspberry Pi 5, 1 GB | $45 | Runs stock Mojo in emulation; compile elsewhere |
| Compute, thin frame | Raspberry Pi Zero 2 W with header | $19.80 | Needs the [runtime fix](MOJO_ARM.md); out of stock at Adafruit when checked |
| Storage | microSD card | Not priced | |
| Power | Supply for the chosen Pi | Not priced | USB-C for Pi 5, micro-USB for Zero 2 W |
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
- A Pi 5 with the HAT is a thick stack and may not fit in 40 mm. Mount it beside
  the panel or use a deeper frame.
- The panel is greyer than a white mat. Test warmer or darker mats.
- Power is one thin cable, or routed through the wall. Battery is impractical
  with Linux always running.
- No glass in front at first, since reflections hurt a low-contrast panel. Test
  it with museum glass if protection is needed.

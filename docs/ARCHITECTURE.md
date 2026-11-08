# Architecture

## Overview

```text
Phone browser                     Frame (XIAO ESP32-C5)
-------------                     ---------------------
Pick and crop photo
Convert to 16-grey frame
Upload frame + thumbnail  --->    HTTP handlers (Mojo over ESP-IDF)
                                    |
                                    v
                                  Photo store (microSD)
                                    |
                                    v
                                  Wake and session logic  <---  Button, timer
                                    |
                                    v
                                  IT8951 driver (Mojo, ESP-IDF SPI)
                                    |
                                    v
                                  Panel
```

## Firmware layers

| Layer | Written in | Provides |
| --- | --- | --- |
| Application | Mojo | Sessions, photo store, screens, QR, HTTP handlers |
| Panel driver | Mojo | IT8951 protocol |
| Runtime shim | Mojo or C | Whatever Mojo needs from a runtime. See [Mojo on RISC-V](MOJO_RISCV.md) |
| ESP-IDF | C, supplied by Espressif | WiFi, TCP/IP, HTTP server, SPI, SD, sleep, storage |

There's no Python on the device, and no operating system beyond ESP-IDF's
FreeRTOS.

## Power lifecycle

The chip is in deep sleep unless it has work to do. The web server only exists
during a session. The panel and SD card have no power during sleep.

**Timer wake** (scheduled photo change):

1. The chip wakes and powers the panel and SD card.
2. It streams the next photo to the panel at full quality.
3. It cuts power to them and sleeps. WiFi is never started.

**Button wake** (managing photos):

1. Press. The chip wakes immediately and powers the panel.
2. It draws a full "Waking up" screen from a bitmap.
3. WiFi connects, the server starts, and a token is generated.
4. A QR screen replaces the loading screen. It has to wait for this point because
   it contains the address the frame was just given.
5. The phone manages photos.
6. Session ends on "Done" or after inactivity. The server stops, the photo is
   restored at full quality, and the chip sleeps.

If WiFi doesn't connect within about a minute, the frame shows a failure message
and sleeps. Presses during a wake are ignored.

## First-time WiFi setup

The frame learns the home network from a phone, with no app.

1. With no saved network, a button press starts a temporary hotspot and shows a
   QR code that joins it. The password is random and only shown on the frame.
2. The phone joins, and a setup page served by the frame opens through a captive
   portal. A second QR opens it if the portal doesn't appear.
3. The person picks a network and enters its password.
4. The frame tests the connection with the hotspot still up, so the page can
   report a wrong password right away.
5. Details are saved in flash, the hotspot closes, and normal use begins.

Holding the button for about 10 seconds forgets the network. Setup mode times out
after a few minutes. Bluetooth provisioning was rejected because iPhone browsers
can't use it, which would require an app. Networks with their own login page
won't work.

During development, WiFi details are flashed with the firmware, so this can wait
until M7.

## Key decision: the phone does the image processing

The browser crops, resizes, converts to grey, dithers, and uploads the finished
frame.

- The frame needs no JPEG or HEIC decoder.
- The preview on the phone is what the panel will show, apart from the panel's
  lower contrast.
- Photo orientation and large camera files are handled before upload.

The cost is that changing the image processing later means re-uploading photos,
unless originals are kept. The first version doesn't keep them, so re-cropping
means picking the photo from the camera roll again.

## Phone page

The frame serves one self-contained, gzipped page from flash, with nothing loaded
from the internet. It runs in the phone's browser.

- A public HTTPS site can't call a plain local address, and an app would need
  installing, so the frame hosts the page itself.
- Plain HTTP rules out service workers, the screen wake lock, Web Share, and
  `crypto.subtle`. The photo picker, canvas, `fetch`, and WebAssembly all work.
- Photos are processed and uploaded one at a time with progress shown. Each
  upload resets the session timer.
- The page asks people to keep it open, since a phone can pause it when locked.
- Previews use the panel's real tones, with an auto-contrast option.
- Thumbnails come from the frame as one bundle, because ESP-IDF's HTTP server
  handles few connections at once.

## Frame format and memory

Raw 4-bit grey, 1872×1404, packed two pixels per byte: 1,314,144 bytes.

A frame fits in the 8 MB PSRAM, but it doesn't need to. Uploads stream from the
network to microSD, and displays stream from microSD to the IT8951, which has its
own frame memory.

## HTTP API sketch

Plain HTTP on the local network. Every `/api` call needs the access token.
Uploads are raw request bodies, which avoids multipart parsing.

| Method | Path | Purpose |
| --- | --- | --- |
| GET | `/` | Phone page, served from flash |
| GET | `/api/photos` | List photos and their order |
| PUT | `/api/photos/{id}/frame` | Upload the raw panel frame |
| PUT | `/api/photos/{id}/thumb` | Upload the thumbnail |
| GET | `/api/thumbs` | Get all thumbnails as one bundle |
| DELETE | `/api/photos/{id}` | Delete a photo |
| PUT | `/api/order` | Set the rotation order |
| POST | `/api/show/{id}` | Display a photo now |
| POST | `/api/done` | End the session |

## Access

- The QR contains the frame's current address and a random token.
- The QR only appears after a physical button press, and the server isn't
  running at any other time.
- Tokens expire after a short period.
- Text next to the QR tells people which WiFi network to join first.
- Anyone on the network with a valid token can manage photos.

## Refresh modes

| Use | Mode |
| --- | --- |
| Photos | GC16, full 16 greys |
| Photo to loading screen | GC16, with a visible flash |
| Loading to QR to connected | A2 black-and-white, no flash |
| End of session | GC16, to clear ghosting |

GC16 flickers black and white for about half a second, which clears ghosting.
Waveshare advises a full refresh after several partial ones, and redrawing at
least every 24 hours, so timer wakes should change the photo at least daily.

## Risks

- **Mojo without an operating system:** there's no official freestanding mode.
  See [Mojo on RISC-V](MOJO_RISCV.md).
- **Linking with ESP-IDF:** not attempted yet. The exact compiler flags need to
  match Espressif's toolchain.
- **SPI bus sharing:** the panel and microSD share a bus, and both are slow at
  times. Only one transfer happens at a time, by design.
- **IT8951 startup time:** unmeasured, and it sets how quickly the loading
  overlay can appear.
- **WiFi setup:** the hardest part of giving a frame to someone else. Deferred
  to M7.

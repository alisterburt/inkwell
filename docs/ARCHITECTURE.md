# Architecture

## Overview

```text
Phone browser                     Frame (Pi running a Mojo binary)
-------------                     --------------------------------
Pick and crop photo
Convert to 16-grey frame
Upload frame + thumbnail  --->    HTTP server
                                    |
                                    v
                                  Photo store (files + order list)
                                    |
                                    v
                                  Wake and session handling  <---  Button, timer
                                    |
                                    v
                                  IT8951 driver (spidev + GPIO)
                                    |
                                    v
                                  Panel
```

## Power lifecycle

The Pi is fully powered off unless it has work to do. The web server only exists
during a session.

**Timer wake** (scheduled photo change):

1. Power controller turns the Pi on.
2. Program starts, draws the next photo at full quality.
3. Pi powers off. WiFi is never started.

**Button wake** (managing photos):

1. Press. There's no visible response until Linux has booted far enough to start
   the program.
2. Program starts as early as possible and draws a "Waking up" overlay, before
   WiFi.
3. WiFi connects, the server starts, and a token is generated.
4. QR overlay replaces the loading overlay. It has to wait for this point anyway
   because it contains the address the frame was just given.
5. The phone manages photos.
6. Session ends on "Done" or after inactivity. The server stops, the photo is
   restored at full quality, and the Pi powers off.

The overlays cover part of the photo instead of replacing it, so the frame still
looks like a picture while waking.

## Key decision: the phone does the image processing

The browser crops, resizes, converts to grey, dithers, and uploads the finished
frame.

- The frame needs no JPEG or HEIC decoder.
- The preview on the phone is what the panel will show, apart from the panel's
  lower contrast.
- Photo orientation and large camera files are handled before upload.

The cost is that changing the image processing later means re-uploading photos,
unless originals are kept. The first version doesn't keep them.

## Frame format

Raw 4-bit grey, 1872×1404, packed two pixels per byte: 1,314,144 bytes.

## Components

| Component | Language | Depends on |
| --- | --- | --- |
| Phone page | HTML, JS, canvas | None |
| HTTP server | Mojo | Standard library sockets |
| Photo store | Mojo | Filesystem |
| Wake and session handling | Mojo | Power controller |
| Panel driver | Mojo | `spidev`, GPIO through FFI |
| QR generator | Mojo | None, or a C library through FFI |
| Text rendering | Mojo | FreeType through FFI |
| Calendar fetch | Mojo | `libcurl` through FFI, JSON parser |

There's no Python on the device.

## HTTP API sketch

Plain HTTP on the local network. Every `/api` call needs the access token.
Uploads are raw request bodies, which avoids multipart parsing.

| Method | Path | Purpose |
| --- | --- | --- |
| GET | `/` | Phone page |
| GET | `/api/photos` | List photos and their order |
| PUT | `/api/photos/{id}/frame` | Upload the raw panel frame |
| PUT | `/api/photos/{id}/thumb` | Upload the thumbnail |
| GET | `/api/photos/{id}/thumb` | Get the thumbnail |
| DELETE | `/api/photos/{id}` | Delete a photo |
| PUT | `/api/order` | Set the rotation order |
| POST | `/api/show/{id}` | Display a photo now |

## Access

- The QR contains the frame's current address and a random token.
- The QR only appears after a physical button press, and the server isn't
  running at any other time.
- Tokens expire after a short period.
- Anyone on the network with a valid token can manage photos.

## Refresh modes

| Use | Mode |
| --- | --- |
| Photos and calendar | GC16, full 16 greys |
| Loading and QR overlays | Fast black-and-white partial update |
| End of session | GC16, to clear ghosting |

## Calendar source

On battery, wake a few times a day to fetch events, and redraw only when they've
changed or the day has rolled over. Polling every few minutes would drain the
battery within weeks to a few months. Webhooks aren't an option because the frame
is off.

Use `singleEvents=true` and a `timeZone` parameter so Google expands recurring
events and returns local times.

## Risks

- **Mojo ecosystem:** community HTTP and JSON libraries haven't been checked on
  Mojo 1.2. Small hand-written versions are the fallback.
- **Boot time:** the wait before anything appears is set by Linux boot. A heavily
  stripped Zero 2 W has reached userspace in under 3.5 seconds, without WiFi.
  Stock Ubuntu will be much slower.
- **Address space:** a minimal OS needs a kernel with 48-bit virtual addresses, or
  a Mojo runtime rebuilt for fewer. See [Mojo on ARMv8.0](MOJO_ARM.md).
- **WiFi setup:** the hardest part of giving a frame to someone else. Deferred
  to M7.

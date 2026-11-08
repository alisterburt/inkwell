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
                                  Scheduler  <---  Button (GPIO)
                                    |
                                    v
                                  IT8951 driver (spidev + GPIO)
                                    |
                                    v
                                  Panel
```

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
| Scheduler | Mojo | Timers |
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
- The QR only appears after a physical button press.
- Tokens expire after a short period.
- Anyone on the network with a valid token can manage photos.

## Refresh modes

| Use | Mode |
| --- | --- |
| Photos and calendar | GC16, full 16 greys |
| QR overlay | Fast black-and-white partial update |
| After the QR times out | GC16 on that area, to clear ghosting |

## Calendar source

Poll Google Calendar every few minutes with a sync token, and redraw only when
events change, plus once at midnight. Webhooks would need a public HTTPS
endpoint and renewals, for little benefit on a wall display.

Use `singleEvents=true` and a `timeZone` parameter so Google expands recurring
events and returns local times.

## Risks

- **Mojo ecosystem:** community HTTP and JSON libraries haven't been checked on
  Mojo 1.2. Small hand-written versions are the fallback.
- **WiFi setup:** the hardest part of giving a frame to someone else. Deferred
  to M6.

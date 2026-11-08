# Fills a packed 4-bit, 1872x1404 frame with a test gradient.
# Allocates nothing, so it needs no Mojo runtime symbols.

@export("inkwell_fill")
def inkwell_fill(
    buf: Pointer[UInt8, MutUntrackedOrigin], w: Int, h: Int
) abi("C") -> UInt32:
    var total: UInt32 = 0
    var i = 0
    for y in range(h):
        var x = 0
        while x < w:
            var a = UInt8((x + y) & 15)
            var b = UInt8((x + 1 + y) & 15)
            var v = (b << 4) | a
            buf[unsafe_offset=i] = v
            total += UInt32(v)
            i += 1
            x += 2
    return total

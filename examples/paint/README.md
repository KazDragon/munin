# Paint

`paint` is a small terminal paint example for Munin's mouse-event dispatch.

Run it from a build configured with examples enabled:

```sh
cmake --build build/Debug --target paint
build/Debug/external/munin/examples/paint/paint
```

Use the mouse to select a colour from the palette, then drag inside the framed
canvas to paint. Press `q` or `Q` to quit.

# Troubleshooting

[Turkish](TROUBLESHOOTING.tr.md)

**No such file or directory**

The sketch folder is incomplete. Arduino only compiles files in the same folder as the .ino; it neither sees nor compiles anything one level up. The whole software/UYANIK/ folder has to stay together, and the folder name has to match UYANIK.ino.

**Panelde yaptığın değişiklik cihazda görünmüyor**

panel.html was edited but panel_html.h wasn’t regenerated. The device serves the embedded copy, not the source.

```bash
python build_html.py --kontrol
```

If it says the copy is out of date, run “python build_html.py” and build again.

**Works while the computer is on, dies when it’s off**

The USB header isn’t getting standby power. Two BIOS settings are needed together (EuP 2013 = Disabled and Resume By USB Device = Enabled). If it used to work and then stopped, the settings were probably reset. The CMOS battery may have died or the BIOS may have been updated.

Check by measuring. Computer off, plug in, PSU switch on, about 5 V between the VCC and GND pins of the JUSB header.

For a permanent fix, pin 9 of the 24-pin connector (the purple wire) is independent of the BIOS.

**The device restarts at random**

The current spike while Wi-Fi transmits pulls the supply voltage down.

**The first connection attempt fails at boot**

This is normal. You’ll see “Connection failed” on the serial port, and the second attempt connects right after. The router rejects the first request, which is common with ESP32s and home routers.

If every attempt fails, the password may really be wrong, but don’t judge from a single failed attempt.

**uyanik.local doesn’t resolve**

mDNS doesn’t work on every network or every client. Use the local IP shown in the panel. If you set up a DHCP reservation on your router, the address stays fixed.

**The setup portal doesn’t open**

If the page doesn’t open by itself after you join the hotspot, go to 192.168.4.1.

**Permission denied**

The Realtime Database rules don’t allow the account you’re signed in with. Check that the UID in the rules matches the account’s UID.

**Factory reset doesn’t work**

The short between GPIO 4 and GND has to last 5 seconds without interruption. A single break resets the counter. The internal pull-up is used, no external resistor needed.

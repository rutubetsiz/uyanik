# Firmware yapısı

[Türkçe](FIRMWARE.tr.md)

For anyone who wants to contribute to the code or modify it.

## Files

The main flow lives in UYANIK.ino. Pins, the web server, the HTTP endpoints, setup and loop, plus the remote control task.

The rest each take on one job. net.h and net.cpp manage the Wi-Fi state machine and the setup portal. rtdb.h and rtdb.cpp are the REST client for Firebase Realtime Database. settings.h and settings.cpp hold the configuration and validate values coming from the user. identity.h and identity.cpp generate the device-specific passwords and handle factory reset. press.h and press.cpp take care of pressing the power and reset buttons. json.h is for JSON string escaping, and gts_root.h contains Google’s root certificates.

The source of the local panel is panel.html, you can open it in a browser to preview it. The copy embedded in the device is panel_html.h, which is generated from panel.html by build_html.py. It shouldn’t be edited by hand.

The folder name has to match the .ino file.

## RTDB nodes

Everything lives under /<deviceName>/.

cmd is the command field. The panel writes power_short, power_long or reset here, and the device reads it and sets it to none.

The rest are written by the device and read by the panels. state is the computer’s status, on or off. ip is the public IP, localIp the local IP. lastSeen is when the device last checked in, as a server timestamp. pcName is the name the computer shows up as in the remote panel.

A command goes like this: the panel writes to cmd, the device reads it, first sets the field to none, then runs the command, and the panel confirms once it sees the field cleared.

The order here is critical. If it were the other way around, with the device running the command first and clearing it afterwards, a failed clear would leave the command in the database and it would run again on the next poll. The user says “force off” once and the device does it twice. Losing a command is cheaper than that.

## HTTP endpoints (local panel)

These are the endpoints the local panel talks to.

/ serves the panel itself, /api/status returns the status as JSON. Commands go to /api/cmd via POST. /api/settings reads the configuration with GET and saves it with POST.

On the Wi-Fi side, /api/wifi/scan returns the latest scan result with GET and starts a new scan with POST. /api/wifi/connect joins a network, /api/wifi/forget deletes the saved one.

For sessions, /api/login takes the password and returns a token. /api/logout invalidates the token, which drops every open session. /api/panelpw changes the panel password.

Finally, /api/factory does a factory reset and /api/restart restarts the device.

Everything except the panel itself goes through the apiOk() check, details are in the security notes.

## Timings

Button press durations: PRESS_SHORT is 300 ms for normal on/off, PRESS_LONG is 6 seconds for force off, and PRESS_RESET is again 300 ms for reset.

The device polls for commands every 2 seconds with POLL_INTERVAL. It checks the computer’s status every 5 seconds with STATE_INTERVAL. It writes the lastSeen field every 30 seconds with STAMP_INTERVAL. And it asks for the public IP every 5 minutes with IP_INTERVAL.

## Dikkat edilecekler

Pressing happens in an esp_timer callback, not in the loop. A short press is 300 ms, while the ATX force-off threshold is 4 seconds. If the release waits behind a blocking call, an “on” command could hard-shutdown a running machine. Sampling the factory reset pin runs on a timer for the same reason.

Serial.printf shouldn’t be used inside esp_timer callbacks. It blocks when the UART buffer is full and delays every timer callback. The way to do it is to set a flag and do the printing in loop().

The factory reset pin is read every 23 ms and needs 217 uninterrupted LOW readings, which is about 5 seconds. 23 ms was picked because of mains interference: the period of 50 Hz is 20 ms, 60 Hz is 16.7 ms, and 23 ms shares no common divisor with either. That way consecutive readings land on different points of the interference. A single HIGH reading resets the counter.

millis() overflows after 49 days. That’s why time comparisons are done using the sign of the difference: (int32_t)(millis() - t) >= 0.

The token refresh endpoint returns field names in snake_case (id_token), while the sign-in endpoint uses camelCase (idToken). Mix the two up and the token never gets refreshed, so the device loses its Firebase connection at exactly the 60 minute mark.

Three root certificates are embedded. Google uses a different certificate chain for each endpoint: RSA for firebasedatabase.app, ECC for identitytoolkit.googleapis.com. If only one is embedded, authentication fails with a certificate error.

Don’t try to sync panel.html and panel_html.h by hand. It was tried once and they drifted apart: the source was updated but the device kept showing the old version. Run python build_html.py --kontrol before building.
UARTReporter
Launches after mpd, upmpdcli, and raat_app to indicate successful start to the M5

Systemd service file points to /root/UARTReporter/a.out because that will be the default location of the executable

to install the systemd unit file, just copy it to /lib/systemd/system/ with
'cp -avr systemd/uartreporter.service /lib/systemd/system/'

# TemperatureController
Windows application which connects to the EVEREST (PC diagnostics application) through shared memory, takes values of the PC sensors' temperatures, displays them on the screen and alerts with repeated beep sound when they reach given treshold.
It tries to open Everest application at start if you provide it with the path.

Tested only with Everest Ultimate Edition version: 5.02.1765 Beta and Windows 8, 8.1, 10

To make it work you have to enable option "Enable shared memory" in Everest's Settings (Hardware Monitoring -> External Applications)

Right-click anywhere inside window client's area to show options.

# Building

Tested with the following software components:

* CCStudio 12.1.0.00007
* SimpleLink CC13xx CC26xx SDK 5.30.01.01
* SysConfig 1.10.0

# Flashing

* In `targetConfigs/CC1352R1F3.ccxml`, replace the serial number in `Debug Probe Selection`. The serial number of attached XDS110 debug probes can be checked with `ccs/ccs_base/common/uscif/xds110/xdsdfu -e`.

# Wire connections

* DIO16 (boosterpack.32): for counting inferences
* DIO11 (boosterpack.18): for measuring recharging time. Connect to `VBAT_OK` as 3V3 may not be low enough to be considered as GPIO 0 in a short recharging period.
* DIO19 (boosterpack.37): general indicator 0
* DIO18 (boosterpack.36): general indicator 1
* DIO21 (boosterpack.8 ): general indicator 2
* DIO3  (boosterpack.19): general indicator 3
* GND <-> GND

See pinout on [CC1352R1 LaunchPad Quick Start Guide](https://www.ti.com/lit/ml/swru525e/swru525e.pdf).

Note that the jumper on TDI between CC1352R1 and XDS110 (debugger) should be removed, or power will flow into MSP430/MSP432 via DIO17.

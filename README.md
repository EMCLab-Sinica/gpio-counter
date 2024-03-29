# Building

Tested with the following software components:

* CCStudio 12.1.0.00007
* SimpleLink CC13xx CC26xx SDK 5.30.01.01
* SysConfig 1.10.0

# Flashing

* In `targetConfigs/CC1352R1F3.ccxml`, replace the serial number in `Debug Probe Selection`. The serial number of attached XDS110 debug probes can be checked with `ccs/ccs_base/common/uscif/xds110/xdsdfu -e`.

# Wire connections

* DIO17 (boosterpack.31) <-> P8.0 on MSP430 or P5.5 on MSP432 (`GPIO_COUNTER_PORT`/`GPIO_COUNTER_PIN`): for counting inferences
* DIO19 (boosterpack.37) <-> P4.7 on MSP430 or P5.4 on MSP432 (`GPIO_LAYER_COUNTER_PORT`/`GPIO_LAYER_COUNTER_PIN`): for counting layers
* DIO11 (boosterpack.18) <-> P1.0 on MSP430 or MSP432: for measuring recharging time
* GND <-> GND on MSP430 or MSP432

See pinout on [CC1352R1 LaunchPad Quick Start Guide](https://www.ti.com/lit/ml/swru525e/swru525e.pdf).

Note that the jumper on TDI between CC1352R1 and XDS110 (debugger) should be removed, or power will flow into MSP430/MSP432 via DIO17.

![](./resources/official_armmbed_example_badge.png)
# Getting Started With The sd-driver

This is the mbed-os example for the SDBlockDevice (SDCard) block device driver. 
See the [SDBlockDevice documentation](https://os.mbed.com/docs/mbed-os/latest/apis/sdblockdevice.html) for more information.

This guide reviews the steps to get the SDCard with FAT filesystem working on an mbed OS platform.
start/online-with-the-online-compiler.html#importing-the-code).)

## Mbed OS build tools

### Mbed CLI 2

Starting with version 6.5, Mbed OS uses Mbed CLI 2. It uses Ninja as a build system, and CMake to generate the build environment and manage the build process in a compiler-independent manner. If you are working with Mbed OS version prior to 6.5 then check the section [Mbed CLI 1](#mbed-cli-1).
1. [Install Mbed CLI 2](https://os.mbed.com/docs/mbed-os/latest/build-tools/install-or-upgrade.html).
1. From the command-line, import the example: `mbed-tools import mbed-os-example-sd-driver`
1. Change the current directory to where the project was imported.

### Mbed CLI 1
1. [Install Mbed CLI 1](https://os.mbed.com/docs/mbed-os/latest/quick-start/offline-with-mbed-cli.html).
1. From the command-line, import the example: `mbed import mbed-os-example-sd-driver`
1. Change the current directory to where the project was imported.

## Hardware Requirements

This example can be used on an mbedos platform that: 

- Has an on-board SDCard slot or,
- Is fitted with a [CI Test Shield](https://github.com/ARMmbed/ci-test-shield). 

This document uses the K64F as an example. Simply change the relevant options (e.g. -m K64F) to be appropriate for your target.

## <a name="insert-sdcard-into-k64f"></a> Insert SDCard into K64F

This example has been run on a K64F with the following pre-formatted microSDHC cards:

- Kingston 2GB mircoSDHC card.  
- Kingston 8GB mircoSDHC card.  
- SanDisk 16GB mircoSDHC ultra card.  

If the card requires formatting then the following procedure is known to work:

- Insert microSD card into SD adapter in USB stick (or similar) so the microSD card can be insert into windows PC.
- Within file explorer, right click/Format on the USB drive.
- Select FAT32, 4096 cluster size, Quick Format.
- Format the drive.

The microSD card should then be ready for use in the K64F. Insert the formatted card
into the SDCard slot on the K64F PCB. 

## Build the Example

1. Connect a USB cable between the USB port on the board and the host computer.
1. Run the following command to build the example project, program the microcontroller flash memory, and open a serial terminal:

    * Mbed CLI 2

     ```sh
     $ mbed-tools compile -m <TARGET> -t <TOOLCHAIN> --flash --sterm
     ```

    * Mbed CLI 1

     ```sh
     $ mbed compile -m <TARGET> -t <TOOLCHAIN> --flash --sterm
     ```

## <a name="run-the-example-binary-on-the-k64f"></a> Run the Example Binary on the K64F 

You should see the following output in the serial terminal once the program has been flashed onto the microcontroller (reset your target if you don't):

```
Welcome to the filesystem example.
Opening a new file, numbers.txt. done.
Writing decimal numbers to a file (20/20) done.
Closing file. done.
Re-opening file read-only. done.
Dumping file to screen.
0
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
EOF.
Closing file. done.
Opening root directory. done.
Printing all filenames:
     numbers.txt
Closeing root directory. done.
Filesystem Demo complete.
```

## <a name="testing-with-an-sdcard-on-target-xyx"></a> Testing with an SDCard on Target XYZ

The standard way to test is with the mbed CI Test Shield plugged into the 
target board. This pin mapping for this configuration is parameterised in 
the [`mbed_app.json`](./mbed_app.json) file.

Note the following things about the `mbed_app.json` file:

- The `mbed_app.json` file is used to define target specific symbols for the SPI pins connecting the SDCard slot to the target MCU:
    - "SPI\_CS". This is the Chip Select line.
    - "SPI\_MOSI". This is the Master Out Slave In data line.
    - "SPI\_MISO". This is the Master In Slave Out data line.
    - "SPI\_CLK".  This is the serial Clock line.
- The default configuration defined in the "config" section is for the standard Arduino header pin mappings for the SPI bus. 
  The "config" section defines a dictionary mapping functional names to target board Arduino header pins: 
    - "SPI\_CS": "ARDUINO_UNO_D10". This causes the MBED\_CONF\_APP\_SPI\_CS symbol to be defined in mbed\_config.h as ARDUINO_UNO_D10, which is used in the filesystem test implementation.
      D10 is defined in the target specific PinNames.h file.
    - "SPI\_MOSI": "ARDUINO_UNO_D11". This causes the MBED\_CONF\_APP\_SPI\_MOSI symbol to be defined in mbed\_config.h.
    - "SPI\_MISO": "ARDUINO_UNO_D12". This causes the MBED\_CONF\_APP\_SPI\_MISO symbol to be defined in mbed\_config.h.
    - "SPI\_CLK": "ARDUINO_UNO_D13". This causes the MBED\_CONF\_APP\_SPI\_CLK symbol to be defined in mbed\_config.h.
- The `"target_overrides"` section is used to override the "SPI\_xxx" symbols for specific target boards, which may have an SDCard slot, for example.
  This is the case for the K64F, where the "SPI\_xxx" are mapped to the pin names for the on-board SDCard.

  ```json
    "K64F": {
         "SPI_MOSI": "PTE3",
         "SPI_MISO": "PTE1",
         "SPI_CLK":  "PTE2",
         "SPI_CS":   "PTE4"
    }
    ```  
- Thus, in the absence of any target specific definitions in the `"target_overrides"` section, all boards will default to 
  using the Arduino header configuration. For those platforms with a `"target_overrides"` section then this configuration
  will be used in preference. 
- Hence in the case that you want to test a platform with an SDCard inserted into a 
  fitted CI test shield (rather than the on-board SDCard slot)
  and there is a `"target_overrides"` section present in the `mbed_app.json` file, you must then delete the `"target_overrides"`
  section before building. This will result in the default configuration being used (suitable for the CI
  Test Shield).
- Note when inserting the v1.0.0 CI Test Shield into the Arduino header of the target platform, the shield pins D0 and
  D1 should be bent to be parallel to the shield PCB so they are not inserted into the Arduino header. This is because
  some boards use the same UART on DAPLINK and D0/D1, which means the serial debug channel breaks and hence the mbed greentea
  test suite will not work correctly. This is mainly on older ST boards and should not be a problem on 
  `K64F`, `NUCLEO_F429ZI` and `UBLOX_EVK_ODIN_W2`. Note also that the v2.0.0 CI Test Shield doesn't suffer from this
  problem and the pins don't need to be bent.  
- When inserting the SDCard into the card slot on the CI test shield, make sure the card is fully inserted. 
  On insertion, there should be a small clicking sound when the card registers, and the back edge of the card
  should protrude no more than ~1mm over the edge of the CI test shield PCB. If the SDCard fails to register, 
  try gently pushing the metal flexible strip in the shape of a spade at the top edge of the SDCard metal slot
  casing with a pair of tweezers, bending it a little to lower it into the slot casing. This helps with the
  insertion mechanism.


## Target K64F with CI Test Shield fitted

![alt text](pics/sd_driver_k64_with_ci_test_shield.jpg "unseen title text")

**Figure 1. The figure shows the K64F platform with the CI shield fitted.**

The above figure shows the K64F with the v1.0.0 CI test shield fitted. Note:

- The pins D0/D1 (top right of CI test shield) are bent sideways so as not to insert into the header.
- The SDCard is fully inserted into the slot and overhangs the PCB by ~1mm.  
  

# Troubleshooting

If you have problems, you can review the [documentation](https://os.mbed.com/docs/latest/tutorials/debugging.html) for suggestions on what could be wrong and how to fix it.

## License and contributions

The software is provided under Apache-2.0 license. Contributions to this project are accepted under the same license. Please see [contributing.md](CONTRIBUTING.md) for more info.

This project contains code from other projects. The original license text is included in those source files. They must comply with our license guide.

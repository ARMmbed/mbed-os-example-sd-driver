# Getting Started With The sd-driver

This is the mbed-os example for the SDBlockDevice (SDCard) block device driver. 
See the [sd-driver](https://github.com/armmbed/sd-driver) repository for more information.

This guide reviews the steps to get the SDCard with FAT filesystem working on an mbed OS platform.

Please install [mbed CLI](https://github.com/ARMmbed/mbed-cli#installing-mbed-cli).

## Hardware requirements

This example can be used on a mbedos platform with SDCard slot or fitted with a 
[CI Test Shield]((https://github.com/ARMmbed/ci-test-shield). 

## Import the example application

From the command-line, import the example:

```
mbed import mbed-os-example-sd-driver
```

You should see: 

	[mbed] Importing program "mbed-os-example-sd-driver" from "https://github.com/ARMmbed/mbed-os-example-sd-driver" at latest revision in the current branch
	[mbed] Adding library "mbed-os" from "https://github.com/ARMmbed/mbed-os" at rev #f4864dc6429e

Move into the newly created direcrtory:

```
cd mbed-os-example-sd-driver
```
	
If the mbed-os libarry was not automatically added (see trace above), do the following to import mbed-os:

```
mbed new .
```

Add the sd-driver repository: 

```
mbed add sd-driver
```


### Now compile

Invoke `mbed compile`, and specify the name of your platform and your favorite toolchain (`GCC_ARM`, `ARM`, `IAR`). For example, for the ARM Compiler 5:

```
mbed compile -m K64F -t GCC_ARM
```

Your PC may take a few minutes to compile your code. At the end, you see the following result:

```
[snip]
Compile [ 99.7%]: SDBlockDevice.cpp
Compile [100.0%]: SPIFBlockDevice.cpp
Link: mbed-os-example-sd-driver
Elf2Bin: mbed-os-example-sd-driver
+--------------------------+-------+-------+-------+
| Module                   | .text | .data |  .bss |
+--------------------------+-------+-------+-------+
| Fill                     |   162 |     0 |  2514 |
| Misc                     | 53840 |  2284 |  1112 |
| drivers                  |  1130 |     0 |    64 |
| features/filesystem      | 13379 |     0 |   550 |
| features/storage         |    42 |     0 |   184 |
| hal                      |   450 |     0 |     8 |
| platform                 |  2497 |    20 |   582 |
| rtos                     |   149 |     4 |     4 |
| rtos/rtx                 |  6143 |    20 |  6870 |
| targets/TARGET_Freescale | 12888 |    12 |   384 |
| Subtotals                | 90680 |  2340 | 12272 |
+--------------------------+-------+-------+-------+
Allocated Heap: 24576 bytes
Allocated Stack: unknown
Total Static RAM memory (data + bss): 14612 bytes
Total RAM memory (data + bss + heap + stack): 39188 bytes
Total Flash memory (text + data + misc): 94060 bytes

Image: .\BUILD\K64F\GCC_ARM\mbed-os-example-sd-driver.bin
```

### <a name="insert-sdcard-into-k64f"></a> Insert SDCard into K64F

The examples and test cases have been run on a K64F with the following pre-formatted microSDHC cards:

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

### <a name="run-the-example-binary-on-the-k64f"></a> Run the Example Binary on the K64F 

Once the binary is built, copy the binary from `<root_dir>/mbed-os-example-sd-driver/BUILD/K64F/GCC_ARM/main.bin` to the K64F:

1. Connect your mbed device to the computer over USB.
1. Copy the binary file to the mbed device.
1. Press the reset button to start the program.
1. Open the UART of the board in your favorite UART viewing program. For example, `screen /dev/ttyACM0`.

You see the following output:

After connecting a serial console and resetting the target, the following trace should be seen:

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


## Troubleshooting

1. Make sure `mbed-cli` is working correctly and its version is newer than `1.0.0`.

 ```
 mbed --version
 ```

 If not, update it:

 ```
 pip install mbed-cli --upgrade
 ```
 

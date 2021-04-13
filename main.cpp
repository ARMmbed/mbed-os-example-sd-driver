/* mbed Microcontroller Library
 * Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "LittleFileSystem.h"
#include "SDBlockDevice.h"
#include <stdio.h>
#include <errno.h>
#include "platform/mbed_retarget.h"

SDBlockDevice sd(MBED_CONF_SD_SPI_MOSI,
                 MBED_CONF_SD_SPI_MISO,
                 MBED_CONF_SD_SPI_CLK,
                 MBED_CONF_SD_SPI_CS);
LittleFileSystem fs("sd", &sd);

void return_error(int ret_val)
{
    if (ret_val) {
        printf("Failure. %d\n", ret_val);
        while (true) {
            __WFI();
        }
    } else {
        printf("done.\n");
    }
}

void errno_error(void *ret_val)
{
    if (ret_val == NULL) {
        printf(" Failure. %d \n", errno);
        while (true) {
            __WFI();
        }
    } else {
        printf(" done.\n");
    }
}

int main()
{
    int error = 0;
    printf("Welcome to the filesystem example.\n");

    // Try to mount the filesystem
    printf("Mounting the filesystem... ");

    error = fs.mount(&sd);
    if (error) {
        // Reformat if we can't mount the filesystem
        // this should only happen on the first boot
        printf("No filesystem found, formatting... ");
        error = fs.reformat(&sd);
        return_error(error);
    }
    printf("Opening a new file, numbers.txt.");
    FILE *fd = fopen("/sd/numbers.txt", "w+");
    errno_error(fd);

    for (int i = 0; i < 20; i++) {
        printf("Writing decimal numbers to a file (%d/20)\r", i);
        fprintf(fd, "%d\n", i);
    }
    printf("Writing decimal numbers to a file (20/20) done.\n");

    printf("Closing file.");
    fclose(fd);
    printf(" done.\n");

    printf("Re-opening file read-only.");
    fd = fopen("/sd/numbers.txt", "r");
    errno_error(fd);

    printf("Dumping file to screen.\n");
    char buff[16] = {0};
    while (!feof(fd)) {
        int size = fread(&buff[0], 1, 15, fd);
        fwrite(&buff[0], 1, size, stdout);
    }
    printf("EOF.\n");

    printf("Closing file.");
    fclose(fd);
    printf(" done.\n");

    printf("Opening root directory.");
    DIR *dir = opendir("/sd/");
    errno_error(fd);

    struct dirent *de;
    printf("Printing all filenames:\n");
    while ((de = readdir(dir)) != NULL) {
        printf("  %s\n", &(de->d_name)[0]);
    }

    printf("Closing root directory. ");
    error = closedir(dir);
    return_error(error);
    printf("Filesystem Demo complete.\n");

    while (true) {}
}


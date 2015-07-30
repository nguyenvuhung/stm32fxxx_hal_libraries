/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Fatfs implementation for STM32Fxxx devices
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_FATFS_H
#define TM_FATFS_H 100

/**
 * @addtogroup TM_STM32Fxxx_HAL_Libraries
 * @{
 */

/**
 * @defgroup TM_FATFS
 * @brief    FATFS implementation for STM32Fxxx devices
 * @{
 *
 * FatFs implementation for STM32F4xx devices
 *
 * This library uses Chan's Fatfs implementation.
 * 
 * This library is only for communication. To work with files, you have to look at Chan's FatFs manual, link below:
 * http://elm-chan.org/fsw/ff/00index_e.html
 *
 * You can work with SPI or SDIO protocol to interface SDCARD.
 *
 * \par SDCARD pinouts
 *
 * Library works with SPI or SDIO mode. Also, when in SDIO mode, you can set to 1- or 4-bit mode.
 * By default, SDIO with 4-bit mode is used, so you will look into right column on table below.
 * 
 * SD CARD PINS
 * 	   
@verbatim
   _________________
  / 1 2 3 4 5 6 7 8 |  NR   |SDIO INTERFACE                               |SPI INTERFACE
 /                  |       |NAME     STM32Fxxx     DESCRIPTION           |NAME   STM32Fxxx   DESCRIPTION 
/ 9                 |       |         4-BIT  1-BIT                        | 
|                   |       |                                             | 
|                   |   1   |CD/DAT3  PC11   -      Connector data line 3 |CS     PB5         Chip select for SPI 
|                   |   2   |CMD      PD2    PD2    Command/Response line |MOSI   PA7         Data input for SPI 
|                   |   3   |VSS1     GND    GND    GND                   |VSS1   GND         GND 
|   SD CARD Pinout  |   4	  |VDD      3.3V   3.3V   3.3V Power supply     |VDD    3.3V        3.3V Power supply 
|                   |   5   |CLK      PC12   PC12   Clock                 |SCK    PA5         Clock for SPI 
|                   |   6   |VSS2     GND    GND    GND                   |VSS2   GND         GND 
|                   |   7   |DAT0     PC8    PC8    Connector data line 0 |MISO   PA6         Data output for SPI 
|                   |   8   |DAT1     PC9    -      Connector data line 1 |-      -           - 
|___________________|   9   |DAT2     PC10   -      Connector data line 2 |-      -           - 
@endverbatim
 *	
 * \par SDIO Communication
 * 
 * By default, SDIO with 4-bit communication is used.
 * If you want to use SDIO 1-bit communication, set defines below in your defines.h file:
 * 
@verbatim
//Set SDIO with 1-bit communication
#define FATFS_SDIO_4BIT   0
@endverbatim
 * 
 * For SDIO communication, you will need at least these files:
 *
@verbatim
- tm_stm32_fatfs.h
- tm_stm32_fatfs.c
- fatfs/diskio.h
- fatfs/diskio.c
- fatfs/ff.h
- fatfs/ff.c
- fatfs/ffconf.h
- fatfs/integer.h
- fatfs/option/syscall.c
- fatfs/option/unicode.c
- fatfs/drivers/fatfs_sd_sdio.h
- fatfs/drivers/fatfs_sd_sdio.c
@endverbatim 
 *
 * \par SPI Communication
 * 
 * Or, if you want to use SPI communication, you have to add lines below in your defines.h file
 *	
@verbatim
//Enable SPI communication, disable SDIO
#define FATFS_USE_SDIO   0
@endverbatim
 *	
 * Files, needed for SPI:
 *
@verbatim
- tm_stm32_fatfs.h
- tm_stm32_fatfs.c
- fatfs/diskio.h
- fatfs/diskio.c
- fatfs/ff.h
- fatfs/ff.c
- fatfs/ffconf.h
- fatfs/integer.h
- fatfs/option/syscall.c
- fatfs/option/unicode.c
- fatfs/drivers/fatfs_sd.h
- fatfs/drivers/fatfs_sd.c
@endverbatim 
 * 
 * \par Overwriting default pinout
 * 
 * SDIO interface pins are fixed, and can not be changed.
 * If you want to change SPI pins, you have to set these defines in your defines.h file:
 * 
@verbatim
//Set your SPI, for corresponding pins look at TM SPI library
#define FATFS_SPI               SPI1
#define FATFS_SPI_PINSPACK      TM_SPI_PinsPack_1
	
//Set your CS pin for SPI			
#define FATFS_CS_PORT           GPIOB
#define FATFS_CS_PIN            GPIO_PIN_5
@endverbatim
 *
 * \par Write protect and Card detect pins
 *
 * Library has support for Write protect and Card detect pins. This two pins are by default on pins below.
 *
 * They are the same for any communication used, and are disabled by default.
 * 
@verbatim
NAME	STM32F4XX	DESCRIPTION
	
WP		PB7			Write protect pin. Pin low when write is enabled
CD		PB6			Card detect pin. Pin low when card detected
@endverbatim
 *
 * Like I said before, these 2 pins are disabled by default. If you want to use it, you have to add 2 lines in your defines.h file:
 *
@verbatim
//Enable Card detect pin
#define FATFS_USE_DETECT_PIN          1

//Enable Write protect pin
#define FATFS_USE_WRITEPROTECT_PIN    1
@endverbatim
 * 
 * WP and CD pins are now enabled with default configuration.
 *
 * Add lines below to your defines.h file only if you want to overwrite default pin settings:
 *
@verbatim
//Default CD pin			
#define FATFS_DETECT_PORT          GPIOB
#define FATFS_DETECT_PIN           GPIO_PIN_6

//Default WP pin			
#define FATFS_WRITEPROTECT_PORT    GPIOB
#define FATFS_WRITEPROTECT_PIN     GPIO_PIN_7
@endverbatim
 *
 * \par Timing function for files
 * 
 * FatFs uses function get_fattime() for time, to set timestamp for files, when they were edited or created.
 * 
 * By default, function returns 0, but if you want to create your own function, you have to set defines in defines.h file:
 * 
@verbatim
//Use custom get_fattime() function
#define FATFS_CUSTOM_FATTIME	1
@endverbatim
 *
 * And then somewhere in your project, add function like below:
 * 
@verbatim
//Use custom get_fattime function
//Implement RTC get time here if you need it
DWORD get_fattime (void) {
	//Get your time from RTC or something like that
	
	return	  ((DWORD)(2014 - 1980) << 25)	// Year 2014
			| ((DWORD)7 << 21)				// Month 7
			| ((DWORD)10 << 16)				// Mday 10
			| ((DWORD)16 << 11)				// Hour 16
			| ((DWORD)0 << 5)				// Min 0
			| ((DWORD)0 >> 1);				// Sec 0
}
@endverbatim
 * 
 * Added support for USB MSC HOST interface with FatFS library.
 * More about, how to configure USB MSC HOST to work properly, you should take a look at 
 * my library tm_stm32f4_usb_msc_host.h. There is also default pinout for USB and other stuff.
 * 
 * I will explain here, how to properly set FatFS library to work with USB.
 * 
 * Files, needed for USB communication and FatFS:
 *
@verbatim
- tm_stm32_fatfs.h
- tm_stm32_fatfs.c
- fatfs/diskio.h
- fatfs/diskio.c
- fatfs/ff.h
- fatfs/ff.c
- fatfs/ffconf.h
- fatfs/integer.h
- fatfs/option/syscall.c
- fatfs/option/unicode.c
- fatfs/drivers/fatfs_usb.h
- fatfs/drivers/fatfs_usb.c
@endverbatim 
 * 
 * After you have your files included in project, open project's defines.h file and add line below:
 * 
@verbatim
//Enable USB in FatFS library
#define FATFS_USE_USB      1
@endverbatim
 * 
 * This will enable USB functions to work with FatFS module. Also, when you make define 2 lines above, SD card settings become inactive.
 * 
 * If you want to use SD card and USB storage at the same time for some reason, you have to enable SD card functionality back. Add lines below in your defines.h file:
 * 
@verbatim
//Enable SDIO communication for SD card
#define FATFS_USE_SDIO		1

//Enable SPI communication for SD card
#define FATFS_USE_SDIO		0
@endverbatim
 *	
 * And also, you should know, that USB has 1: partition and SD card has 0: partition. So when you mount SD card, you should use:
 * 
@verbatim
//Mount SD card
f_mount(&sd_fs, "0:", 1);

//Mount USB storage
f_mount(&usb_fs, "1:", 1);
@endverbatim

 * This allows you to copy data from one SD card to USB and back too and use of 2 different physical drives at the same time.
 *
 * \par FatFS with SDRAM
 *
 * As of version 1.7, FATFS can now be used with SDRAM memory on STM32F429-Discovery or STM324x9-EVAL board.
 *
 * To enable this feature, you will have to open defines.h file and add define:
 *
@verbatim
//Enable SDRAM
#define FATFS_USE_SDRAM   1
@endverbatim
 *
 * @note  After that, again, SDCARD will be disabled by default, if you want to use it too (with SDCARD together) you have to do 
 * the same as you have to do when you use USB with FATFS, otherwise, if you will try to read/write from SDCARD, you will get errors.
 *
 * Files, you will need for running SDRAM driver are:
 *
@verbatim
- tm_stm32_fatfs.h
- tm_stm32_fatfs.c
- fatfs/diskio.h
- fatfs/diskio.c
- fatfs/ff.h
- fatfs/ff.c
- fatfs/ffconf.h
- fatfs/integer.h
- fatfs/option/syscall.c
- fatfs/option/unicode.c
- fatfs/drivers/fatfs_sdram.h
- fatfs/drivers/fatfs_sdram.c
- tm_stm32f4_sdram.h
- tm_stm32f4_sdram.c
@endverbatim 
 *
 * @note SDRAM is not a memory which holds data when power is off. For that reason, you will have to use @ref f_mkfs() function to 
 *       create file system object on your SDRAM drive. This will have to be done when you initialize your SDRAM or when you first time
 *       try to mount from SDRAM. Otherwise, you will not be able to interface with FATFS based functionality like SDCARD can do.
 *
 * \par FatFS with SPI FLASH
 *
 * This option is added to implementation, but low level drivers are not implemented yet.
 * 
 * @note If you will try to read from SPIFLASH using @ref f_mount() function, you will get errors.
 *
 * \par New names for drivers
 *
 * Before version 1.7, SDCARD and USB drivers were supported only with this library.
 *
 * If you want to use them both at a time, you always have to keep in mind to 
 * use different numbers for drivers, for example (notice number in the second parameter):
 *
@verbatim
//Mount SDCARD
f_mount(&fsSD, "0:", 1);
//Mount USB 
f_mount(&fsUSB, "1:", 1);
@endverbatim
 *
 * As of version 1.7, you can now use the same as before, or like that:
 *
@verbatim
//Mount SDCARD
f_mount(&fsSD, "SD:", 1);
//Mount USB 
f_mount(&fsUSB, "USB:", 1);
@endverbatim
 *
 * If you want to open file, fox example on USB, you can do like this:
 *
@verbatim
//Open file on USB key
//Mount first
f_open(&fil, "USB:my_file.txt", FA...);
@endverbatim
 *
 * \par All supported names
 *
 * Below is a list of all supported strings for volume numbers:
 *
 * - SDCARD: <b>SD</b>; Drive number 0
 * - USB DRIVE: <b>USB</b>; Drive number 1
 * - SDRAM: <b>SDRAM</b>; Drive number 2
 * - SPI FLASH: <b>SPIFLASH</b>; Drive number 3; Low level driver is not implemented yet
 *
 * Example for opening files:
 *
@verbatim
//Open file on SDRAM
f_open(&fil, "SDRAM:my_file_sdram.txt", FA...);
//Open file on SDCARD
f_open(&fil, "SD:my_file_sd.txt", FA...);
//and more files
@endverbatim
 *
 * \par Example of using all 4 possible drivers in your project
 *
 * To use all available data sources (physical drivers) for FATFS, you will have to open defines.h file, and add these defines:
 *
@verbatim
//Enable all possible FATFS sources implemented in library

//Enable USB
#define FATFS_USE_USB       1
//Enable SDRAM
#define FATFS_USE_SDRAM     1
//Enable SPI flash
#define FATFS_USE_SPIFLASH  1 //Not implemented yet

//SDCARD is enabled by default, but when you enable anything else too, SDCARD is disabled
//If you want to enable it too, you have to tell that!
#define FATFS_USE_SDIO      1 //Set to 0 if you want to use SDCARD with SPI
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32Fxxx HAL
 - defines.h
 - TM SPI           (only when SPI)
 - TM DELAY         (only when SPI)
 - TM GPIO
 - TM SDRAM         (only when SDRAM)
 - FatFS by Chan
@endverbatim
 */

#include "stm32fxxx_hal.h"
#include "defines.h"
#include "tm_stm32_gpio.h"
#include "ff.h"
#include "diskio.h"

/**
 * @defgroup TM_FATFS_Macros
 * @brief    Library defines
 * @{
 */

/** 
 * @brief  Default truncate buffer size in bytes
 * @note   If you have in plan to truncate from beginning large files a lot of times,
 *         then you should think about increasing this value as far as possible.
 *         With larger buffer size you will get faster response with truncating.
 */
#ifndef FATFS_TRUNCATE_BUFFER_SIZE
#define FATFS_TRUNCATE_BUFFER_SIZE	256
#endif

/**
 * @}
 */
 
/**
 * @defgroup TM_FATFS_Typedefs
 * @brief    Library typedefs
 * @{
 */

/**
 * @brief  FATFS size structure
 */
typedef struct {
	uint32_t Total; /*!< Total size of memory */
	uint32_t Free;  /*!< Free size of memory */
} TM_FATFS_Size_t;

/**
 * @}
 */

/**
 * @defgroup TM_FATFS_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief   Gets total and free memory sizes of any drive
 * @param   *str: Pointer to string for drive to be checked
 * @param   *SizeStruct: Pointer to empty @ref TM_FATFS_Size_t structure to store data about memory
 * @retval  FRESULT structure members. If data are valid, FR_OK is returned
 * @example Get memory sizes of USB device:
 *             TM_FATFS_GetDriveSize("USB:", &SizeStruct);
 */
FRESULT TM_FATFS_GetDriveSize(char* str, TM_FATFS_Size_t* SizeStruct);

/**
 * @brief  Truncates beginning of file
 *
 * Example:
 *	- You have a file, its content is: "abcdefghijklmnoprstuvwxyz",
 *	- You want to truncate first 5 bytes,
 *	- Call @ref TM_FATFS_TruncateBeginning(&opened_file, 5);
 *	- You will get new file data: "fghijklmnoprstuvwxyz"
 
 * @param  *fil: Pointer to already opened file
 * @param  index: Number of characters that will be truncated from beginning
 * @note   If index is more than file size, everything will be truncated, but file will not be deleted
 * @retval FRESULT struct members. If everything ok, FR_OK is returned
 */
FRESULT TM_FATFS_TruncateBeginning(FIL* fil, uint32_t index);

/**
 * @brief  Checks card detect pin (if activated) if card is inserted
 * @note   Pin must be set low in order to get card inserted, otherwise card is not inserted
 * @note   Card detect pin must be activated in order to get this functionality to work
 * @param  None
 * @retval Card detected status:
 *            - 0: Card is not inserted
 *            - > 0: Card is inserted
 */
uint8_t TM_FATFS_CheckCardDetectPin(void);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

#endif


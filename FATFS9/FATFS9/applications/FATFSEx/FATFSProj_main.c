#include "FATFSProj_main.h"

#define BUFFER_SIZE_BYTES 512

FATFS   fs;
FIL     file_object;
UINT    bytes_read;
FRESULT res;

uint8_t soundBuffers[2][BUFFER_SIZE_BYTES];

COMPILER_ALIGNED(16) DmacDescriptor my_descriptors[2];
COMPILER_ALIGNED(16) DmacDescriptor my_writebacks[2];

volatile int myFlag;

int main (void)
{
	int i;
	int endOfFile;
	int lastBlockStarted;
	
	atmel_start_init();
	
	printf ("\n\rHello, world!\n\r\n\r");

#define PC01 GPIO(GPIO_PORTC,  1)
#define PC10 GPIO(GPIO_PORTC, 10)
	
	gpio_set_pin_direction (PC01, GPIO_DIRECTION_OUT);
	gpio_set_pin_pull_mode (PC01, GPIO_PULL_OFF);
	gpio_set_pin_function (PC01, GPIO_PIN_FUNCTION_OFF);
	gpio_set_pin_level (PC01, false);

	gpio_set_pin_direction (PC10, GPIO_DIRECTION_OUT);
	gpio_set_pin_pull_mode (PC10, GPIO_PULL_OFF);
	gpio_set_pin_function (PC10, GPIO_PIN_FUNCTION_OFF);
	gpio_set_pin_level (PC10, false);

	//                  RRRRLLLL
	I2S->TXDATA.reg = 0x00018000;
	
	do {
		
		printf ("Enabling DMA\n\r");
		
		// ENABLE DMA
		MCLK->AHBMASK.bit.DMAC_ = 1;
		DMAC->CTRL.bit.SWRST = 1;
		DMAC->CTRL.bit.DMAENABLE = 0;
		DMAC->BASEADDR.bit.BASEADDR = (uint32_t)my_descriptors;
		DMAC->WRBADDR.bit.WRBADDR = (uint32_t)my_writebacks;
		DMAC->CTRL.bit.LVLEN0 = 1;
		DMAC->CTRL.bit.LVLEN1 = 1;
		DMAC->CTRL.bit.LVLEN2 = 1;
		DMAC->CTRL.bit.LVLEN3 = 1;
		DMAC->CTRL.bit.DMAENABLE = 1;
	
		NVIC_DisableIRQ(DMAC_0_IRQn);
		NVIC_ClearPendingIRQ(DMAC_0_IRQn);
		NVIC_EnableIRQ(DMAC_0_IRQn);
		NVIC_SetPriority(DMAC_0_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

		NVIC_DisableIRQ(DMAC_1_IRQn);
		NVIC_ClearPendingIRQ(DMAC_1_IRQn);
		NVIC_EnableIRQ(DMAC_1_IRQn);
		NVIC_SetPriority(DMAC_1_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

		NVIC_DisableIRQ(DMAC_2_IRQn);
		NVIC_ClearPendingIRQ(DMAC_2_IRQn);
		NVIC_EnableIRQ(DMAC_2_IRQn);
		NVIC_SetPriority(DMAC_2_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

		NVIC_DisableIRQ(DMAC_3_IRQn);
		NVIC_ClearPendingIRQ(DMAC_3_IRQn);
		NVIC_EnableIRQ(DMAC_3_IRQn);
		NVIC_SetPriority(DMAC_3_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

		NVIC_DisableIRQ(DMAC_4_IRQn);
		NVIC_ClearPendingIRQ(DMAC_4_IRQn);
		NVIC_EnableIRQ(DMAC_4_IRQn);
		NVIC_SetPriority(DMAC_4_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

		// clear the channel 0 descriptor
		memset((void*)&my_descriptors[0], 0x00, sizeof(my_descriptors[0]));
		memset((void*)&my_writebacks[0], 0x00, sizeof(my_writebacks[0]));
	
		// disable then reset DMA channel 0
		DMAC->Channel[0].CHCTRLA.bit.ENABLE = 0;
		DMAC->Channel[0].CHCTRLA.bit.SWRST = 1;	
	
		// set priority level 0
		DMAC->Channel[0].CHPRILVL.bit.PRILVL = 0;

		// set trigger source
		DMAC->Channel[0].CHCTRLA.bit.TRIGSRC = I2S_DMAC_ID_TX_0;		// I2S Tx 0 Trigger
		DMAC->Channel[0].CHCTRLA.bit.TRIGACT = 2; //DMAC_CHCTRLA_TRIGACT_BLOCK_Val;  // one trigger per block
  	
		printf ("Mounting filesystem.\n\r");
		memset (&fs, 0, sizeof (FATFS));
		res = f_mount(&fs, "", 1);
		if (res != FR_OK) {
			printf("-E- f_mount pb: 0x%X\n\r", res);
			break;
		}
		
		printf ("Opening audio file.\n\r");
		res = f_open(&file_object, (char const *)"0:/monster.wav", FA_OPEN_EXISTING | FA_READ);
		if (res != FR_OK) {
			printf("-E- f_open pb: 0x%X\n\r", res);
			break;
		}
		
		printf ("Skipping WAV header.\n\r");
		res = f_read(&file_object, soundBuffers[0], 44, &bytes_read);
		if ((res != FR_OK) || (bytes_read != 44)) {
			printf("-E- f_read pb: 0x%X\n\r", res);
			printf("-E- f_read bytes: 0x%X\n\r", bytes_read);
			break;
		}

		printf ("WAV header:\n\r");		
		for (i = 0; i < 44; i++) {
			printf ("%02x ", soundBuffers[0][i]);
			if ((i & 3) == 3) {
				printf (" ");
			}
			if ((i & 15) == 15) {
				printf ("\n\r");
			}
		}
		printf ("\n\r");
		
		printf ("Clearing audio buffers.\n\r");
		memset (soundBuffers[0], 0, BUFFER_SIZE_BYTES);
		memset (soundBuffers[1], 0, BUFFER_SIZE_BYTES);
		
		// clear end of file flag
		endOfFile = false;
		lastBlockStarted = false;

		do {
		
			if (!lastBlockStarted) {
				// configure descriptor 0
				my_descriptors[0].DESCADDR.bit.DESCADDR = 0x00000000;
				my_descriptors[0].DSTADDR.bit.DSTADDR = (uint32_t)&(I2S->TXDATA.reg);
				my_descriptors[0].SRCADDR.bit.SRCADDR = soundBuffers[0];
				my_descriptors[0].SRCADDR.bit.SRCADDR += 512;
				my_descriptors[0].BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_WORD_Val;
				my_descriptors[0].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
				my_descriptors[0].BTCTRL.bit.SRCINC = 1;
				my_descriptors[0].BTCTRL.bit.DSTINC = 0;
				my_descriptors[0].BTCTRL.bit.EVOSEL = DMAC_BTCTRL_EVOSEL_DISABLE_Val;
				my_descriptors[0].BTCTRL.bit.BLOCKACT = DMAC_BTCTRL_BLOCKACT_INT_Val;
				my_descriptors[0].BTCTRL.bit.STEPSIZE = DMAC_BTCTRL_STEPSIZE_X1_Val;
				my_descriptors[0].BTCNT.bit.BTCNT = 512 / 4;
				my_descriptors[0].BTCTRL.bit.VALID = 1;

				// start dma playback of buffer 0
				myFlag = false;
				DMAC->Channel[0].CHINTFLAG.bit.TCMPL = 1;
				DMAC->Channel[0].CHINTENSET.bit.TERR = 1;
				DMAC->Channel[0].CHINTENSET.bit.TCMPL = 1;
			    gpio_set_pin_level (PC10, true);
				DMAC->Channel[0].CHCTRLA.bit.ENABLE = 1;
		
				// update last block started flag
				if (endOfFile) {
					lastBlockStarted = true;
				}
			}
		
			if (!endOfFile) {
				// load buffer 1 from file
				res = f_read(&file_object, soundBuffers[1], 512, &bytes_read);
				// if less than 512 bytes read, pad to end of buffer with zeros
				if (bytes_read < 512) {
					printf ("only read %d bytes\n\r", bytes_read);
					memset (&soundBuffers[1][bytes_read], 0, 512 - bytes_read);
					endOfFile = true;
					lastBlockStarted = false;
				}
			}

			// wait for dma playback of buffer 0 to complete
			while (!myFlag) {
			}
			DMAC->Channel[0].CHCTRLA.bit.ENABLE = 0;
			
			if (endOfFile && lastBlockStarted) {
				break;
			}
		
			if (!lastBlockStarted) {
				// configure descriptor 0
				my_descriptors[0].DESCADDR.bit.DESCADDR = 0x00000000;
				my_descriptors[0].DSTADDR.bit.DSTADDR = &(I2S->TXDATA.reg);
				my_descriptors[0].SRCADDR.bit.SRCADDR = soundBuffers[1];
				my_descriptors[0].SRCADDR.bit.SRCADDR += 512;
				my_descriptors[0].BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_WORD_Val;
				my_descriptors[0].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
				my_descriptors[0].BTCTRL.bit.SRCINC = 1;
				my_descriptors[0].BTCTRL.bit.DSTINC = 0;
				my_descriptors[0].BTCTRL.bit.EVOSEL = DMAC_BTCTRL_EVOSEL_DISABLE_Val;
				my_descriptors[0].BTCTRL.bit.BLOCKACT = DMAC_BTCTRL_BLOCKACT_INT_Val;
				my_descriptors[0].BTCTRL.bit.STEPSIZE = DMAC_BTCTRL_STEPSIZE_X1_Val;
				my_descriptors[0].BTCNT.bit.BTCNT = 512 / 4;
				my_descriptors[0].BTCTRL.bit.VALID = 1;

				// start dma playback of buffer 1
				myFlag = false;
				DMAC->Channel[0].CHINTFLAG.bit.TCMPL = 1;
				DMAC->Channel[0].CHINTENSET.bit.TERR = 1;
				DMAC->Channel[0].CHINTENSET.bit.TCMPL = 1;
				DMAC->Channel[0].CHCTRLA.bit.ENABLE = 1;

				// update last block started flag
				if (endOfFile) {
					lastBlockStarted = true;
				}
			}
		
			if (!endOfFile) {
				// load buffer 0 from file
				res = f_read(&file_object, soundBuffers[0], 512, &bytes_read);
				// if less than 512 bytes read, pad to end of buffer with zeros
				if (bytes_read < 512) {
					printf ("only read %d bytes\n\r", bytes_read);
					memset (&soundBuffers[0][bytes_read], 0, 512 - bytes_read);
					endOfFile = true;
					lastBlockStarted = false;
				}
			}

			// wait for dma playback of buffer 1 to complete
			while (!myFlag) {
			}
			DMAC->Channel[0].CHCTRLA.bit.ENABLE = 0;

			if (endOfFile && lastBlockStarted) {
				break;
			}
			
		} while (1);

		printf ("\n\r\n\r");
		
		printf ("Closing audio file.\n\r");
		res = f_close(&file_object);
		if (res != FR_OK) {
			printf("-E- f_close pb: 0x%X\n\r", res);
			break;
		}
		
		printf ("Disabling DMA\n\r");

		// disable channel 0
		DMAC->Channel[0].CHCTRLA.bit.ENABLE = 0;

		// DISABLE ALL THE DMA SHIT
		NVIC_DisableIRQ(DMAC_0_IRQn);
		NVIC_DisableIRQ(DMAC_1_IRQn);
		NVIC_DisableIRQ(DMAC_2_IRQn);
		NVIC_DisableIRQ(DMAC_3_IRQn);
		NVIC_DisableIRQ(DMAC_4_IRQn);
		DMAC->CTRL.bit.DMAENABLE = 0;
		MCLK->AHBMASK.bit.DMAC_ = 0;

	} while (0);
		

	while (1) {
	}
	
	return 0;	
}


void PlayFile (void)
{
}
	
	

/** Size of the file to write/read. */
#define DATA_SIZE 2048

/** Test settings: Number of bytes to test */
#define TEST_SIZE (4 * 1024)

/** Uncomment the macro to Format card and do testing*/
//#define FORMAT_CARD

/* Read/write buffer */
static uint8_t data_buffer[DATA_SIZE];

/**
 * \brief Scan files under a certain path.
 *
 * \param path Folder path.
 *
 * \return Scan result, 1: success.
 */
static FRESULT scan_files(char *path)
{
	FRESULT res;
	FILINFO fno;
	DIR     dir;
	int32_t i;
	char *  pc_fn;
#if _USE_LFN
	char c_lfn[_MAX_LFN + 1];
	fno.lfname = c_lfn;
	fno.lfsize = sizeof(c_lfn);
#endif

	/* Open the directory */
	res = f_opendir(&dir, path);
	if (res == FR_OK) {
		i = strlen(path);
		for (;;) {
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0) {
				break;
			}

#if _USE_LFN
			pc_fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			pc_fn = fno.fname;
#endif
			if (*pc_fn == '.') {
				continue;
			}

			/* Check if it is a directory type */
			if (fno.fattrib & AM_DIR) {
				sprintf(&path[i], "/%s", pc_fn);
				res = scan_files(path);
				if (res != FR_OK) {
					break;
				}

				path[i] = 0;
			} else {
				printf("%s/%s\n\r", path, pc_fn);
			}
		}
	}

	return res;
}

/**
 * \brief Do file system tests.
 *
 * \param disk_dev_num disk number
 *
 * \return Test result, 1: success.
 */
static uint8_t run_fatfs_test(uint32_t disk_dev_num)
{
	uint32_t i;
	UINT     byte_to_read;
	UINT     byte_read;
#if _FS_READONLY == 0
	UINT byte_written;
#endif

	FRESULT res;
	DIR     dirs;
	TCHAR   root_directory[4];
	/* File name to be validated */
	TCHAR file_name[18];

	TCHAR test_folder_name[5];
	TCHAR test_folder_location[8];
	TCHAR get_test_folder[8];
#if _USE_LFN
	char lfn[_MAX_LFN + 1];
	fno.lfname = lfn;
	fno.lfsize = _MAX_LFN + 1;
#endif

	/* Declare these as static to avoid stack usage.
	 * They each contain an array of maximum sector size.
	 */
	static FATFS fs;
	static FIL   file_object;
	char         disk_str_num[2];

	sprintf(disk_str_num, "%d", (int)disk_dev_num);
	sprintf(test_folder_name, "%s", (const char *)"TEST");
	sprintf(root_directory, "%s:/", disk_str_num);
	sprintf(test_folder_location, "%s%s", root_directory, test_folder_name);
	sprintf(file_name, "%s/Basic.bin", test_folder_location); /*File path*/

	/* Mount disk*/
	printf("-I- Mount disk %d\n\r", (int)disk_dev_num);
	/* Clear file system object */
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(&fs, "", 1);
	if (res != FR_OK) {
		printf("-E- f_mount pb: 0x%X\n\r", res);
		return 0;
	}

#ifdef FORMAT_CARD /** Enable the macro to Format card and do testing*/
#if _FS_READONLY == 0
	/* Format disk */
	printf("-I- Format disk %d\n\r", (int)disk_dev_num);
	puts("-I- Please wait a moment during formatting...\r");
	res = f_mkfs(disk_str_num, /* Drv */
	             0,            /* FDISK partition */
	             512);         /* AllocSize */
	puts("-I- Disk format finished !\r");
	if (res != FR_OK) {
		printf("-E- f_mkfs pb: 0x%X\n\r", res);
		return 0;
	}
#else
	puts("-I- Please run Full version FAT FS test first\r");
	return 0;
#endif
#endif

	/*Create a directory*/
	puts("-I- Creating test directory !\r");
	res = f_mkdir(test_folder_name);
	if (res != FR_OK) {
		printf("-E- f_mkdir pb: 0x%X\n\r", res);
		return 0;
	}

	/*Opening the directory*/
	puts("-I- Opening directory !\r");
	res = f_opendir(&dirs, test_folder_location);
	if (res == FR_OK) {
		/* Erase sd card to reformat it ? */
		puts("-I- The disk is already formatted.\r");

		/* Display the file tree */
		puts("-I- Display files contained in the memory :\r");
		strcpy((char *)data_buffer, test_folder_location);
		scan_files((char *)data_buffer);
	}

	/*Changing  the directory*/
	puts("-I- Changing to test directory !\r");
	res = f_chdir(test_folder_location);
	puts("-I- Changed to directory\r");
	if (res != FR_OK) {
		printf("-E- f_chdir pb: 0x%X\n\r", res);
		return 0;
	}

	/*Get the current working directory*/
	puts("-I- Getting current working directory !\r");
	res = f_getcwd(get_test_folder, 8);
	if (res != FR_OK) {
		printf("-E- getcwd not working\n\r");
		return 0;
	}
	res = strcmp(test_folder_location, get_test_folder);
	if (res != FR_OK) {
		printf("-E- Mismatch in f_getcwd and actual Folder name\n\r");
		return 0;
	}

#if _FS_READONLY == 0
	/* Create a new file */
	printf("-I- Create a file : \"%s\"\n\r", file_name);
	res = f_open(&file_object, (char const *)file_name, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	if (res != FR_OK) {
		printf("-E- f_open create pb: 0x%X\n\r", res);
		return 0;
	}

	/* Write a checkerboard pattern in the buffer */
	for (i = 0; i < sizeof(data_buffer); i++) {
		if ((i & 1) == 0) {
			data_buffer[i] = (i & 0x55);
		} else {
			data_buffer[i] = (i & 0xAA);
		}
	}
	puts("-I- Write file\r");
	for (i = 0; i < TEST_SIZE; i += DATA_SIZE) {
		res = f_write(&file_object, data_buffer, DATA_SIZE, &byte_written);

		if (res != FR_OK) {
			printf("-E- f_write pb: 0x%X\n\r", res);
			return 0;
		}
	}

	/* Flush after writing */
	puts("-I- Synching file\r");
	res = f_sync(&file_object);
	if (res != FR_OK) {
		printf("-E- f_sync pb: 0x%X\n\r", res);
		return 0;
	}
	/* Close the file */
	puts("-I- Close file\r");
	res = f_close(&file_object);
	if (res != FR_OK) {
		printf("-E- f_close pb: 0x%X\n\r", res);
		return 0;
	}
#endif
	/* Open the file */
	printf("-I- Open the same file : \"%s\"\n\r", file_name);
	res = f_open(&file_object, (char const *)file_name, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK) {
		printf("-E- f_open read pb: 0x%X\n\r", res);
		return 0;
	}

	/* Read file */
	puts("-I- Read file\r");
	memset(data_buffer, 0, DATA_SIZE);
	byte_to_read = file_object.fsize;

	for (i = 0; i < byte_to_read; i += DATA_SIZE) {
		res = f_read(&file_object, data_buffer, DATA_SIZE, &byte_read);
		if (res != FR_OK) {
			printf("-E- f_read pb: 0x%X\n\r", res);
			return 0;
		}
	}

	/* Close the file*/
	puts("-I- Close file\r");
	res = f_close(&file_object);
	if (res != FR_OK) {
		printf("-E- f_close pb: 0x%X\n\r", res);
		return 0;
	}

	/* Compare the read data with the expected data */
	for (i = 0; i < sizeof(data_buffer); i++) {
		if (!((((i & 1) == 0) && (data_buffer[i] == (i & 0x55))) || (data_buffer[i] == (i & 0xAA)))) {
			printf("Invalid data at data[%u] (expected 0x%02X, read 0x%02X)\n\r",
			       (unsigned int)i,
			       (unsigned int)(((i & 1) == 0) ? (i & 0x55) : (i & 0xAA)),
			       (unsigned int)data_buffer[i]);
		}
	}
	puts("-I- File data Ok !\r");

#if _FS_READONLY == 0
	/*Rename the file*/
	puts("-I- Rename file\r");
	res = f_rename(file_name, "Renam.bin");
	if (res != FR_OK) {
		printf("-E- f_rename pb: 0x%X\n\r", res);
		return 0;
	}

	memset(data_buffer, 0, DATA_SIZE);
	/* Display the file tree */
	puts("-I- Display files contained in the memory after renaming :\r");
	strcpy((char *)data_buffer, test_folder_location);
	scan_files((char *)data_buffer);

	memset(file_name, 0, sizeof(file_name));
	sprintf(file_name, "%s/Renam.bin", test_folder_location);

	/*Change the mode of the file to hidden*/
	puts("-I- Change file mode\r");
	res = f_chmod(file_name, AM_HID, 0);
	if (res != FR_OK) {
		printf("-E- f_chmod pb: 0x%X\n\r", res);
		return 0;
	}

	memset(data_buffer, 0, DATA_SIZE);
	/* Display the file tree */
	puts("-I- Display files contained in the memory after hiding :\r");
	strcpy((char *)data_buffer, test_folder_location);
	scan_files((char *)data_buffer);

#if _USE_FIND == 1
	/*Searching a file in location*/
	res = f_findfirst(&dirs, &fno, "", "Ren*.bin"); /* Start to search for bin files with the name started by "Ren" */
	if (res != FR_OK) {
		printf("-E- f_findfirst pb: 0x%X\n\r", res);
		return 0;
	}
#if _USE_LFN
	printf("-I  file name search found %-12s  %s\n", fno.fname, fno.lfname); /* Display the item name */
#else
	printf("-I  file name search found %s\n", fno.fname); /* Display the item name */
#endif
#endif /*_USE_FIND*/

#if _USE_LABEL == 1
	/*Setting the drive label to ACME*/
	puts("-I- Setting label Name\r");
	res = f_setlabel((const char *)"ACME");
	if (res != FR_OK) {
		printf("-E- f_setlabel pb: 0x%X\n\r", res);
		return 0;
	}
	/*Getting the drive label*/
	puts("-I- Getting label Name\r");
	res = f_getlabel(file_name, &label_name, 0);
	if (res != FR_OK) {
		printf("-E- f_getlabel pb: 0x%X\n\r", res);
		return 0;
	}
	if (strcmp((const char *)"ACME", (const char *)label_name)) {
		printf("-E- f_getlabel/f_setlabel not working pb: 0x%X\n\r", res);
	}
#endif
	puts("-I- Delete file\r");
	res = f_unlink(file_name);
	if (res != FR_OK) {
		printf("-E- f_unlink pb: 0x%X\n\r", res);
		return 0;
	}
	memset(data_buffer, 0, DATA_SIZE);
	/* Display the file tree */
	puts("-I- Display files contained in the memory after deleting :\r");
	strcpy((char *)data_buffer, test_folder_location);
	scan_files((char *)data_buffer);
#endif
	/*Closing directory*/
	res = f_closedir(&dirs);
	if (res != FR_OK) {
		printf("-E- f_closedir pb: 0x%X\n\r", res);
		return 0;
	}
	return 1;
}

uint32_t dma_src[16] = {
	0x11223344, 0x55667788, 0x88776655, 0x44332211,
	0x11111111, 0x22222222, 0x33333333, 0x44444444,
	0xaaaaaaaa, 0x55555555, 0xcccccccc, 0x33333333,
	0xdeadbeef, 0xabad1dea, 0xbeeffeed, 0xdead1dea
};

uint32_t dma_dest[16] = {
	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0
};

volatile int myFlag = 0;

#ifdef __NOPE__
int main(void)
{
	atmel_start_init();
	
	printf ("MCLK.AHBMASK.bit.DMAC_: %d\n\r", MCLK->AHBMASK.bit.DMAC_);
	
	printf ("source: \n\r");
	for (int i = 0; i < 16; i++) {
		printf ("%08x ", dma_src[i]);
		if ((i & 3) == 3) {
			printf ("\n\r");
		}
	}
	printf ("\n\r");

	printf ("destination: \n\r");
	for (int i = 0; i < 16; i++) {
		printf ("%08x ", dma_dest[i]);
		if ((i & 3) == 3) {
			printf ("\n\r");
		}
	}
	printf ("\n\r");

	printf ("DO SOME DMA\n\r\n\r");

	// ENABLE ALL THE DMA SHIT
	MCLK->AHBMASK.bit.DMAC_ = 1;
	DMAC->CTRL.bit.SWRST = 1;
	DMAC->CTRL.bit.DMAENABLE = 0;
	DMAC->BASEADDR.bit.BASEADDR = (uint32_t)my_descriptors;
	DMAC->WRBADDR.bit.WRBADDR = (uint32_t)my_writebacks;
	DMAC->CTRL.bit.LVLEN0 = 1;
	DMAC->CTRL.bit.LVLEN1 = 1;
	DMAC->CTRL.bit.LVLEN2 = 1;
	DMAC->CTRL.bit.LVLEN3 = 1;
	DMAC->CTRL.bit.DMAENABLE = 1;
	
    NVIC_DisableIRQ(DMAC_0_IRQn);
    NVIC_ClearPendingIRQ(DMAC_0_IRQn);
    NVIC_EnableIRQ(DMAC_0_IRQn);
    NVIC_SetPriority(DMAC_0_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

    NVIC_DisableIRQ(DMAC_1_IRQn);
    NVIC_ClearPendingIRQ(DMAC_1_IRQn);
    NVIC_EnableIRQ(DMAC_1_IRQn);
    NVIC_SetPriority(DMAC_1_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

    NVIC_DisableIRQ(DMAC_2_IRQn);
    NVIC_ClearPendingIRQ(DMAC_2_IRQn);
    NVIC_EnableIRQ(DMAC_2_IRQn);
    NVIC_SetPriority(DMAC_2_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

    NVIC_DisableIRQ(DMAC_3_IRQn);
    NVIC_ClearPendingIRQ(DMAC_3_IRQn);
    NVIC_EnableIRQ(DMAC_3_IRQn);
    NVIC_SetPriority(DMAC_3_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

    NVIC_DisableIRQ(DMAC_4_IRQn);
    NVIC_ClearPendingIRQ(DMAC_4_IRQn);
    NVIC_EnableIRQ(DMAC_4_IRQn);
    NVIC_SetPriority(DMAC_4_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

	// clear the channel 0 descriptor, enable, and reset it
	memset((void*)&my_descriptors[0], 0x00, sizeof(my_descriptors[0]));
	memset((void*)&my_writebacks[0], 0x00, sizeof(my_writebacks[0]));
	DMAC->Channel[0].CHCTRLA.bit.ENABLE = 0;
	DMAC->Channel[0].CHCTRLA.bit.SWRST = 1;	
	
	// set priority level 0
	DMAC->Channel[0].CHPRILVL.bit.PRILVL = 0;
	
	// set trigger source
	DMAC->Channel[0].CHCTRLA.bit.TRIGSRC = DMAC_CHCTRLA_TRIGSRC_DISABLE_Val; // software trigger
	DMAC->Channel[0].CHCTRLA.bit.TRIGACT = DMAC_CHCTRLA_TRIGACT_BLOCK_Val;   // one trigger per block
  	
	// configure descriptor 0
	my_descriptors[0].DESCADDR.bit.DESCADDR = 0x00000000;
	my_descriptors[0].DSTADDR.bit.DSTADDR = dma_dest;
	my_descriptors[0].SRCADDR.bit.SRCADDR = dma_src;
	my_descriptors[0].BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_WORD_Val;
	my_descriptors[0].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
	my_descriptors[0].BTCTRL.bit.SRCINC = 1;
	my_descriptors[0].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_DST_Val;
	my_descriptors[0].BTCTRL.bit.DSTINC = 1;
	my_descriptors[0].BTCTRL.bit.EVOSEL = DMAC_BTCTRL_EVOSEL_DISABLE_Val;
	my_descriptors[0].BTCTRL.bit.BLOCKACT = DMAC_BTCTRL_BLOCKACT_NOACT_Val;
	my_descriptors[0].BTCTRL.bit.STEPSIZE = DMAC_BTCTRL_STEPSIZE_X1;
	my_descriptors[0].BTCNT.bit.BTCNT = 64 / 4;
	
	if (my_descriptors[0].BTCTRL.bit.SRCINC) {
		// if increment source is set, the source address must be the end address
		my_descriptors[0].SRCADDR.bit.SRCADDR += 64;
	}

	if (my_descriptors[0].BTCTRL.bit.DSTINC) {
		// if increment destination is set, the destination address must be the end address
		my_descriptors[0].DSTADDR.bit.DSTADDR += 64;
	}

	my_descriptors[0].BTCTRL.bit.VALID = 1;

	// DMAC->Channel[0].CHINTENSET.bit.TERR = 1;
	// DMAC->Channel[0].CHINTENSET.bit.TCMPL = 1;
	DMAC->Channel[0].CHCTRLA.bit.ENABLE = 1;

	if (DMAC->Channel[0].CHCTRLA.bit.TRIGSRC == 0) {
		// uses software trigger, so trigger it
		DMAC->SWTRIGCTRL.reg |= (1 << 0);
	}

	delay_ms (100);

	printf ("DMA DONE\n\r\n\r");
	
	printf ("source: \n\r");
	for (int i = 0; i < 16; i++) {
		printf ("%08x ", dma_src[i]);
		if ((i & 3) == 3) {
			printf ("\n\r");
		}
	}
	printf ("\n\r");

	printf ("destination: \n\r");
	for (int i = 0; i < 16; i++) {
		printf ("%08x ", dma_dest[i]);
		if ((i & 3) == 3) {
			printf ("\n\r");
		}
	}
	printf ("\n\r");

	// disable channel 0
	DMAC->Channel[0].CHCTRLA.bit.ENABLE = 0;
	
/*		
	my_descriptors[0].BTCNT = 0;
	my_descriptors[0].BTCTRL = 0;
	
	my_descriptors[1].DESCADDR = 0x00000000;
	my_descriptors[1].DSTADDR = 0;
	my_descriptors[1].SRCADDR = 0;
	my_descriptors[1].BTCNT = 0;
	my_descriptors[1].BTCTRL = 0;
	
	my_writebacks[0].DESCADDR = 0;
	my_writebacks[0].DSTADDR = 0;
	my_writebacks[0].SRCADDR = 0;
	my_writebacks[0].BTCNT = 0;
	my_writebacks[0].BTCTRL = 0;
	
	my_writebacks[1].DESCADDR = 0;
	my_writebacks[1].DSTADDR = 0;
	my_writebacks[1].SRCADDR = 0;
	my_writebacks[1].BTCNT = 0;
	my_writebacks[1].BTCTRL = 0;
*/
	

	// DISABLE ALL THE DMA SHIT
	NVIC_DisableIRQ(DMAC_0_IRQn);
	NVIC_DisableIRQ(DMAC_1_IRQn);
	NVIC_DisableIRQ(DMAC_2_IRQn);
	NVIC_DisableIRQ(DMAC_3_IRQn);
	NVIC_DisableIRQ(DMAC_4_IRQn);
	DMAC->CTRL.bit.DMAENABLE = 0;
	MCLK->AHBMASK.bit.DMAC_ = 0;

	
	
	
	printf ("%08x\n\r", I2S->INTFLAG);
    I2S->TXDATA.reg = 0x80000000;
	printf ("%08x\n\r", I2S->INTFLAG);

	printf ("CC0: %08x\n\r", I2S->CLKCTRL[0].reg);
	printf ("CC1: %08x\n\r", I2S->CLKCTRL[1].reg);
	printf ("TXC: %08x\n\r", I2S->TXCTRL.reg);
	
	// I2S->CTRLA.bit.TXEN = 0;
	// I2S->TXCTRL.bit.DATASIZE = 5;
	// I2S->CTRLA.bit.TXEN = 1;
	
	printf ("CC0: %08x\n\r", I2S->CLKCTRL[0].reg);
	printf ("CC1: %08x\n\r", I2S->CLKCTRL[1].reg);
	printf ("TXC: %08x\n\r", I2S->TXCTRL.reg);
		
	uint32_t j = 0;
	uint32_t i = 0;
	volatile uint32_t s = 0;
	while (1) {
		while (!(I2S->INTFLAG.reg & 0x100)) {
			if (I2S->INTFLAG.reg & 0x1000) {
				I2S->INTFLAG.bit.TXUR0 = 1;
			}
		}
		//                  RRRRLLLL
		I2S->TXDATA.reg = 0xAAAACC33;
/*
		while (!(I2S->INTFLAG.reg & 0x100)) {
			if (I2S->INTFLAG.reg & 0x1000) {
				I2S->INTFLAG.bit.TXUR0 = 1;
			}
		}
		I2S->TXDATA.reg = 0xcc33;
*/
/*		
		j++;
		if (j == 47999) {
			putchar ('.');
			j = 0;
		}
*/
	}


	if (run_fatfs_test(0)) {
		printf("-I- DISK 0 Test passed !\n\r\n\r");
	} else {
		printf("-F- DISK 0 Test Failed !\n\r\n\r");
	}
	while (1)
		;
	return 0;
}
#endif

void DMAC_0_Handler (void)
{
	DMAC->Channel[0].CHINTFLAG.bit.TCMPL = 1;
    gpio_set_pin_level (PC10, false);
    gpio_set_pin_level (PC01, true);
	myFlag = true;
    gpio_set_pin_level (PC01, false);

	// printf ("DMAC_0_Handler: %08x\n\r", DMAC->Channel[0].CHINTFLAG.reg);
}

void DMAC_1_Handler (void)
{
	printf ("DMAC_0_Handler: %08x\n\r", DMAC->Channel[0].CHINTFLAG.reg);
}

void DMAC_2_Handler (void)
{
	printf ("DMAC_0_Handler: %08x\n\r", DMAC->Channel[0].CHINTFLAG.reg);
}

void DMAC_3_Handler (void)
{
	printf ("DMAC_0_Handler: %08x\n\r", DMAC->Channel[0].CHINTFLAG.reg);
}

void DMAC_4_Handler (void)
{
	printf ("DMAC_0_Handler: %08x\n\r", DMAC->Channel[0].CHINTFLAG.reg);
}
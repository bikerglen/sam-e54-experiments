/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"
#include <hpl_dmac_config.h>
#include <hpl_dma.h>

// To use I2S example, enable DMA and uncomment below code

/*
#if CONF_DMAC_ENABLE == 0
#warning "DMAC system driver needs to be enabled to use I2S driver"
#endif

void I2S_0_example(void)
{
uint8_t i2sc_dma_channel_number = 0;

    uint32_t i2sc_data[16] = {0xAAAAAAAA, 0x55555555, 0x12341234, 0x56785678,
        0xAAAAAAAB, 0x55555556, 0x12341235, 0x56785679,
        0xAAAAAAAC, 0x55555557, 0x12341236, 0x5678567A,
        0xAAAAAAAD, 0x55555558, 0x12341237, 0x5678567B};

	while (1) {
     // Configure the DMAC source address, destination address,
     // next descriptor address, data count and Enable the DMAC Channel
    _dma_set_source_address(i2sc_dma_channel_number, (const void *)i2sc_data);
    _dma_set_destination_address(i2sc_dma_channel_number, \
    (const void *) &I2S->TXDATA.reg);
    _dma_set_data_amount(i2sc_dma_channel_number, ((sizeof i2sc_data) / 4));
    _dma_enable_transaction(i2sc_dma_channel_number, false);

	printf ("dma started\n\r");
	
	delay_ms (1000);
    }

}
*/

/**
 * Example of using CALENDER_INTERFACE.
 */
static struct calendar_alarm alarm;

static void alarm_cb(struct calendar_descriptor *const descr)
{
	/* alarm expired */
}

void CALENDER_INTERFACE_example(void)
{
	struct calendar_date date;
	struct calendar_time time;

	calendar_enable(&CALENDER_INTERFACE);

	date.year  = 2000;
	date.month = 12;
	date.day   = 31;

	time.hour = 12;
	time.min  = 59;
	time.sec  = 59;

	calendar_set_date(&CALENDER_INTERFACE, &date);
	calendar_set_time(&CALENDER_INTERFACE, &time);

	alarm.cal_alarm.datetime.time.sec = 4;
	alarm.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
	alarm.cal_alarm.mode              = REPEAT;

	calendar_set_alarm(&CALENDER_INTERFACE, &alarm, alarm_cb);
}

/**
 * Example of using TARGET_IO to write "Hello World" using the IO abstraction.
 */
void TARGET_IO_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&TARGET_IO, &io);
	usart_sync_enable(&TARGET_IO);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

/* Auto-generated config file hpl_i2s_config.h */
#ifndef HPL_I2S_CONFIG_H
#define HPL_I2S_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// I2S0 is used
#ifndef CONF_I2S_0_USED
#define CONF_I2S_0_USED 1
#endif

// CTRLA.CKEN0
#ifndef CONF_I2S_0_CKEN0
#define CONF_I2S_0_CKEN0 1
#endif

// CTRLA.CKEN1
#ifndef CONF_I2S_0_CKEN1
#define CONF_I2S_0_CKEN1 1
#endif

// CTRLA.SEREN0
#ifndef CONF_I2S_0_SEREN0
#define CONF_I2S_0_SEREN0 1
#endif

// CTRLA.SEREN1
#ifndef CONF_I2S_0_SEREN1
#define CONF_I2S_0_SEREN1 1
#endif

// CLKCTRL0.SCKSEL
#ifndef CONF_I2S_0_SCKSEL_0
#define CONF_I2S_0_SCKSEL_0 0
#endif

// CLKCTRL0.FSSEL
#ifndef CONF_I2S_0_FSSEL_0
#define CONF_I2S_0_FSSEL_0 0
#endif

// CLKCTRL1.SCKSEL
#ifndef CONF_I2S_0_SCKSEL_1
#define CONF_I2S_0_SCKSEL_1 0
#endif

// CLKCTRL1.FSSEL
#ifndef CONF_I2S_0_FSSEL_1
#define CONF_I2S_0_FSSEL_1 0
#endif

// SERCTRL0.CLKSEL
#ifndef CONF_I2S_0_CLKSEL_0
#define CONF_I2S_0_CLKSEL_0 0
#endif

// SERCTRL1.CLKSEL
#ifndef CONF_I2S_0_CLKSEL_1
#define CONF_I2S_0_CLKSEL_1 1
#endif

// <h> Transmitter Control Signals Basic Settings

// <o> SCK generation division <1-64>
// <i> SCK generation division (MCKDIV + 1) from Master Clock (GCLK).
// <id> i2s_mckdiv_0
#ifndef CONF_I2S_0_MCKDIV_0
#define CONF_I2S_0_MCKDIV_0 4
#endif

// <o.0..6> FrameSync/WordSelect Length
// <i> FrameSync/WordSelect length, in number of SCK clocks.
// <0x04=> 8 SCK
// <0x05=> 16 SCK
// <0x06=> 24 SCK
// <0x07=> 32 SCK
// <0x2B=> 48 SCK
// <0x2F=> 64 SCK
// <0x36=> 72 SCK
// <0x37=> 96 SCK
// <0x3F=> 128 SCK
// <id> i2s_fs_size_cfg_0
#ifndef CONF_I2S_0_FS_SIZE_CFG_0
#define CONF_I2S_0_FS_SIZE_CFG_0 0x07
#endif

// CLKCTRL.FSWIDTH
#define CONF_I2S_0_FSWIDTH_0 ((CONF_I2S_0_FS_SIZE_CFG_0 >> 5) & 0x3)
// CLKCTRL.NBSLOTS

#define CONF_I2S_0_NBSLOTS_0 ((CONF_I2S_0_FS_SIZE_CFG_0 >> 2) & 0x7)
// CLKCTRL.SLOTSIZE

#define CONF_I2S_0_SLOTSIZE_0 ((CONF_I2S_0_FS_SIZE_CFG_0 >> 0) & 0x3)
// </h>
// <e> Transmitter Control Signals Advanced Settings
// <id> i2s_arch_Transmitter_advanced_ctrl
#ifndef CONF_I2S_0_CTRL_ADVANCED_0
#define CONF_I2S_0_CTRL_ADVANCED_0 1
#endif

// <o> Transmitter Master Clock (MCK) source selection
// <i> Even we are master, you can still use more accurate clock from MCK pin to replace GCLK
// <i> If MCK is from MCK pin input, the MCK output settings will be ignored
// <0=> use GCLK
// <1=> use MCK pin input
// <id> i2s_arch_mcksel_0
#ifndef CONF_I2S_0_MCKSEL_0
#define CONF_I2S_0_MCKSEL_0 0
#endif

// <q> Transmitter Master Clock (MCK) output Enable
// <id> i2s_arch_mcken_0
#ifndef CONF_I2S_0_MCKEN_0
#define CONF_I2S_0_MCKEN_0 1
#endif

// <q> Transmitter Master Clock (MCK) output invert
// <id> i2s_arch_mckoutinv_0
#ifndef CONF_I2S_0_MCKOUTINV_0
#define CONF_I2S_0_MCKOUTINV_0 0
#endif

// <o> Transmitter Master Clock (MCK) output division <1-64>
// <i> MCK output generation division(MCKOUTDIV + 1) from actual Master Clock (GCLK).
// <id> i2s_arch_mckoutdiv_0
#ifndef CONF_I2S_0_MCKOUTDIV_0
#define CONF_I2S_0_MCKOUTDIV_0 1
#endif

// <q> SerialClock (SCK) output invert
// <id> i2s_arch_sckoutinv_0
#ifndef CONF_I2S_0_SCKOUTINV_0
#define CONF_I2S_0_SCKOUTINV_0 0
#endif

// <q> FrameSync (FS) output invert
// <id> i2s_arch_fsoutinv_0
#ifndef CONF_I2S_0_FSOUTINV_0
#define CONF_I2S_0_FSOUTINV_0 1
#endif

// </e>

// <h> Transmitter Data Streaming Basic Settings
// <o> Data word size
// <0=> 32 bits
// <1=> 24 bits
// <2=> 20 bits
// <3=> 18 bits
// <4=> 16 bits
// <6=> 8 bits
// <id> i2s_datasize_0
#ifndef CONF_I2S_0_DATASIZE_0
#define CONF_I2S_0_DATASIZE_0 5
#endif

// </h>

// <e> Transmitter Data Streaming Advanced Settings
// <id> i2s_arch_Transmitter_data_advanced_crtl
#ifndef CONF_I2S_0_DATA_ADVANCED_0
#define CONF_I2S_0_DATA_ADVANCED_0 1
#endif
// <q> Disable data streaming
// <i> When using I2S driver, it's still possible to disable master data streaming to get similar behavior as controller.
// <i> To optimize the driver, if no I2S streaming needs, use the I2S Controller driver.
// <id> i2s_arch_data_off_0
#ifndef CONF_I2S_0_DATA_OFF_0
#define CONF_I2S_0_DATA_OFF_0 0
#endif
// No option for loop back test
// <id> i2s_arch_rxloop_0
#ifndef CONF_I2S_0_RXLOOP_0
#define CONF_I2S_0_RXLOOP_0 0
#endif

// <q> Mono mode
// <i> Automatically extend single channel input to dual channel
// <i> Note that it requires single channel formatted input data, which is not stantard one
// <i> Default: disabled
// <id> i2s_arch_mono_0
#ifndef CONF_I2S_0_MONO_0
#define CONF_I2S_0_MONO_0 0x0
#endif

// CLKCTRL.FSINV
// <q> FrameSync/WordSelect invert (switch left/right channel)
// <i> FrameSync is inverted before being used, so that left and right channels are swapped
// <id> i2s_arch_fsinv_0
#ifndef CONF_I2S_0_FSINV_0
#define CONF_I2S_0_FSINV_0 0
#endif

// CLKCTRL.BITDELAY
// <o> Data Bit Delay (from FrameSync/WordSelect Edge)
// <1=> 1 Bit (I2S standard)
// <0=> No delay (Left justified)
// <id> i2s_bitdelay_0
#ifndef CONF_I2S_0_BITDELAY_0
#define CONF_I2S_0_BITDELAY_0 1
#endif

// </e>

// SERCTRL.DMA: fixed to single channel
#define CONF_I2S_0_DMA_0 0x0

// SERCTRL.SLOTDIS: calculate from bitrate
#define CONF_I2S_0_SLOTDIS_0                                                                                           \
	((CONF_I2S_0_FSWIDTH_0 == 0)                                                                                       \
	     ? 0xFC                                                                                                        \
	     : ((CONF_I2S_0_NBSLOTS_0 == 3)                                                                                \
	            ? 0xFA                                                                                                 \
	            : ((CONF_I2S_0_NBSLOTS_0 == 5) ? 0xF6 : ((CONF_I2S_0_NBSLOTS_0 == 7) ? 0xEE : 0xFF))))

// SERCTRL.BITREV: fixed to MSBIT
#define CONF_I2S_0_BITREV_0 0x0

// SERCTRL.EXTEND: fixed to ZERO
#define CONF_I2S_0_EXTEND_0 0x0

// SERCTRL.WORDADJ: fixed to RIGHT adjusted
#define CONF_I2S_0_WORDADJ_0 0x0

// SERCTRL.SLOTADJ: fixed to LEFT adjusted
#define CONF_I2S_0_SLOTADJ_0 (0x1u << 7)

// SERCTRL.TXSAME: fixed to ZERO
#define CONF_I2S_0_TXSAME_0 0x0

// SERCTRL.TXDEFAULT: fixed to ZERO
#define CONF_I2S_0_TXDEFAULT_0 0x0
// SERCTRL.SERMODE: fixed to TX
#define CONF_I2S_0_SERMODE_0 1
// <h> Receiver Control Signals Basic Settings

// <o> SCK generation division <1-64>
// <i> SCK generation division (MCKDIV + 1) from Master Clock (GCLK).
// <id> i2s_mckdiv_1
#ifndef CONF_I2S_0_MCKDIV_1
#define CONF_I2S_0_MCKDIV_1 4
#endif

// <o.0..6> FrameSync/WordSelect Length
// <i> FrameSync/WordSelect length, in number of SCK clocks.
// <0x04=> 8 SCK
// <0x05=> 16 SCK
// <0x06=> 24 SCK
// <0x07=> 32 SCK
// <0x2B=> 48 SCK
// <0x2F=> 64 SCK
// <0x36=> 72 SCK
// <0x37=> 96 SCK
// <0x3F=> 128 SCK
// <id> i2s_fs_size_cfg_1
#ifndef CONF_I2S_0_FS_SIZE_CFG_1
#define CONF_I2S_0_FS_SIZE_CFG_1 0x2f
#endif

// CLKCTRL.FSWIDTH
#define CONF_I2S_0_FSWIDTH_1 ((CONF_I2S_0_FS_SIZE_CFG_1 >> 5) & 0x3)
// CLKCTRL.NBSLOTS

#define CONF_I2S_0_NBSLOTS_1 ((CONF_I2S_0_FS_SIZE_CFG_1 >> 2) & 0x7)
// CLKCTRL.SLOTSIZE

#define CONF_I2S_0_SLOTSIZE_1 ((CONF_I2S_0_FS_SIZE_CFG_1 >> 0) & 0x3)
// </h>
// <e> Receiver Control Signals Advanced Settings
// <id> i2s_arch_Receiver_advanced_ctrl
#ifndef CONF_I2S_0_CTRL_ADVANCED_1
#define CONF_I2S_0_CTRL_ADVANCED_1 1
#endif

// <o> Receiver Master Clock (MCK) source selection
// <i> Even we are master, you can still use more accurate clock from MCK pin to replace GCLK
// <i> If MCK is from MCK pin input, the MCK output settings will be ignored
// <0=> use GCLK
// <1=> use MCK pin input
// <id> i2s_arch_mcksel_1
#ifndef CONF_I2S_0_MCKSEL_1
#define CONF_I2S_0_MCKSEL_1 0
#endif

// <q> Receiver Master Clock (MCK) output Enable
// <id> i2s_arch_mcken_1
#ifndef CONF_I2S_0_MCKEN_1
#define CONF_I2S_0_MCKEN_1 0
#endif

// <q> Receiver Master Clock (MCK) output invert
// <id> i2s_arch_mckoutinv_1
#ifndef CONF_I2S_0_MCKOUTINV_1
#define CONF_I2S_0_MCKOUTINV_1 0
#endif

// <o> Receiver Master Clock (MCK) output division <1-64>
// <i> MCK output generation division(MCKOUTDIV + 1) from actual Master Clock (GCLK).
// <id> i2s_arch_mckoutdiv_1
#ifndef CONF_I2S_0_MCKOUTDIV_1
#define CONF_I2S_0_MCKOUTDIV_1 1
#endif

// <q> SerialClock (SCK) output invert
// <id> i2s_arch_sckoutinv_1
#ifndef CONF_I2S_0_SCKOUTINV_1
#define CONF_I2S_0_SCKOUTINV_1 0
#endif

// <q> FrameSync (FS) output invert
// <id> i2s_arch_fsoutinv_1
#ifndef CONF_I2S_0_FSOUTINV_1
#define CONF_I2S_0_FSOUTINV_1 0
#endif

// </e>

// <h> Receiver Data Streaming Basic Settings
// <o> Data word size
// <0=> 32 bits
// <1=> 24 bits
// <2=> 20 bits
// <3=> 18 bits
// <4=> 16 bits
// <6=> 8 bits
// <id> i2s_datasize_1
#ifndef CONF_I2S_0_DATASIZE_1
#define CONF_I2S_0_DATASIZE_1 4
#endif

// </h>

// <e> Receiver Data Streaming Advanced Settings
// <id> i2s_arch_Receiver_data_advanced_crtl
#ifndef CONF_I2S_0_DATA_ADVANCED_1
#define CONF_I2S_0_DATA_ADVANCED_1 1
#endif
// <q> Disable data streaming
// <i> When using I2S driver, it's still possible to disable master data streaming to get similar behavior as controller.
// <i> To optimize the driver, if no I2S streaming needs, use the I2S Controller driver.
// <id> i2s_arch_data_off_1
#ifndef CONF_I2S_0_DATA_OFF_1
#define CONF_I2S_0_DATA_OFF_1 1
#endif
// No option for loop back test
// <id> i2s_arch_rxloop_1
#ifndef CONF_I2S_0_RXLOOP_1
#define CONF_I2S_0_RXLOOP_1 0
#endif

// <q> Mono mode
// <i> Automatically extend single channel input to dual channel
// <i> Note that it requires single channel formatted input data, which is not stantard one
// <i> Default: disabled
// <id> i2s_arch_mono_1
#ifndef CONF_I2S_0_MONO_1
#define CONF_I2S_0_MONO_1 0x0
#endif

// CLKCTRL.FSINV
// <q> FrameSync/WordSelect invert (switch left/right channel)
// <i> FrameSync is inverted before being used, so that left and right channels are swapped
// <id> i2s_arch_fsinv_1
#ifndef CONF_I2S_0_FSINV_1
#define CONF_I2S_0_FSINV_1 0
#endif

// CLKCTRL.BITDELAY
// <o> Data Bit Delay (from FrameSync/WordSelect Edge)
// <1=> 1 Bit (I2S standard)
// <0=> No delay (Left justified)
// <id> i2s_bitdelay_1
#ifndef CONF_I2S_0_BITDELAY_1
#define CONF_I2S_0_BITDELAY_1 1
#endif

// </e>

// SERCTRL.DMA: fixed to single channel
#define CONF_I2S_0_DMA_1 0x0

// SERCTRL.SLOTDIS: calculate from bitrate
#define CONF_I2S_0_SLOTDIS_1                                                                                           \
	((CONF_I2S_0_FSWIDTH_1 == 0)                                                                                       \
	     ? 0xFC                                                                                                        \
	     : ((CONF_I2S_0_NBSLOTS_1 == 3)                                                                                \
	            ? 0xFA                                                                                                 \
	            : ((CONF_I2S_0_NBSLOTS_1 == 5) ? 0xF6 : ((CONF_I2S_0_NBSLOTS_1 == 7) ? 0xEE : 0xFF))))

// SERCTRL.BITREV: fixed to MSBIT
#define CONF_I2S_0_BITREV_1 0x0

// SERCTRL.EXTEND: fixed to ZERO
#define CONF_I2S_0_EXTEND_1 0x0

// SERCTRL.WORDADJ: fixed to RIGHT adjusted
#define CONF_I2S_0_WORDADJ_1 0x0

// SERCTRL.SLOTADJ: fixed to LEFT adjusted
#define CONF_I2S_0_SLOTADJ_1 (0x1u << 7)

// SERCTRL.TXSAME: fixed to ZERO
#define CONF_I2S_0_TXSAME_1 0x0

// SERCTRL.TXDEFAULT: fixed to ZERO
#define CONF_I2S_0_TXDEFAULT_1 0x0
// SERCTRL.SERMODE: fixed to RX
#define CONF_I2S_0_SERMODE_1 0

// <<< end of configuration section >>>

#endif // HPL_I2S_CONFIG_H

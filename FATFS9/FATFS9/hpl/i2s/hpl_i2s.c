/**
 * \file
 *
 * \brief SAM I2S HPL
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include <hpl_i2s.h>
#include <hpl_i2s_config.h>
#include <utils_assert.h>

/** I2S (I2S0) peripheral is used. */
#ifndef CONF_I2S_0_USED
#define CONF_I2S_0_USED 0
#endif

/** I2S usage flags. */
#define I2S_USED_FLAGS ((CONF_I2S_0_USED << 0))

/** Amount of I2S peripherals instances used. */
#define I2S_USED_AMOUNT (CONF_I2S_0_USED)

#ifdef __cplusplus
extern "C" {
#endif

/** Static configuration value for ctrla register. */
#define CONF_I2S_CTRLA(i)                                                                                              \
	(((CONF_I2S_##i##_SEREN0 && !CONF_I2S_##i##_DATA_OFF_0) << I2S_CTRLA_TXEN_Pos)                                     \
	 | ((CONF_I2S_##i##_SEREN1 && !CONF_I2S_##i##_DATA_OFF_1) << I2S_CTRLA_RXEN_Pos)                                   \
	 | ((CONF_I2S_##i##_CKEN0) << I2S_CTRLA_CKEN0_Pos) | ((CONF_I2S_##i##_CKEN1) << I2S_CTRLA_CKEN1_Pos))

/** Enable masks for specific interface. */
#define I2S_CTRLA_EN_MASK(iface, iface_ctrl) ((I2S_CTRLA_CKEN0 << (iface_ctrl)) | (I2S_CTRLA_TXEN << (iface)))

/** Static CLKCTRL configuration value for specific interface. */
#define CONF_I2S_CLKCTRL(i, n)                                                                                         \
	((CONF_I2S_##i##_MCKOUTINV_##n << I2S_CLKCTRL_MCKOUTINV_Pos)                                                       \
	 | (CONF_I2S_##i##_SCKOUTINV_##n << I2S_CLKCTRL_SCKOUTINV_Pos)                                                     \
	 | (CONF_I2S_##i##_FSOUTINV_##n << I2S_CLKCTRL_FSOUTINV_Pos)                                                       \
	 | I2S_CLKCTRL_MCKOUTDIV(CONF_I2S_##i##_MCKOUTDIV_##n - 1) | I2S_CLKCTRL_MCKDIV(CONF_I2S_##i##_MCKDIV_##n - 1)     \
	 | (CONF_I2S_##i##_MCKEN_##n << I2S_CLKCTRL_MCKEN_Pos) | (CONF_I2S_##i##_MCKSEL_##n << I2S_CLKCTRL_MCKSEL_Pos)     \
	 | (CONF_I2S_##i##_SCKSEL_##n << I2S_CLKCTRL_SCKSEL_Pos) | (CONF_I2S_##i##_FSINV_##n << I2S_CLKCTRL_FSINV_Pos)     \
	 | (CONF_I2S_##i##_FSSEL_##n << I2S_CLKCTRL_FSSEL_Pos) | (CONF_I2S_##i##_BITDELAY_##n << I2S_CLKCTRL_BITDELAY_Pos) \
	 | CONF_I2S_##i##_FS_SIZE_CFG_##n)

/** Static SERCTRL configuration value for specific interface. */
#define CONF_I2S_TXCTRL(i)                                                                                             \
	((CONF_I2S_##i##_DMA_0 << I2S_TXCTRL_DMA_Pos) | (CONF_I2S_##i##_MONO_0 << I2S_TXCTRL_MONO_Pos)                     \
	 | I2S_TXCTRL_SLOTDIS(CONF_I2S_##i##_SLOTDIS_0) | (CONF_I2S_##i##_BITREV_0 << I2S_TXCTRL_BITREV_Pos)               \
	 | I2S_TXCTRL_EXTEND(CONF_I2S_##i##_EXTEND_0) | (CONF_I2S_##i##_WORDADJ_0 << I2S_TXCTRL_WORDADJ_Pos)               \
	 | I2S_TXCTRL_DATASIZE(CONF_I2S_##i##_DATASIZE_0) | (CONF_I2S_##i##_SLOTADJ_0)                                     \
	 | (CONF_I2S_##i##_TXSAME_0 << I2S_TXCTRL_TXSAME_Pos) | I2S_TXCTRL_TXDEFAULT(CONF_I2S_##i##_TXDEFAULT_0))
#define CONF_I2S_RXCTRL(i)                                                                                             \
	((CONF_I2S_##i##_RXLOOP_1 << I2S_RXCTRL_RXLOOP_Pos) | (CONF_I2S_##i##_DMA_1 << I2S_RXCTRL_DMA_Pos)                 \
	 | (CONF_I2S_##i##_MONO_1 << I2S_RXCTRL_MONO_Pos) | I2S_RXCTRL_SLOTDIS(CONF_I2S_##i##_SLOTDIS_1)                   \
	 | (CONF_I2S_##i##_BITREV_1 << I2S_RXCTRL_BITREV_Pos) | I2S_RXCTRL_EXTEND(CONF_I2S_##i##_EXTEND_1)                 \
	 | (CONF_I2S_##i##_WORDADJ_1 << I2S_RXCTRL_WORDADJ_Pos) | I2S_RXCTRL_DATASIZE(CONF_I2S_##i##_DATASIZE_1)           \
	 | (CONF_I2S_##i##_SLOTADJ_1) | (CONF_I2S_##i##_CLKSEL_1 << I2S_RXCTRL_CLKSEL_Pos)                                 \
	 | I2S_RXCTRL_SERMODE(CONF_I2S_##i##_SERMODE_1))

/** I2S init configurations. */
struct _i2s_init_cfg {
	/** CLKCTRLn. */
	uint32_t clkctrl[2];
	/** SERCTRLn. */
	uint32_t serctrl[2];
	/** CTRLA. */
	uint8_t ctrla;
	/** Peripheral index. */
	uint8_t i;
};

/** I2S init configuration struct values. */
#define I2S_INIT_CFG(i)                                                                                                \
	{                                                                                                                  \
		{CONF_I2S_CLKCTRL(i, 0), CONF_I2S_CLKCTRL(i, 1)}, {CONF_I2S_TXCTRL(i), CONF_I2S_RXCTRL(i)}, CONF_I2S_CTRLA(i), \
		    i                                                                                                          \
	}

/** Check if there is more configuration. */
#define I2S_MORE(i) (~((1 << ((i) + 1)) - 1) & I2S_USED_FLAGS)

/** I2S initialization configurations. */
static const struct _i2s_init_cfg _init_cfg[I2S_USED_AMOUNT] = {
#if CONF_I2S_0_USED
    I2S_INIT_CFG(0)
#if I2S_MORE(0)
        ,
#endif
#endif
};

/** Get bit to indicate the usage of I2S interface. */
#define _i2s_iface_use_flag(i, iface) (1 << (((i) << 1) + (iface)))

/** Used bitmap for I2S interfaces. */
static uint32_t _iface_use_flags = 0;

/** Get hardware peripheral index.
 * \param[in] hw Pointer to hardware base.
 * \return index of peripheral.
 */
static uint8_t _i2s_get_index(const void *hw)
{
#ifdef I2S
	if (I2S == hw)
		return 0;
#endif
#ifdef I2S0
	if (I2S0 == hw)
		return 0;
#endif
	(void)hw;
	return 0;
}

/** Get pointer to the init configurations
 * \param[in] index Peripheral index.
 * \return Pointer to the init configurations.
 */
static const struct _i2s_init_cfg *_i2s_get_cfg(const uint8_t index)
{
	uint8_t i;
	for (i = 0; i < I2S_USED_AMOUNT; i++) {
		if (_init_cfg[i].i == index) {
			return &_init_cfg[i];
		}
	}
	return NULL;
}

/**
 * Take the I2S interface and tag it as occupied.
 * \param[in] i Hardware peripheral index.
 * \param[in] iface The interface to take.
 * \return \c true If success.
 */
static bool _i2s_iface_take(const uint8_t i, const enum _i2s_iface iface)
{
	uint32_t bit = _i2s_iface_use_flag(i, iface);
	if (_iface_use_flags & bit) {
		return false;
	}
	_iface_use_flags |= bit;
	return true;
}

/**
 * Free the I2S interface.
 * \param[in] i Hardware peripheral index.
 * \param[in] iface The interface to free.
 */
static void _i2s_iface_free(const uint8_t i, const enum _i2s_iface iface)
{
	uint32_t bit = _i2s_iface_use_flag(i, iface);
	_iface_use_flags &= ~bit;
}

/** Sync busy bits for CTRLA register. */
#define _I2S_CTRLA_SYNC_BITS                                                                                           \
	(I2S_SYNCBUSY_ENABLE | I2S_SYNCBUSY_CKEN_Msk | I2S_SYNCBUSY_TXEN | I2S_SYNCBUSY_RXEN | I2S_SYNCBUSY_SWRST)

int32_t _i2s_sync_init(struct _i2s_sync_dev *dev, const void *hw, const enum _i2s_iface iface)
{
	uint8_t               hw_index;
	struct _i2s_init_cfg *cfg;
	uint8_t               iface_ctrl;
	uint8_t               ctrla;
	uint8_t               en_bits;
	const uint8_t         data_size[7] = {32, 24, 20, 18, 16, 16, 8};
	bool                  enabled;

	ASSERT(dev && hw);

	hw_index   = _i2s_get_index(hw);
	cfg        = (struct _i2s_init_cfg *)_i2s_get_cfg(hw_index);
	iface_ctrl = (iface == 0) ? 0 : ((cfg->serctrl[iface] & I2S_RXCTRL_CLKSEL) ? 1 : 0);
	en_bits    = cfg->ctrla & I2S_CTRLA_EN_MASK(iface, iface_ctrl);

	if (hri_i2s_is_syncing(hw, _I2S_CTRLA_SYNC_BITS)) {
		return ERR_DENIED;
	}
	ctrla = hri_i2s_read_CTRLA_reg(hw);

	/* Clock sharing case */
	switch (cfg->ctrla & (I2S_CTRLA_CKEN_Msk | I2S_CTRLA_TXEN_Pos | I2S_CTRLA_RXEN_Pos)) {
	case (I2S_CTRLA_CKEN0 | I2S_CTRLA_TXEN_Pos | I2S_CTRLA_RXEN_Pos):
	case (I2S_CTRLA_CKEN1 | I2S_CTRLA_TXEN_Pos | I2S_CTRLA_RXEN_Pos):
		if (ctrla & (I2S_CTRLA_TXEN << iface)) {
			return ERR_DENIED;
		}
		break;
	default:
		if (ctrla & en_bits) {
			return ERR_DENIED;
		}
		break;
	}

	if (!_i2s_iface_take(hw_index, iface)) {
		return ERR_ALREADY_INITIALIZED;
	}

	hri_i2s_wait_for_sync(hw, 0xFF);
	enabled = hri_i2s_get_CTRLA_ENABLE_bit(hw);
	if (enabled) {
		hri_i2s_clear_CTRLA_ENABLE_bit(hw);
		hri_i2s_wait_for_sync(hw, 0xFF);
	}

	dev->prvt       = (void *)hw;
	dev->iface      = iface;
	dev->iface_ctrl = iface_ctrl;

	dev->is_controller = !(cfg->ctrla & (I2S_CTRLA_TXEN << iface));
	dev->is_slave      = (cfg->clkctrl[iface_ctrl] & I2S_CLKCTRL_FSSEL_FSPIN);
	dev->word_size     = data_size[(cfg->serctrl[iface_ctrl] & I2S_TXCTRL_DATASIZE_Msk) >> I2S_TXCTRL_DATASIZE_Pos];

	(iface == 0) ? hri_i2s_write_TXCTRL_reg(hw, cfg->serctrl[iface])
	             : hri_i2s_write_RXCTRL_reg(hw, cfg->serctrl[iface]);
	hri_i2s_write_CLKCTRL_reg(hw, iface_ctrl, cfg->clkctrl[iface_ctrl]);

	if (enabled) {
		hri_i2s_write_CTRLA_reg(hw, ctrla);
	}

	return ERR_NONE;
}

void _i2s_sync_deinit(struct _i2s_sync_dev *dev)
{
	uint8_t hw_index;

	ASSERT(dev && dev->prvt);

	hw_index = _i2s_get_index(dev->prvt);

	_i2s_sync_disable(dev);
	_i2s_iface_free(hw_index, (enum _i2s_iface)dev->iface);
}

int32_t _i2s_sync_enable(struct _i2s_sync_dev *dev)
{
	I2s *   hw;
	uint8_t en_bits, ctrla;

	ASSERT(dev && dev->prvt);

	hw = (I2s *)dev->prvt;

	if (hri_i2s_is_syncing(hw, _I2S_CTRLA_SYNC_BITS)) {
		return ERR_DENIED;
	}

	if (dev->is_controller) {
		en_bits = (I2S_CTRLA_CKEN0) << dev->iface_ctrl;
	} else {
		en_bits = (I2S_CTRLA_CKEN0 << dev->iface_ctrl) | (I2S_CTRLA_TXEN << dev->iface);
	}

	ctrla = hri_i2s_read_CTRLA_reg(hw);

	if ((ctrla & en_bits) == en_bits) {
		return ERR_ALREADY_INITIALIZED;
	}

	ctrla |= en_bits | I2S_CTRLA_ENABLE;
	hri_i2s_write_CTRLA_reg(hw, ctrla);
	return ERR_NONE;
}

void _i2s_sync_disable(struct _i2s_sync_dev *dev)
{
	I2s *   hw;
	uint8_t en_bits, ctrla;

	ASSERT(dev && dev->prvt);

	hw = (I2s *)dev->prvt;

	while (hri_i2s_is_syncing(hw, _I2S_CTRLA_SYNC_BITS))
		;
	ctrla = hri_i2s_read_CTRLA_reg(hw);

	if (dev->is_controller) {
		en_bits = (I2S_CTRLA_CKEN0) << dev->iface_ctrl;
	} else {
		switch (ctrla & (I2S_CTRLA_CKEN_Msk | I2S_CTRLA_TXEN_Pos | I2S_CTRLA_RXEN_Pos)) {
		case I2S_CTRLA_CKEN0 | I2S_CTRLA_TXEN_Pos | I2S_CTRLA_RXEN_Pos:
		case I2S_CTRLA_CKEN1 | I2S_CTRLA_TXEN_Pos | I2S_CTRLA_RXEN_Pos:
			en_bits = (I2S_CTRLA_CKEN0 << dev->iface_ctrl);
			break;
		default:
			en_bits = (I2S_CTRLA_CKEN0 << dev->iface_ctrl) | (I2S_CTRLA_TXEN << dev->iface);
		}
	}

	if ((ctrla & en_bits) == 0) {
		return;
	}

	ctrla &= ~en_bits;
	if ((ctrla & (I2S_CTRLA_CKEN_Msk | I2S_CTRLA_TXEN_Pos | I2S_CTRLA_RXEN_Pos)) == 0) {
		ctrla &= ~I2S_CTRLA_ENABLE;
	}

	hri_i2s_write_CTRLA_reg(hw, ctrla);
}

/** FrameSync/WordSelect valid configurations. */
static const struct _i2s_ws_cfg_val {
	/** Supported number of bits. */
	uint16_t n_bits;
	/** Register flags for setup. */
	uint8_t reg_flags;
	/** For controller only. */
	uint8_t ctrl_only;
} _i2s_ws_cfg[] = {
    {8, I2S_CLKCTRL_FSWIDTH_SLOT | I2S_CLKCTRL_NBSLOTS(1) | I2S_CLKCTRL_SLOTSIZE_8, 0},
    {16, I2S_CLKCTRL_FSWIDTH_SLOT | I2S_CLKCTRL_NBSLOTS(1) | I2S_CLKCTRL_SLOTSIZE_16, 0},
    {24, I2S_CLKCTRL_FSWIDTH_SLOT | I2S_CLKCTRL_NBSLOTS(1) | I2S_CLKCTRL_SLOTSIZE_24, 0},
    {32, I2S_CLKCTRL_FSWIDTH_SLOT | I2S_CLKCTRL_NBSLOTS(1) | I2S_CLKCTRL_SLOTSIZE_32, 0},
    {48, I2S_CLKCTRL_FSWIDTH_HALF | I2S_CLKCTRL_NBSLOTS(3) | I2S_CLKCTRL_SLOTSIZE_24, 0},
    {64, I2S_CLKCTRL_FSWIDTH_HALF | I2S_CLKCTRL_NBSLOTS(3) | I2S_CLKCTRL_SLOTSIZE_32, 0},
    {72, I2S_CLKCTRL_FSWIDTH_HALF | I2S_CLKCTRL_NBSLOTS(5) | I2S_CLKCTRL_SLOTSIZE_24, 0},
    {96, I2S_CLKCTRL_FSWIDTH_HALF | I2S_CLKCTRL_NBSLOTS(5) | I2S_CLKCTRL_SLOTSIZE_32, 0},
    {128, I2S_CLKCTRL_FSWIDTH_HALF | I2S_CLKCTRL_NBSLOTS(7) | I2S_CLKCTRL_SLOTSIZE_32, 0},
};

int32_t _i2s_sync_set_ws_length(struct _i2s_sync_dev *dev, const uint16_t n_bits)
{
	I2s *    hw;
	uint32_t clkctrl;
	bool     enabled;
	uint8_t  i, nbslots;
	uint8_t  ctrla;

	ASSERT(dev && dev->prvt);

	hw = (I2s *)dev->prvt;

	if (dev->is_slave) {
		return ERR_UNSUPPORTED_OP;
	}

	for (i = 0; i < sizeof(_i2s_ws_cfg) / sizeof(struct _i2s_ws_cfg_val); i++) {
		if (n_bits == _i2s_ws_cfg[i].n_bits) {
			if (!dev->is_controller && _i2s_ws_cfg[i].ctrl_only) {
				continue;
			}
			break;
		}
	}
	if (i >= sizeof(_i2s_ws_cfg) / sizeof(struct _i2s_ws_cfg_val)) {
		return ERR_INVALID_ARG;
	}

	hri_i2s_wait_for_sync(hw, 0xFF);
	enabled = hri_i2s_get_CTRLA_ENABLE_bit(hw);
	if (enabled) {
		ctrla = hri_i2s_read_CTRLA_reg(hw);
		hri_i2s_clear_CTRLA_ENABLE_bit(hw);
		hri_i2s_wait_for_sync(hw, 0xFF);
	}

	clkctrl = hri_i2s_read_CLKCTRL_reg(hw, dev->iface_ctrl);
	clkctrl &= ~(I2S_CLKCTRL_FSWIDTH_Msk | I2S_CLKCTRL_NBSLOTS_Msk | I2S_CLKCTRL_SLOTSIZE_Msk);
	clkctrl |= _i2s_ws_cfg[i].reg_flags;

	if (!dev->is_controller) {
		/* Adjust enabled slots. */
		uint32_t serctrl = (dev->iface == 0) ? hri_i2s_read_TXCTRL_reg(hw) : hri_i2s_read_RXCTRL_reg(hw);
		nbslots          = ((_i2s_ws_cfg[i].reg_flags & I2S_CLKCTRL_NBSLOTS_Msk) >> I2S_CLKCTRL_NBSLOTS_Pos) + 1;
		serctrl |= I2S_TXCTRL_SLOTDIS_Msk;
		serctrl &= ~I2S_TXCTRL_SLOTDIS0;
		serctrl &= ~(I2S_TXCTRL_SLOTDIS0 << (nbslots >> 1));
		(dev->iface == 0) ? hri_i2s_write_TXCTRL_reg(hw, serctrl) : hri_i2s_write_RXCTRL_reg(hw, serctrl);
	}

	hri_i2s_write_CLKCTRL_reg(hw, dev->iface_ctrl, clkctrl);

	if (enabled) {
		hri_i2s_write_CTRLA_reg(hw, ctrla);
	}
	return ERR_NONE;
}

int32_t _i2s_sync_set_word_length(struct _i2s_sync_dev *dev, const uint8_t n_bits)
{
	I2s *   hw;
	uint8_t datasize;
	bool    enabled;
	uint8_t ctrla;

	ASSERT(dev && dev->prvt);

	hw = (I2s *)dev->prvt;

	if (dev->is_controller) {
		return ERR_UNSUPPORTED_OP;
	}

	switch (n_bits) {
	case 32:
		datasize = 0;
		break;
	case 24:
		datasize = 1;
		break;
	case 20:
		datasize = 2;
		break;
	case 18:
		datasize = 3;
		break;
	case 16:
		datasize = 4;
		break;
	case 8:
		datasize = 6;
		break;
	default:
		return ERR_INVALID_ARG;
	}

	hri_i2s_wait_for_sync(hw, 0xFF);
	enabled = hri_i2s_get_CTRLA_ENABLE_bit(hw);
	if (enabled) {
		ctrla = hri_i2s_read_CTRLA_reg(hw);
		hri_i2s_clear_CTRLA_ENABLE_bit(hw);
		hri_i2s_wait_for_sync(hw, 0xFF);
	}

	(dev->iface == 0) ? hri_i2s_write_TXCTRL_DATASIZE_bf(hw, datasize) : hri_i2s_write_RXCTRL_DATASIZE_bf(hw, datasize);

	if (enabled) {
		hri_i2s_write_CTRLA_reg(hw, ctrla);
	}
	return ERR_NONE;
}

int32_t _i2s_sync_set_sck_div(struct _i2s_sync_dev *dev, const uint32_t n_mck)
{
	I2s *   hw;
	bool    enabled;
	uint8_t ctrla;

	ASSERT(dev && dev->prvt);

	hw = (I2s *)dev->prvt;

	if (dev->is_slave) {
		return ERR_UNSUPPORTED_OP;
	}

	if (n_mck < 1 || n_mck > 64) {
		return ERR_INVALID_ARG;
	}

	hri_i2s_wait_for_sync(hw, 0xFF);
	enabled = hri_i2s_get_CTRLA_ENABLE_bit(hw);
	if (enabled) {
		ctrla = hri_i2s_read_CTRLA_reg(hw);
		hri_i2s_clear_CTRLA_ENABLE_bit(hw);
		hri_i2s_wait_for_sync(hw, 0xFF);
	}

	hri_i2s_write_CLKCTRL_MCKDIV_bf(hw, dev->iface_ctrl, n_mck - 1);

	if (enabled) {
		hri_i2s_write_CTRLA_reg(hw, ctrla);
	}
	return ERR_NONE;
}

#ifdef __cplusplus
}
#endif

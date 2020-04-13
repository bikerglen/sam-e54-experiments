/**
 * \file
 *
 * \brief SAM I2S Controller HAL
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

#include "hal_i2s_controller_sync.h"
#include "utils_assert.h"

int32_t i2s_c_sync_init(struct i2s_c_sync_desc *i2s, const void *hw, const enum i2s_c_iface iface)
{
	int32_t rc;

	ASSERT(i2s && hw);

	/** OUT/IN interface (corresponding to interface 0 and 1)
	 *  will be used in HPL.
	 */
	rc = _i2s_sync_init(&i2s->dev, hw, (enum _i2s_iface)iface);
	if (rc < 0) {
		return rc;
	}

	return ERR_NONE;
}

void i2s_c_sync_deinit(struct i2s_c_sync_desc *i2s)
{
	ASSERT(i2s);

	_i2s_sync_deinit(&i2s->dev);
}

int32_t i2s_c_sync_enable(struct i2s_c_sync_desc *i2s)
{
	ASSERT(i2s);

	return _i2s_sync_enable(&i2s->dev);
}

void i2s_c_sync_disable(struct i2s_c_sync_desc *i2s)
{
	ASSERT(i2s);

	_i2s_sync_disable(&i2s->dev);
}

int32_t i2s_c_sync_set_ws_length(struct i2s_c_sync_desc *i2s, const uint16_t n_sck)
{
	ASSERT(i2s);

	return _i2s_sync_set_ws_length(&i2s->dev, n_sck);
}

int32_t i2s_c_sync_set_sck_div(struct i2s_c_sync_desc *i2s, const uint32_t n_mck)
{
	ASSERT(i2s);

	return _i2s_sync_set_sck_div(&i2s->dev, n_mck);
}

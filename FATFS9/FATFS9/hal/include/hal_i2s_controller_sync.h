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

#ifndef _HAL_I2S_CONTROLLER_H_INCLUDED
#define _HAL_I2S_CONTROLLER_H_INCLUDED

#include "hpl_i2s.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup doc_driver_hal_i2s_controller_sync
 *
 *@{
 */

/** I2S Controller Interface. */
enum i2s_c_iface {
	/** I2S controller interface 0. */
	I2S_C_IFACE_0,
	/** I2S controller interface 1. */
	I2S_C_IFACE_1
};

/**
 * I2S Controller Descriptor struct
 */
struct i2s_c_sync_desc {
	/** HPL device instance for I2S Controller. */
	struct _i2s_sync_dev dev;
};

/**
 * \brief Initialize the I2S Controller
 *
 * \param[in, out] i2s Pointer to the I2S Controller instance.
 * \param[in] hw Pointer to the hardware base.
 * \param[in] iface The I2S interface used.
 *
 * \return Operation status.
 * \retval 0 Success.
 * \retval <0 Error.
 */
int32_t i2s_c_sync_init(struct i2s_c_sync_desc *i2s, const void *hw, const enum i2s_c_iface iface);

/**
 * \brief Deinitialize the I2S Controller
 *
 * \param[in, out] i2s Pointer to the I2S Controller instance.
 */
void i2s_c_sync_deinit(struct i2s_c_sync_desc *i2s);

/**
 * \brief Enable the I2S Controller
 *
 * \param[in, out] i2s Pointer to the I2S Controller instance.
 *
 * \return Operation status.
 * \retval 0 Success.
 * \retval <0 Error.
 */
int32_t i2s_c_sync_enable(struct i2s_c_sync_desc *i2s);

/**
 * \brief Disable the I2S Controller
 *
 * \param[in, out] i2s Pointer to the I2S Controller instance.
 */
void i2s_c_sync_disable(struct i2s_c_sync_desc *i2s);

/**
 * \brief Set the Word Select pulse Length of the I2S Controller
 *
 * Note that it works only when the I2S Controller is disabled.
 *
 * \param[in, out] i2s Pointer to the I2S Controller instance.
 * \param[in] n_sck Describes how many SCK bits generates a Word Select pulse.
 *
 * \return Operation status.
 * \retval 0 Success.
 * \retval <0 Error.
 */
int32_t i2s_c_sync_set_ws_length(struct i2s_c_sync_desc *i2s, const uint16_t n_sck);

/**
 * \brief Set the SCK division from MCK
 *
 * Note that it works only when the I2S Controller is disabled.
 *
 * \param[in, out] i2s Pointer to the I2S Controller instance.
 * \param[in] n_mck Describes how many MCK bits generates a SCK clock.
 *
 * \return Operation status.
 * \retval 0 Success.
 * \retval <0 Error.
 */
int32_t i2s_c_sync_set_sck_div(struct i2s_c_sync_desc *i2s, const uint32_t n_mck);

/** \brief Retrieve the current driver version
 *
 *  \return Current driver version.
 */
uint32_t i2s_c_sync_get_version(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* _HAL_I2S_CONTROLLER_H_INCLUDED */

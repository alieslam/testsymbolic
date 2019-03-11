/*
    Copyright 2016-2018 (C) Alexey Dynda

    This file is part of Tiny Protocol Library.

    Protocol Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Protocol Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Protocol Library.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 This is Tiny protocol implementation for microcontrollers

 @file
 @brief Tiny protocol Types
*/
#ifndef _TINY_PROTO_TYPES_H_
#define _TINY_PROTO_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "os/tiny_defines.h"

/// \cond
#ifdef CONFIG_ENABLE_FCS32
    typedef uint32_t fcs_t;
#else
    typedef uint16_t fcs_t;
#endif
/// \endcond

/**
 * @defgroup ERROR_FLAGS Return error codes for Tiny API functions
 * @{
 */
/// Tiny operation successful. Only tiny_send_start and tiny_read_start functions return this code
#define TINY_SUCCESS                    (1)
/// No error. For tiny_send and tiny_read functions, this means, no data sent or received
#define TINY_NO_ERROR                   (0)
/// Timeout
#define TINY_ERR_FAILED                 (-1)
/// Timeout happened. The function must be called once again.
#define TINY_ERR_TIMEOUT                (-2)
/// Data too large to fit the user buffer, valid for tiny_read function
#define TINY_ERR_DATA_TOO_LARGE         (-3)
/// Some invalid data passed to Tiny API function.
#define TINY_ERR_INVALID_DATA           (-4)
/// API function detected that operation cannot be performed right now.
#define TINY_ERR_BUSY                   (-5)
/// Out of sync - received some data, which are not part of the frame (tiny_read)
#define TINY_ERR_OUT_OF_SYNC            (-6)
/// No data for now, need to retry reading once again
#define TINY_ERR_AGAIN                  (-7)

/** @} */

/**
 * @defgroup FLAGS_GROUP Flags for Tiny API functions
 * @{
 */

/// This flag makes tiny API functions perform as non-blocking
#define TINY_FLAG_NO_WAIT               (0)
/// This flag makes tiny_read function to read whole frame event if it doesn't fit the buffer
#define TINY_FLAG_READ_ALL              (1)
/// Informs advanced API that caller wants to start transmit new frame to the channel
#define TINY_FLAG_LOCK_SEND             (2)
/// This flag makes tiny API functions perform in blocking mode
#define TINY_FLAG_WAIT_FOREVER          (0x80)

/** @} */

/**
 * This structure contains captured statistics while the protocol
 * sends and receives messages.
 */
typedef struct
{
    /// Number of bytes received out of frame bytes
    uint32_t            oosyncBytes;
    /// Number of payload bytes totally sent through the channel
    uint32_t            bytesSent;
    /// Number of payload bytes totally received through the channel
    uint32_t            bytesReceived;
    /// Number of frames, successfully sent through the channel
    uint32_t            framesSent;
    /// Number of frames, successfully received through the communication channel
    uint32_t            framesReceived;
    /// Number of broken frames received
    uint32_t            framesBroken;
} STinyStats;

/**
 * The function writes data to communication channel port.
 * @param pdata - pointer to user private data - absent in Arduino version
 * @param buffer - pointer to the data to send to channel.
 * @param size - size of data to write.
 * @see read_block_cb_t
 * @return the function must return negative value in case of error or number of bytes written
 *         or zero.
 */
typedef int (*write_block_cb_t)(void *pdata, const uint8_t *buffer, int size);

/**
 * The function reads data from communication channel.
 * @param pdata - pointer to user private data. - absent in Arduino version
 * @param buffer - pointer to a buffer to read data to from the channel.
 * @param size - maximum size of the buffer.
 * @see write_block_cb_t
 * @return the function must return negative value in case of error or number of bytes actually read
 *         or zero.
 */
typedef int (*read_block_cb_t)(void *pdata, uint8_t *buffer, int size);


/**
 * on_frame_cb_t is a callback function, which is called every time new frame is received, or sent.
 * refer to tiny_set_callbacks
 * @param handle - handle of Tiny.
 * @param uid    - UID of the received frame or sent frame (if uids are enabled).
 * @param pdata  - data received over Tiny Protocol.
 * @param size   - size of data received.
 * @return None.
 * @see   tiny_set_callbacks
 */
typedef void (*on_frame_cb_t)(void *handle, uint16_t uid, uint8_t *pdata, int size);

#ifdef __cplusplus
}
#endif

#endif

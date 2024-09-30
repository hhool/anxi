/**
 * @file crc16.h
 * @author hhool (hhool@outlook.com)
 * @brief  crc16 checksum calculation function
 * @version 0.1
 * @date 2024-07-08
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_COMMON_CRC16_H_
#define APP_COMMON_CRC16_H_

#include <stdint.h>

namespace anx {
namespace common {
/**
 * @brief crc16 checksum calculation function
 *
 * @param data data to be calculated
 * @param length data length
 * @return uint16_t crc16 checksum
 */

uint16_t crc16(const uint8_t* data, uint32_t length);

}  // namespace common
}  // namespace anx

#endif  // APP_COMMON_CRC16_H_

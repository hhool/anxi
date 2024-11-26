/**
 * @file crc16_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief crc16 checksum calculation function unit test
 * @version 0.1
 * @date 2024-11-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#define imsleep(microsecond) Sleep(microsecond)  // ms
#else
#include <unistd.h>
#define imsleep(microsecond) usleep(1000 * microsecond)  // ms
#endif

#ifndef _WIN32
#include <thread>  // std::thread
#endif

#include "gtest/gtest.h"

#include "app/common/crc16.h"

class CRC16Test : public testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(CRC16Test, CheckCRC16) {
  // 05H功能码，强制写线圈，01H功能地址线圈
  ///////////////////////////////////////////////////////////////////////
  // 线圈地址0x02 音波启动
  // broadcast machin on
  {
    std::vector<uint8_t> data = {0x00, 0x05, 0x00, 0x02, 0xFF, 0x00};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x2B2C);
  }
  // broadcast machin off
  {
    std::vector<uint8_t> data = {0x00, 0x05, 0x00, 0x02, 0x00, 0x00};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0xDB6D);
  }
  // peer machin on
  {
    std::vector<uint8_t> data = {0x01, 0x05, 0x00, 0x02, 0xFF, 0x00};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0xFA2D);
  }
  // peer machin off
  {
    std::vector<uint8_t> data = {0x01, 0x05, 0x00, 0x02, 0x00, 0x00};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x0A6C);
  }
  // 线圈地址0x00， 报警复位
  {
    std::vector<uint8_t> data = {0x01, 0x05, 0x00, 0x00, 0x00, 0x00};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0xCACD);
  }
  {
    std::vector<uint8_t> data = {0x01, 0x05, 0x00, 0x00, 0xFF, 0x00};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x3A8C);
  }
  // 线圈地址0x01， 频率复位
  {
    std::vector<uint8_t> data = {0x01, 0x05, 0x00, 0x01, 0x00, 0x00};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x0A9C);
  }
  // 线圈地址0x03， 开机自动扫描
  {
    std::vector<uint8_t> data = {0x01, 0x05, 0x00, 0x03, 0xFF, 0x00};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x3A7C);
  }
  ///////////////////////////////////////////////////////////////////////

  // 04H功能码，读取数据
  ///////////////////////////////////////////////////////////////////////
  // get current freqence
  {
    std::vector<uint8_t> data = {0x01, 0x04, 0x00, 0x01, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x0A60);
    /// send: 01 04 00 01 00 01 60 0A
    /// response: 01 04 02 4D 97 CD CE
    /// value is 0x4D97 = 19863
  }
  // get current power
  {
    std::vector<uint8_t> data = {0x01, 0x04, 0x00, 0x00, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0xCA31);
    /// send: 01 04 00 00 00 01 31 CA
    /// response: 01 04 02 00 2A 38 EF
    /// value is 0x002A = 42
  }
  // get current fault code
  {
    std::vector<uint8_t> data = {0x01, 0x04, 0x00, 0x02, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x0A90);
    /// send: 01 04 00 02 00 01 90 0A
    /// response: 01 04 02 00 00 B9 30
    /// value is 0x0000 means no fault
  }
  // get current state
  {
    std::vector<uint8_t> data = {0x01, 0x04, 0x00, 0x03, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0xCAC1);
    /// send: 01 04 00 03 00 01 C1 CA
    /// response: 01 04 02 00 03 F9 31
    /// value is 0x0003 means on
  }
  // get current version
  {
    std::vector<uint8_t> data = {0x01, 0x04, 0x00, 0x04, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x0B70);
    /// send: 01 04 00 04 00 01 70 0B
    /// response: 01 84 02 C2 C1
    /// value: unknown
  }
  // set amplitude 20%
  {
    std::vector<uint8_t> data = {0x01, 0x06, 0x00, 0x18, 0x00, 0x14};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0xC209);
    /// send: 01 06 00 18 00 14 09 C2
    /// response: 01 06 00 18 00 14 09 C2
  }
  // set amplitude 60%
  {
    std::vector<uint8_t> data = {0x01, 0x06, 0x00, 0x18, 0x00, 0x3C};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0xDC09);
  }
  // set amplitude of welding 80%
  {
    std::vector<uint8_t> data = {0x01, 0x06, 0x00, 0x18, 0x00, 0x50};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0xF109);
  }
  // set amplitude of welding 100%
  {
    std::vector<uint8_t> data = {0x01, 0x06, 0x00, 0x18, 0x00, 0x64};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x2608);
  }
  // set time of welding
  {
    std::vector<uint8_t> data = {0x01, 0x06, 0x00, 0x19, 0x00, 0xC8};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x9B59);
    /// send: 01 06 00 19 00 C8 59 9B
    /// response: 01 06 00 19 00 C8 59 9B
    /// time is 0xC8 = 200
  }
  {
    // 600 unit 0x02, 0x58
    std::vector<uint8_t> data = {0x01, 0x06, 0x00, 0x19, 0x02, 0x58};
    // 01 06 00 19 02 58 58 97
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    uint8_t high = (crc & 0xFF00) >> 8;
    uint8_t low = crc & 0xFF;
    std::vector<uint8_t> result = {high, low};
    EXPECT_EQ(crc, 0x9758);
    /// send: 01 06 00 19 02 58 58 97
    /// response: 01 06 00 19 02 58 58 97
    /// time is 0x0258 = 600
  }
  {
    // 400 unit 0x02, 0x58
    std::vector<uint8_t> data = {0x01, 0x06, 0x00, 0x19, 0x01, 0x90};
    // 01 06 00 19 01 90 59 f1
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    uint8_t high = (crc & 0xFF00) >> 8;
    uint8_t low = crc & 0xFF;
    std::vector<uint8_t> result = {high, low};
    EXPECT_EQ(crc, 0xf159);
    /// send: 01 06 00 19 01 90 59 f1
    /// response: 01 06 00 19 01 90 59 f1
    /// time is 0x0190 = 400
  }
  // set amplitude and time of welding
  {
    std::vector<uint8_t> data = {0x01, 0x10, 0x00, 0x18, 0x00, 0x02,
                                 0x04, 0x00, 0x3C, 0x01, 0x2C};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x4433);
  }
  // set highest freq 20200HZ
  {
    std::vector<uint8_t> data = {0x01, 0x06, 0x00, 0x03, 0x4E, 0xE8};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x244C);
  }
  // set lowest freq 19200HZ
  {
    std::vector<uint8_t> data = {0x01, 0x06, 0x00, 0x04, 0x4B, 0x00};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0xFBFE);
  }
  // set highest and lowest freq 20200HZ 19200HZ
  {
    std::vector<uint8_t> data = {0x01, 0x10, 0x00, 0x03, 0x00, 0x02,
                                 0x04, 0x4E, 0xE8, 0x4B, 0x00};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x5612);
  }
  // set highest and lowest freq 36000HZ 34000HZ
  {
    std::vector<uint8_t> data = {0x01, 0x10, 0x00, 0x3,  0x00, 0x02,
                                 0x04, 0x8C, 0xA0, 0x84, 0xD0};
    uint16_t crc = anx::common::crc16(data.data(), data.size());
    EXPECT_EQ(crc, 0x94FB);
  }
  //
  {
    std::vector<uint8_t> data_3 = {0x01, 0x84, 0x02};
    uint16_t crc = anx::common::crc16(data_3.data(), data_3.size());
    EXPECT_EQ(crc, 0xC1C2);
  }
  // guess get freq at machine on settings
  // 03H
  {
    // 00 读取开机起始频率
    std::vector<uint8_t> data_3 = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data_3.data(), data_3.size());
    EXPECT_EQ(crc, 0x0a84);
    /// send: 01 03 00 00 00 01 84 0A
    /// response: 01 03 02 4D 62 0C FD
    /// value: 0x4D62 = 19810
  }
  {
    // 01 软起动时间
    std::vector<uint8_t> data_3 = {0x01, 0x03, 0x00, 0x01, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data_3.data(), data_3.size());
    EXPECT_EQ(crc, 0xcad5);
    /// send: 01 03 00 01 00 01 D5 CA
    /// response: 01 03 02 00 64 B9 AF
    /// value: 0x0064 = 100
  }
  {
    // 02 功率上限
    std::vector<uint8_t> data_3 = {0x01, 0x03, 0x00, 0x02, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data_3.data(), data_3.size());
    EXPECT_EQ(crc, 0xca25);
    /// send: 01 03 00 02 00 01 25 CA
    /// response:  01 03 02 05 DC BA 8D
    /// value: 0x05DC = 1500
  }
  {
    // 03 频率最大值
    std::vector<uint8_t> data_3 = {0x01, 0x03, 0x00, 0x03, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data_3.data(), data_3.size());
    EXPECT_EQ(crc, 0x0a74);
    /// send: 01 03 00 03 00 01 74 0A
    /// response: 01 03 02 50 14 84 4B
    /// value: 0x5014 = 20404
  }
  {
    // 04 频率最小值
    std::vector<uint8_t> data_3 = {0x01, 0x03, 0x00, 0x04, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data_3.data(), data_3.size());
    EXPECT_EQ(crc, 0xcbc5);
    /// send: 01 03 00 04 00 01 C5 CB
    /// response: 01 03 02 4B 00 8E B4
    /// value: 0x4B00 = 19200
  }
  {
    // 05 强制扫描参数
    std::vector<uint8_t> data_3 = {0x01, 0x03, 0x00, 0x05, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data_3.data(), data_3.size());
    EXPECT_EQ(crc, 0x0b94);
    /// send: 01 03 00 05 00 01 94 0B
    /// response: 01 03 02 00 32 39 91
    /// value: 0x0032 = 50
  }
  {
    // 06 强制扫描次数
    std::vector<uint8_t> data_3 = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data_3.data(), data_3.size());
    EXPECT_EQ(crc, 0x0b64);
    /// send: 01 03 00 06 00 01 64 0B
    /// response: 01 03 02 00 05 78 47
    /// value: 0x0005 = 5
  }
  {
    // 07 强制扫描振幅
    std::vector<uint8_t> data_3 = {0x01, 0x03, 0x00, 0x07, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data_3.data(), data_3.size());
    EXPECT_EQ(crc, 0xcb35);
    /// send: 01 03 00 07 00 01 35 CB
    /// response: 01 03 02 00 3C B8 55
    /// value: 0x003C = 60
  }

  // guess get amplitude settings
  {
    std::vector<uint8_t> data_3 = {0x01, 0x03, 0x00, 0x18, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data_3.data(), data_3.size());
    EXPECT_EQ(crc, 0x0d04);
    /// send: 01 03 00 18 00 01 04 0D
    /// response: 01 03 02 00 64 B9 AF
    /// value: 0x0064 = 100
  }
  // guess get time settings
  {
    std::vector<uint8_t> data_3 = {0x01, 0x03, 0x00, 0x19, 0x00, 0x01};
    uint16_t crc = anx::common::crc16(data_3.data(), data_3.size());
    EXPECT_EQ(crc, 0xcd55);
    /// send: 01 03 00 19 00 01 55 CD
    /// response: 01 03 02 00 64 B9 AF
    /// value: 0x0064 = 100
  }
  {
    // cmd is error from remote response
    std::vector<uint8_t> data_3 = {0x01, 0x83, 0x03};
    uint16_t crc = anx::common::crc16(data_3.data(), data_3.size());
    EXPECT_EQ(crc, 0x3101);
    /// send: 01 83 03 01 31
    /// response: 01 83 01 80 F0
    /// value:
  }
}
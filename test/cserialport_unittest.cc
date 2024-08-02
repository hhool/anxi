/**
 * @file cserialport_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief just a test file
 * @version 0.1
 * @date 2024-07-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include <CSerialPort/SerialPort.h>
#include <CSerialPort/SerialPortInfo.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace itas109;

static char portName1[256];
static char portName2[256];

class CSerialPortTests : public testing::Test {
 protected:
  virtual void SetUp() {
    m_availablePortsInfoVector = CSerialPortInfo::availablePortInfos();

    ASSERT_TRUE(m_availablePortsInfoVector.size() > 0) << "No valid port";
  }

  virtual void TearDown() {
    if (m_serialport.isOpen()) {
      m_serialport.close();

#ifdef _WIN32
      Sleep(100);
#else
      usleep(1000 * 100);
#endif
    }

    if (m_serialport2.isOpen()) {
      m_serialport2.close();

#ifdef _WIN32
      Sleep(100);
#else
      usleep(1000 * 100);
#endif
    }
  };
  CSerialPort m_serialport;
  CSerialPort m_serialport2;
  std::vector<SerialPortInfo> m_availablePortsInfoVector;
};

TEST_F(CSerialPortTests, isContainsVirtualPair_0_1) {
  std::cout << "Serial Port available Count : "
            << m_availablePortsInfoVector.size() << std::endl;
  int virtualSerialPortCount = 0;
  for (size_t i = 0; i < m_availablePortsInfoVector.size(); i++) {
    if (0 == strcmp(m_availablePortsInfoVector[i].portName, portName1) ||
        0 == strcmp(m_availablePortsInfoVector[i].portName, portName2)) {
      ++virtualSerialPortCount;
    }
  }

  EXPECT_EQ(virtualSerialPortCount, 2);
}

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

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <CSerialPort/SerialPort.h>
#include <CSerialPort/SerialPortInfo.h>

using namespace itas109;  // NOLINT

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
  }
  CSerialPort m_serialport;
  CSerialPort m_serialport2;
  std::vector<SerialPortInfo> m_availablePortsInfoVector;
};

namespace {
std::string char2hexstr(const char* data, int len) {
  std::string str;
  for (int i = 0; i < len; i++) {
    char buf[3] = {0};
    sprintf(buf, "%02X", (unsigned char)data[i]);
    str += buf;
  }
  return str;
}
void sleep_ms(int64_t ms) {
#ifdef _WIN32
  Sleep(ms);
#else
  usleep(1000 * ms);
#endif
}
}  // namespace

class MyListener : public CSerialPortListener {
 public:
  explicit MyListener(CSerialPort* sp) : p_sp(sp) {}

  void onReadEvent(const char* portName, unsigned int readBufferLen) {
    if (readBufferLen > 0) {
      char* data = new char[readBufferLen + 1];  // '\0'

      if (data) {
        // read
        int recLen = p_sp->readData(data, readBufferLen);

        if (recLen > 0) {
          data[recLen] = '\0';
          printf("%s - Count: %d, Length: %d, Str: %s, Hex: %s\n", portName,
                 ++countRead, recLen, data, char2hexstr(data, recLen).c_str());

          // return receive data
          p_sp->writeData(data, recLen);
        }

        delete[] data;
      }
    }
  }

 private:
  CSerialPort* p_sp;
  int countRead = 0;
};

TEST_F(CSerialPortTests, enumSerialPort) {
  std::cout << "Serial Port available Count : "
            << m_availablePortsInfoVector.size() << std::endl;
  for (size_t i = 0; i < m_availablePortsInfoVector.size(); i++) {
    std::cout << "Port Name : " << m_availablePortsInfoVector[i].portName
              << std::endl;
  }
}

TEST_F(CSerialPortTests, writeAndReadData) {
  strncpy(portName2, "COM3", 4);
  m_serialport.init(portName2, itas109::BaudRate115200, itas109::ParityNone,
                    itas109::DataBits8, itas109::StopOne,
                    itas109::FlowHardware);
  m_serialport.setOperateMode(itas109::SynchronousOperate);
  m_serialport.setReadIntervalTimeout(0);  // read interval timeout 0ms
  m_serialport.setByteReadBufferFullNotify(4096 * 0.8);  // buffer full notify
  ASSERT_TRUE(m_serialport.open());

  MyListener listener(&m_serialport);
  m_serialport.connectReadEvent(&listener);

  // write hex data
  char hex[8];
  hex[0] = 0x01;
  hex[1] = 0x04;
  hex[2] = 0x00;
  hex[3] = 0x01;
  hex[4] = 0x00;
  hex[5] = 0x01;
  hex[6] = 0x60;
  hex[7] = 0x0A;
  m_serialport.writeData(hex, sizeof(hex));
  sleep_ms(1000);
  char data_result[16] = {0};
  int result = 0;
  int try_count = 0;
  while (result == 0 && try_count < 10) {
    result = m_serialport.readData(data_result, 16);
    if (result > 0) {
      break;
    }
    try_count++;
    sleep_ms(100);
  }
  sleep_ms(5000);
}

TEST_F(CSerialPortTests, openSerialPort) {
  strcpy(portName1, m_availablePortsInfoVector[0].portName);
  strcpy(portName2, m_availablePortsInfoVector[1].portName);

  m_serialport.init(portName2, 9600);
  ASSERT_TRUE(m_serialport.open());
}

TEST_F(CSerialPortTests, openSerialPort2) {
  m_serialport2.init(portName1, 9600);
  ASSERT_TRUE(m_serialport2.open());
}

TEST_F(CSerialPortTests, writeData) {
  m_serialport.init(portName2, 9600);
  ASSERT_TRUE(m_serialport.open());

  std::string str = "Hello World!";
  int len = m_serialport.writeData(str.c_str(), str.length());
  ASSERT_EQ(len, str.length());
}

TEST_F(CSerialPortTests, writeData2) {
  m_serialport2.init(portName1, 9600);
  ASSERT_TRUE(m_serialport2.open());

  std::string str = "Hello World!";
  int len = m_serialport2.writeData(str.c_str(), str.length());
  ASSERT_EQ(len, str.length());
}

TEST_F(CSerialPortTests, readData) {
  m_serialport.init(portName2, 9600);
  ASSERT_TRUE(m_serialport.open());

  MyListener listener(&m_serialport);
  m_serialport.connectReadEvent(&listener);

  std::string str = "Hello World!";
  int len = m_serialport.writeData(str.c_str(), str.length());
  ASSERT_EQ(len, str.length());

#ifdef _WIN32
  Sleep(5000);
#else
  usleep(1000 * 1000 * 5);
#endif
}

TEST_F(CSerialPortTests, readData2) {
  m_serialport2.init(portName1, 9600);
  ASSERT_TRUE(m_serialport2.open());

  MyListener listener(&m_serialport2);
  m_serialport2.connectReadEvent(&listener);

  std::string str = "Hello World!";
  int len = m_serialport2.writeData(str.c_str(), str.length());
  ASSERT_EQ(len, str.length());

#ifdef _WIN32
  Sleep(5000);
#else
  usleep(1000 * 1000 * 5);
#endif
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

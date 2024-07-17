/**
 * @file appinterface.h
 * @author hhool (hhool@outlook.com)
 * @brief appinterface class definition
 * @version 0.1
 * @date 2024-07-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef ANXI_APP_APPINTERFACE_H_
#define ANXI_APP_APPINTERFACE_H_

#include <memory>

namespace anx {
namespace app {
class AppInterface {
 public:

 public:
  virtual ~AppInterface() {}

  virtual int32_t Run() = 0;
  virtual void Exit() = 0;

 private:
};

void* CreateApp();
int32_t Run(void*);
void Exit(void*);
void DestroyApp(void*);
}  // namespace app
}  // namespace anx

#endif  // ANXI_APP_APPINTERFACE_H_

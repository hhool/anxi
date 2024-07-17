/**
 * @file application.h
 * @author hhool (hhool@outlook.com)
 * @brief application class definition
 * @version 0.1
 * @date 2024-07-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef ANXI_APP_APPLICATION_H_
#define ANXI_APP_APPLICATION_H_

#include <memory>
#include <string>
#include <vector>

#include "app/appinterface.h"

namespace anx {
namespace ui {
class MainWindow;
}
namespace app {
class AppInterface;
class Application : public AppInterface {
 public:
  Application();
  ~Application();

  int32_t Run() override;
  void Exit() override;

 private:
	 std::unique_ptr<anx::ui::MainWindow> main_window_;
};
}  // namespace app
}  // namespace anx

#endif  // ANXI_APP_APPLICATION_H_

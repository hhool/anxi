/**
 * @file work_window_tab_main_first_page_solution_design.h
 * @author hhool (hhool@outlook.com)
 * @brief work window for page solution design helper
 * used to handle the solution design page of the work window
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_TAB_MAIN_FIRST_PAGE_SOLUTION_DESIGN_H_
#define APP_UI_WORK_WINDOW_TAB_MAIN_FIRST_PAGE_SOLUTION_DESIGN_H_

#include "app/ui/work_window_tab_main_first_page_solution_design_base.h"

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;  // NOLINT

namespace anx {
namespace esolution {
class SolutionDesign;
struct ExpDesignHeader;
struct ExpDesignResult;
}  // namespace esolution
namespace ui {
class WorkWindow;
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {

class WorkWindowFirstPageAxiallySymmetrical : public DuiLib::CNotifyPump,
                                              public DuiLib::INotifyUI,
                                              public PageSolutionDesignBase {
 public:
  WorkWindowFirstPageAxiallySymmetrical(
      WorkWindow* pOwner,
      DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowFirstPageAxiallySymmetrical();

 public:
  void Notify(TNotifyUI& msg) override;

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT
  void OnValueChanged(TNotifyUI& msg);  // NOLINT

 public:
  std::unique_ptr<anx::esolution::SolutionDesign> SolutionDesignFromPage()
      override;

 protected:
  void InitPage() override;
  /// @brief Update the main tab page of work window with
  /// solution_design_.result_
  void UpadateTabMainFirstPageElementViewResult() override;

  std::unique_ptr<anx::esolution::ExpDesignResult> ExpDesignResultFromControl()
      override;

 private:
  WorkWindow* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  std::string t_prefix_;
  bool exp_running_ = false;
};

class WorkWindownFirstPageStressAjustable : public DuiLib::CNotifyPump,
                                            public DuiLib::INotifyUI,
                                            public PageSolutionDesignBase {
 public:
  WorkWindownFirstPageStressAjustable(
      WorkWindow* pOwner,
      DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindownFirstPageStressAjustable();

 public:
  void Notify(TNotifyUI& msg) override;

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT
  void OnValueChanged(TNotifyUI& msg);  // NOLINT

 public:
  std::unique_ptr<anx::esolution::SolutionDesign> SolutionDesignFromPage()
      override;

 protected:
  void InitPage() override;
  /// @brief Update the main tab page of work window with
  /// solution_design_.result_
  void UpadateTabMainFirstPageElementViewResult() override;

  std::unique_ptr<anx::esolution::ExpDesignResult> ExpDesignResultFromControl()
      override;

 private:
  WorkWindow* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  std::string t_prefix_;
  bool exp_running_ = false;
};

class WorkWindowFirstPageTh3pointBending : public DuiLib::CNotifyPump,
                                           public DuiLib::INotifyUI,
                                           public PageSolutionDesignBase {
 public:
  WorkWindowFirstPageTh3pointBending(WorkWindow* pOwner,
                                     DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowFirstPageTh3pointBending() override;

 public:
  void Notify(TNotifyUI& msg) override;

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT
  void OnValueChanged(TNotifyUI& msg);  // NOLINT

 public:
  std::unique_ptr<anx::esolution::SolutionDesign> SolutionDesignFromPage()
      override;

 protected:
  void InitPage() override;
  /// @brief Update the main tab page of work window with
  /// solution_design_.result_
  void UpadateTabMainFirstPageElementViewResult() override;

  std::unique_ptr<anx::esolution::ExpDesignResult> ExpDesignResultFromControl()
      override;

 private:
  WorkWindow* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  std::string t_prefix_;
  bool exp_running_ = false;
};

class WorkWindowFirstPageVibrationBending : public DuiLib::CNotifyPump,
                                            public DuiLib::INotifyUI,
                                            public PageSolutionDesignBase {
 public:
  WorkWindowFirstPageVibrationBending(
      WorkWindow* pOwner,
      DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowFirstPageVibrationBending();

 public:
  void Notify(TNotifyUI& msg) override;
  void OnValueChanged(TNotifyUI& msg);  // NOLINT

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT

 public:
  std::unique_ptr<anx::esolution::SolutionDesign> SolutionDesignFromPage()
      override;

 protected:
  void InitPage() override;
  /// @brief Update the main tab page of work window with
  /// solution_design_.result_
  void UpadateTabMainFirstPageElementViewResult() override;

  std::unique_ptr<anx::esolution::ExpDesignResult> ExpDesignResultFromControl()
      override;

 private:
  WorkWindow* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  std::string t_prefix_;
  bool exp_running_ = false;
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_FIRST_PAGE_SOLUTION_DESIGN_H_

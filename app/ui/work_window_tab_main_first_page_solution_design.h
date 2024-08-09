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
                                              public PageSolutionDesignBase {
 public:
  explicit WorkWindowFirstPageAxiallySymmetrical(
      WorkWindow* pOwner,
      DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowFirstPageAxiallySymmetrical();

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT

 public:
  std::unique_ptr<anx::esolution::SolutionDesign> SolutionDesignFromPage()
      override;

 protected:
  /// @brief Update the main tab page of work window with
  /// solution_design_.result_
  void UpadateTabMainFirstPageElementViewResult() override;

  std::unique_ptr<anx::esolution::ExpDesignResult> ExpDesignResultFromControl()
      override;

 private:
  WorkWindow* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
};

class WorkWindownFirstPageStressAjustable : public DuiLib::CNotifyPump,
                                            public PageSolutionDesignBase {
 public:
  explicit WorkWindownFirstPageStressAjustable(
      WorkWindow* pOwner,
      DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindownFirstPageStressAjustable();

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT

 public:
  std::unique_ptr<anx::esolution::SolutionDesign> SolutionDesignFromPage()
      override;

 protected:
  /// @brief Update the main tab page of work window with
  /// solution_design_.result_
  void UpadateTabMainFirstPageElementViewResult() override;

  std::unique_ptr<anx::esolution::ExpDesignResult> ExpDesignResultFromControl()
      override;

 private:
  WorkWindow* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
};

class WorkWindowFirstPageTh3pointBending : public DuiLib::CNotifyPump,
                                           public PageSolutionDesignBase {
 public:
  explicit WorkWindowFirstPageTh3pointBending(
      WorkWindow* pOwner,
      DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowFirstPageTh3pointBending();

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg); // NOLINT

 public:
  std::unique_ptr<anx::esolution::SolutionDesign> SolutionDesignFromPage()
      override;

 protected:
  /// @brief Update the main tab page of work window with
  /// solution_design_.result_
  void UpadateTabMainFirstPageElementViewResult() override;

  std::unique_ptr<anx::esolution::ExpDesignResult> ExpDesignResultFromControl()
      override;

 private:
  WorkWindow* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
};

class WorkWindowFirstPageVibrationBending : public DuiLib::CNotifyPump,
                                            public PageSolutionDesignBase {
 public:
  explicit WorkWindowFirstPageVibrationBending(
      WorkWindow* pOwner,
      DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowFirstPageVibrationBending();

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg); // NOLINT

 public:
  std::unique_ptr<anx::esolution::SolutionDesign> SolutionDesignFromPage()
      override;

 protected:
  /// @brief Update the main tab page of work window with
  /// solution_design_.result_
  void UpadateTabMainFirstPageElementViewResult() override;

  std::unique_ptr<anx::esolution::ExpDesignResult> ExpDesignResultFromControl()
      override;

 private:
  WorkWindow* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_FIRST_PAGE_SOLUTION_DESIGN_H_
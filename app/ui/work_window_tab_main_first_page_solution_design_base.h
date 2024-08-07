/**
 * @file work_window_tab_main_first_page_solution_design_base.h
 * @author hhool (hhool@outlook.com)
 * @brief work window for page solution design base class
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_TAB_MAIN_FIRST_PAGE_SOLUTION_DESIGN_BASE_H_
#define APP_UI_WORK_WINDOW_TAB_MAIN_FIRST_PAGE_SOLUTION_DESIGN_BASE_H_

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;  // NOLINT

namespace anx {
namespace esolution {
class SolutionDesign;
struct ExpDesignHeader;
struct ExpDesignBaseParam;
struct ExpDesignResult;
}  // namespace esolution
namespace ui {
class WorkWindow;
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {
class PageSolutionDesignBase {
 public:
  PageSolutionDesignBase(DuiLib::CPaintManagerUI* paint_manager_ui,
                         int32_t design_type);
  virtual ~PageSolutionDesignBase();

 public:
  /// @brief Initialize the page control, set the control data
  /// with default value from solution_design and show the page
  /// source is at root directory of the executable default folder
  /// @return none
  /// @see Dispose  releated to InitPage.
  /// @see SolutionDesignFromPage releated to InitPage. get the solution design
  /// from the page control
  /// InitPage is called by the work window when the page is created
  /// and the page is shown.
  /// InitPage -> SolutionDesignFromPage
  void InitPage();

  /// @brief Load the solution design file from the file path
  /// and set the control data with the solution design
  /// @param filepath the file path of the solution design file
  /// @return int32_t 0 success, non-zero fail
  /// @see SolutionDesignFromPage releated to LoadSolutionDesingFile. get the
  /// solution design from the page control LoadSolutionDesingFile is called by
  /// the work window when the page is created and the page is shown.
  /// LoadSolutionDesingFile -> SolutionDesignFromPage
  int32_t LoadSolutionDesingFile(const std::string& filepath);

  /// @brief Dispose the page control, release the control data
  /// @return none
  void Dispose();

 public:
  /// @brief Get the solution design from the page control and
  /// return the solution design. The solution design is used to
  /// save the solution design to the file.
  /// Called by the work window when the page is created and the page is shown.
  /// InitPage -> SolutionDesignFromPage
  /// LoadSolutionDesingFile -> SolutionDesignFromPage
  /// @return std::unique_ptr<anx::esolution::SolutionDesign> the solution
  /// design
  virtual std::unique_ptr<anx::esolution::SolutionDesign>
  SolutionDesignFromPage() = 0;

 protected:
  /// @brief Check the solution design control with the solution design
  int32_t CheckSolutionDesign(const anx::esolution::SolutionDesign& design);
  /// @brief Update the main tab page of work window with
  /// solution_design_.header_ and solution_design_.base_param_
  virtual int32_t UpadateTabMainFirstPageElementViewHeaderAndBaseParam();
  /// @brief Update the main tab page of work window with
  /// solution_design_.result_
  virtual void UpadateTabMainFirstPageElementViewResult() = 0;
  /// @brief Get The header of the solution design.
  /// @return std::unique_ptr<anx::esolution::ExpDesignHeader> the header of the
  /// solution design, nullptr if fail, the caller should delete the pointer
  /// after use.
  std::unique_ptr<anx::esolution::ExpDesignHeader> ExpDesignHeaderFromControl();
  /// @brief Get The base param of the solution design.
  /// @return std::unique_ptr<anx::esolution::ExpDesignBaseParam> the base param
  /// of the solution design, nullptr if fail, the caller should delete the
  /// pointer after use.
  std::unique_ptr<anx::esolution::ExpDesignBaseParam>
  ExpDesignBaseParamFromControl();
  /// @brief Get The result of the solution design.
  /// @return std::unique_ptr<anx::esolution::ExpDesignResult>
  virtual std::unique_ptr<anx::esolution::ExpDesignResult>
  ExpDesignResultFromControl() = 0;

 protected:
  int32_t design_type_;
  std::unique_ptr<esolution::SolutionDesign> solution_design_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_FIRST_PAGE_SOLUTION_DESIGN_BASE_H_

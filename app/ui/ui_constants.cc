/**
 * @file ui_constant.cc
 * @author hhool (hhool@outlook.com)
 * @brief  define file for the ui constant
 * @version 0.1
 * @date 2024-07-25
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "app/ui/ui_constants.h"

namespace anx {
namespace ui {

/////////////////////////////////////////////////////////////////////////////////////
//
//
// constrol class name and interface name

// UIActiveX
const TCHAR* const kUIActiveXClassName = _T("ActiveXUI");
const TCHAR* const kUIActiveXInterfaceName = _T("ActiveX");

// ComboUI
const TCHAR* const kComboUIClassName = _T("ComboUI");
const TCHAR* const kComboUIInterfaceName = _T("Combo");

// LabelUI
const TCHAR* const kLabelUIClassName = _T("LabelUI");
const TCHAR* const kLabelUIInterfaceName = _T("Label");

// ButtonUI
const TCHAR* const kButtonUIClassName = _T("ButtonUI");
const TCHAR* const kButtonUIInterfaceName = _T("Button");

// OptionUI
const TCHAR* const kOptionUIClassName = _T("OptionUI");
const TCHAR* const kOptionUIInterfaceName = _T("Option");

// TextUI
const TCHAR* const kTextUIClassName = _T("TextUI");
const TCHAR* const kTextUIInterfaceName = _T("Text");

// ProgressUI
const TCHAR* const kProgressUIClassName = _T("ProgressUI");
const TCHAR* const kProgressUIInterfaceName = _T("Progress");

// SliderUI
const TCHAR* const kSliderUIClassName = _T("SliderUI");
const TCHAR* const kSliderUIInterfaceName = _T("Slider");

// EditUI
const TCHAR* const kEditUIClassName = _T("EditUI");
const TCHAR* const kEditUIInterfaceName = _T("Edit");

// IEditUI
const TCHAR* const kIEditUIInterfaceName = _T("Edit");

// ScrollBarUI
const TCHAR* const kScrollBarUIClassName = _T("ScrollBarUI");
const TCHAR* const kScrollBarUIInterfaceName = _T("ScrollBar");

// ContainerUI
const TCHAR* const kContainerUIClassName = _T("ContainerUI");
const TCHAR* const kContainerUIInterfaceName = _T("Container");

// IContainerUI
const TCHAR* const kIContainerUIInterfaceName = _T("IContainer");

// VerticalLayoutUI
const TCHAR* const kVerticalLayoutUIClassName = _T("VerticalLayoutUI");
const TCHAR* const kVerticalLayoutUIInterfaceName = _T("VerticalLayout");

// HorizontalLayoutUI
const TCHAR* const kHorizontalLayoutUIClassName = _T("HorizontalLayoutUI");
const TCHAR* const kHorizontalLayoutUIInterfaceName = _T("HorizontalLayout");

// TileLayoutUI
const TCHAR* const kTileLayoutUIClassName = _T("TileLayoutUI");
const TCHAR* const kTileLayoutUIInterfaceName = _T("TileLayout");

// DialogLayoutUI
const TCHAR* const kDialogLayoutUIClassName = _T("DialogLayoutUI");
const TCHAR* const kDialogLayoutUIInterfaceName = _T("DialogLayout");

// TabLayoutUI
const TCHAR* const kTabLayoutUIClassName = _T("TabLayoutUI");
const TCHAR* const kTabLayoutUIInterfaceName = _T("TabLayout");

// ControlUI
const TCHAR* const kControlUIClassName = _T("ControlUI");
const TCHAR* const kControlUIInterfaceName = _T("Control");

// ListUI
const TCHAR* const kListUIClassName = _T("ListUI");
const TCHAR* const kListUIInterfaceName = _T("List");

// IListUI
const TCHAR* const kIListUIInterfaceName = _T("IList");

// IListOwnerUI
const TCHAR* const kIListOwnerUIInterfaceName = _T("IListOwner");

// ListHeaderUI
const TCHAR* const kListHeaderUIClassName = _T("ListHeaderUI");
const TCHAR* const kListHeaderUIInterfaceName = _T("ListHeader");

// ListHeaderItemUI
const TCHAR* const kListHeaderItemUIClassName = _T("ListHeaderItemUI");
const TCHAR* const kListHeaderItemUIInterfaceName = _T("ListHeaderItem");

// ListElementUI
const TCHAR* const kListElementUIClassName = _T("ListElementUI");
const TCHAR* const kListElementUIInterfaceName = _T("ListElement");

// IListItemUI
const TCHAR* const kIListItemUIInterfaceName = _T("ListItem");

// ListLabelElementUI
const TCHAR* const kListLabelElementUIClassName = _T("ListLabelElementUI");
const TCHAR* const kListLabelElementUIInterfaceName = _T("ListLabelElement");

// ListTextElementUI
const TCHAR* const kListTextElementUIClassName = _T("ListTextElementUI");
const TCHAR* const kListTextElementUIInterfaceName = _T("ListTextElement");

// ListExpandElementUI
// const TCHAR* const kListExpandElementUIClassName = _T("ListExpandElementUI");
// const TCHAR* const kListExpandElementUIInterfaceName =
// _T("ListExpandElement");

// ListContainerElementUI
const TCHAR* const kListContainerElementUIClassName =
    _T("ListContainerElementUI");
const TCHAR* const kListContainerElementUIInterfaceName =
    _T("ListContainerElement");

// RichEditUI
const TCHAR* const kRichEditUIClassName = _T("RichEditUI");
const TCHAR* const kRichEditUIInterfaceName = _T("RichEdit");

/////////////////////////////////////////////////////////////////////////////////////
//
//
// control related message
const TCHAR* const kWindowInit = _T("windowinit");
const TCHAR* const kClick = _T("click");
const TCHAR* const kSelectChanged = _T("selectchanged");
const TCHAR* const kItemSelect = _T("itemselect");
const TCHAR* const kItemActivate = _T("itemactivate");
const TCHAR* const kItemClick = _T("itemclick");
const TCHAR* const kDropDown = _T("dropdown");
const TCHAR* const kTimer = _T("timer");
const TCHAR* const kMenu = _T("menu");
const TCHAR* const kReturn = _T("return");
const TCHAR* const kTextChanged = _T("textchanged");
const TCHAR* const kKillFocus = _T("killfocus");
const TCHAR* const kSetFocus = _T("setfocus");
const TCHAR* const kValueChanged = _T("valuechanged");

/////////////////////////////////////////////////////////////////////////////////////
//
//
// header button related
const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");

/////////////////////////////////////////////////////////////////////////////////////
//
//
// header menu design related
const TCHAR* const kMenu_Design_Connect = _T("menu_design_item_connect");
const TCHAR* const kMenu_Design_DisConnect = _T("menu_design_item_disconnect");
const TCHAR* const kMenu_Design_Device_Settings =
    _T("menu_design_item_device_settings");
const TCHAR* const kMenu_Design_Modify_Addr =
    _T("menu_design_item_modify_address");
const TCHAR* const kMenu_Design_Read_Solution =
    _T("menu_design_item_read_solution");
const TCHAR* const kMenu_Design_Save_Solution =
    _T("menu_design_item_save_solution");
const TCHAR* const kMenu_Design_Reset_Solution =
    _T("menu_design_item_reset_solution");
const TCHAR* const kMenu_Design_Exit = _T("menu_design_item_exit");

/////////////////////////////////////////////////////////////////////////////////////
//
//
// header menu store related
const TCHAR* const kMenu_Store_ExpRecord = _T("menu_store_item_exp_record");

/////////////////////////////////////////////////////////////////////////////////////
//
//
// some menu.button related
const TCHAR* const kMenu_Button_ExpTitle = _T("exp_title");

}  // namespace ui
}  // namespace anx

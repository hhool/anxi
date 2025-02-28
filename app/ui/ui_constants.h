/**
 * @file ui_constant.h
 * @author hhool (hhool@outlook.com)
 * @brief  define file for the ui constant
 * @version 0.1
 * @date 2024-07-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_UI_CONSTANTS_H_
#define APP_UI_UI_CONSTANTS_H_

#include <tchar.h>

namespace anx {
namespace ui {

/////////////////////////////////////////////////////////////////////////////////////
//
//
// constrol class name and interface name

// UIActiveX
extern const TCHAR* const kUIActiveXClassName;      // = _T("ActiveXUI");
extern const TCHAR* const kUIActiveXInterfaceName;  // = _T("ActiveX");

// ComboUI
extern const TCHAR* const kComboUIClassName;      // = _T("ComboUI");
extern const TCHAR* const kComboUIInterfaceName;  // = _T("Combo");

// LabelUI
extern const TCHAR* const kLabelUIClassName;      // = _T("LabelUI");
extern const TCHAR* const kLabelUIInterfaceName;  // = _T("Label");

// ButtonUI
extern const TCHAR* const kButtonUIClassName;      // = _T("ButtonUI");
extern const TCHAR* const kButtonUIInterfaceName;  // = _T("Button");

// OptionUI
extern const TCHAR* const kOptionUIClassName;      // = _T("OptionUI");
extern const TCHAR* const kOptionUIInterfaceName;  // = _T("Option");

// TextUI
extern const TCHAR* const kTextUIClassName;      // = _T("TextUI");
extern const TCHAR* const kTextUIInterfaceName;  // = _T("Text");

// ProgressUI
extern const TCHAR* const kProgressUIClassName;      // = _T("ProgressUI");
extern const TCHAR* const kProgressUIInterfaceName;  // = _T("Progress");

// SliderUI
extern const TCHAR* const kSliderUIClassName;      // = _T("SliderUI");
extern const TCHAR* const kSliderUIInterfaceName;  // = _T("Slider");

// EditUI
extern const TCHAR* const kEditUIClassName;      // = _T("EditUI");
extern const TCHAR* const kEditUIInterfaceName;  // = _T("Edit");

// IEditUI
extern const TCHAR* const kIEditUIInterfaceName;  // = _T("Edit");

// ScrollBarUI
extern const TCHAR* const kScrollBarUIClassName;      // = _T("ScrollBarUI");
extern const TCHAR* const kScrollBarUIInterfaceName;  // = _T("ScrollBar");

// ContainerUI
extern const TCHAR* const kContainerUIClassName;      // = _T("ContainerUI");
extern const TCHAR* const kContainerUIInterfaceName;  // = _T("Container");

// IContainerUI
extern const TCHAR* const kIContainerUIInterfaceName;  // = _T("IContainer");

// VerticalLayoutUI
extern const TCHAR* const
    kVerticalLayoutUIClassName;  // = _T("VerticalLayoutUI");
extern const TCHAR* const
    kVerticalLayoutUIInterfaceName;  // = _T("VerticalLayout");

// HorizontalLayoutUI
extern const TCHAR* const
    kHorizontalLayoutUIClassName;  // = _T("HorizontalLayoutUI");
extern const TCHAR* const
    kHorizontalLayoutUIInterfaceName;  // = _T("HorizontalLayout");

// TileLayoutUI
extern const TCHAR* const kTileLayoutUIClassName;      // = _T("TileLayoutUI");
extern const TCHAR* const kTileLayoutUIInterfaceName;  // = _T("TileLayout");

// DialogLayoutUI
extern const TCHAR* const kDialogLayoutUIClassName;  // = _T("DialogLayoutUI");
extern const TCHAR* const
    kDialogLayoutUIInterfaceName;  // = _T("DialogLayout");

// TabLayoutUI
extern const TCHAR* const kTabLayoutUIClassName;      // = _T("TabLayoutUI");
extern const TCHAR* const kTabLayoutUIInterfaceName;  // = _T("TabLayout");

// ControlUI
extern const TCHAR* const kControlUIClassName;      // = _T("ControlUI");
extern const TCHAR* const kControlUIInterfaceName;  // = _T("Control");

// ListUI
extern const TCHAR* const kListUIClassName;      // = _T("ListUI");
extern const TCHAR* const kListUIInterfaceName;  // = _T("List");

// IListUI
extern const TCHAR* const kIListUIInterfaceName;  // = _T("IList");

// IListOwnerUI
extern const TCHAR* const kIListOwnerUIInterfaceName;  // = _T("IListOwner");

// ListHeaderUI
extern const TCHAR* const kListHeaderUIClassName;      // = _T("ListHeaderUI");
extern const TCHAR* const kListHeaderUIInterfaceName;  // = _T("ListHeader");

// ListHeaderItemUI
extern const TCHAR* const
    kListHeaderItemUIClassName;  // = _T("ListHeaderItemUI");
extern const TCHAR* const
    kListHeaderItemUIInterfaceName;  // = _T("ListHeaderItem");

// ListElementUI
extern const TCHAR* const kListElementUIClassName;  // = _T("ListElementUI");
extern const TCHAR* const kListElementUIInterfaceName;  // = _T("ListElement");

// IListItemUI
extern const TCHAR* const kIListItemUIInterfaceName;  // = _T("ListItem");

// ListLabelElementUI
extern const TCHAR* const
    kListLabelElementUIClassName;  // = _T("ListLabelElementUI");
extern const TCHAR* const
    kListLabelElementUIInterfaceName;  // = _T("ListLabelElement");

// ListTextElementUI
extern const TCHAR* const
    kListTextElementUIClassName;  // = _T("ListTextElementUI");
extern const TCHAR* const
    kListTextElementUIInterfaceName;  // = _T("ListTextElement");

// ListExpandElementUI
// extern const TCHAR* const kListExpandElementUIClassName;// =
// _T("ListExpandElementUI"); extern const TCHAR* const
// kListExpandElementUIInterfaceName;// = _T("ListExpandElement");

// ListContainerElementUI
extern const TCHAR* const
    kListContainerElementUIClassName;  // = _T("ListContainerElementUI");
extern const TCHAR* const
    kListContainerElementUIInterfaceName;  // = _T("ListContainerElement");

// RichEditUI
extern const TCHAR* const kRichEditUIClassName;      // = _T("RichEditUI");
extern const TCHAR* const kRichEditUIInterfaceName;  // = _T("RichEdit");

/////////////////////////////////////////////////////////////////////////////////////
//
//
// control related message
extern const TCHAR* const kWindowInit;     // = _T("windowinit");
extern const TCHAR* const kClick;          // = _T("click");
extern const TCHAR* const kSelectChanged;  // = _T("selectchanged");
extern const TCHAR* const kItemSelect;     // = _T("itemselect");
extern const TCHAR* const kItemActivate;   // = _T("itemactivate");
extern const TCHAR* const kItemClick;      // = _T("itemclick");
extern const TCHAR* const kDropDown;       // = _T("dropdown");
extern const TCHAR* const kTimer;          // = _T("timer");
extern const TCHAR* const kMenu;           // = _T("menu");
extern const TCHAR* const kReturn;         // = _T("return");
extern const TCHAR* const kTextChanged;    // = _T("textchanged");
extern const TCHAR* const kKillFocus;      // = _T("killfocus");
extern const TCHAR* const kSetFocus;       // = _T("setfocus");
extern const TCHAR* const kValueChanged;   // = _T("valuechanged");

/////////////////////////////////////////////////////////////////////////////////////
//
//
// header button related
extern const TCHAR* const kTitleControlName;          // = _T("apptitle");
extern const TCHAR* const kCloseButtonControlName;    // = _T("closebtn");
extern const TCHAR* const kMinButtonControlName;      // = _T("minbtn");
extern const TCHAR* const kMaxButtonControlName;      // = _T("maxbtn");
extern const TCHAR* const kRestoreButtonControlName;  // = _T("restorebtn");

/////////////////////////////////////////////////////////////////////////////////////
//
//
// header menu design related
extern const TCHAR* const kMenu_Design_Connect;     // = _T("menu_connect");
extern const TCHAR* const kMenu_Design_DisConnect;  // = _T("menu_disconnect");
extern const TCHAR* const
    kMenu_Design_Device_Settings;  // = _T("menu_device_settings");
extern const TCHAR* const
    kMenu_Design_Modify_Addr;  // = _T("menu_modify_address");
extern const TCHAR* const
    kMenu_Design_Read_Solution;  // = _T("menu_read_solution");
extern const TCHAR* const
    kMenu_Design_Save_Solution;               // = _T("menu_save_solution");
extern const TCHAR* const
    kMenu_Design_Reset_Solution;              // = _T("menu_reset_solution");
extern const TCHAR* const kMenu_Design_Exit;  // = _T("menu_exit");

/////////////////////////////////////////////////////////////////////////////////////
//
//
// header menu store related
extern const TCHAR* const
    kMenu_Store_ExpRecord;  // = _T("menu_store_item_exp_record");

/////////////////////////////////////////////////////////////////////////////////////
//
//
// some menu.button related
extern const TCHAR* const kMenu_Button_ExpTitle;  // = _T("exp_title");
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_UI_CONSTANTS_H_

#include "lua_imgui_enum.hpp"
#include "lua_imgui_type.hpp"
#include "imgui.h"
#include <vector>
#include <string_view>

struct enum_name_value_pair
{
    std::string_view name;
    int value;
};

struct enum_name_data_pair
{
    std::string_view name;
    std::vector<enum_name_value_pair> data;
};

using enum_data = std::vector<enum_name_data_pair>;

void imgui_binding_lua_register_enum(lua_State* L)
{
    auto regfunc = [&L](enum_data& datas) -> void {
        //                                                          // ? M
        for(auto& i : datas)
        {
            lua_pushlstring(L, i.name.data(), i.name.length());     // ? M k
            lua_createtable(L, 0, static_cast<int>(i.data.size())); // ? M k t
            for(auto& j : i.data)
            {
                lua_pushlstring(L, j.name.data(), j.name.length()); // ? M k t k
                lua_pushinteger(L, j.value);                        // ? M k t k v
                lua_settable(L, -3);                                // ? M k t
            }
            lua_settable(L, -3);                                    // ? M
        }
    };
    
    // common
    enum_data datas1 = {
{"ImGuiWindowFlags", {
    {"None"                     , ImGuiWindowFlags_None                     },
    {"NoTitleBar"               , ImGuiWindowFlags_NoTitleBar               },
    {"NoResize"                 , ImGuiWindowFlags_NoResize                 },
    {"NoMove"                   , ImGuiWindowFlags_NoMove                   },
    {"NoScrollbar"              , ImGuiWindowFlags_NoScrollbar              },
    {"NoScrollWithMouse"        , ImGuiWindowFlags_NoScrollWithMouse        },
    {"NoCollapse"               , ImGuiWindowFlags_NoCollapse               },
    {"AlwaysAutoResize"         , ImGuiWindowFlags_AlwaysAutoResize         },
    {"NoBackground"             , ImGuiWindowFlags_NoBackground             },
    {"NoSavedSettings"          , ImGuiWindowFlags_NoSavedSettings          },
    {"NoMouseInputs"            , ImGuiWindowFlags_NoMouseInputs            },
    {"MenuBar"                  , ImGuiWindowFlags_MenuBar                  },
    {"HorizontalScrollbar"      , ImGuiWindowFlags_HorizontalScrollbar      },
    {"NoFocusOnAppearing"       , ImGuiWindowFlags_NoFocusOnAppearing       },
    {"NoBringToFrontOnFocus"    , ImGuiWindowFlags_NoBringToFrontOnFocus    },
    {"AlwaysVerticalScrollbar"  , ImGuiWindowFlags_AlwaysVerticalScrollbar  },
    {"AlwaysHorizontalScrollbar", ImGuiWindowFlags_AlwaysHorizontalScrollbar},
    {"NoNavInputs"              , ImGuiWindowFlags_NoNavInputs              },
    {"NoNavFocus"               , ImGuiWindowFlags_NoNavFocus               },
    {"UnsavedDocument"          , ImGuiWindowFlags_UnsavedDocument          },
    {"NoNav"                    , ImGuiWindowFlags_NoNav                    },
    {"NoDecoration"             , ImGuiWindowFlags_NoDecoration             },
    {"NoInputs"                 , ImGuiWindowFlags_NoInputs                 },
}},
{"ImGuiChildFlags", {
    {"None"                  , ImGuiChildFlags_None                  },
    {"Border"                , ImGuiChildFlags_Border                },
    {"AlwaysUseWindowPadding", ImGuiChildFlags_AlwaysUseWindowPadding},
    {"ResizeX"               , ImGuiChildFlags_ResizeX               },
    {"ResizeY"               , ImGuiChildFlags_ResizeY               },
    {"AutoResizeX"           , ImGuiChildFlags_AutoResizeX           },
    {"AutoResizeY"           , ImGuiChildFlags_AutoResizeY           },
    {"AlwaysAutoResize"      , ImGuiChildFlags_AlwaysAutoResize      },
    {"FrameStyle"            , ImGuiChildFlags_FrameStyle            },
}},
{"ImGuiInputTextFlags", {
    {"None"               , ImGuiInputTextFlags_None               },
    {"CharsDecimal"       , ImGuiInputTextFlags_CharsDecimal       },
    {"CharsHexadecimal"   , ImGuiInputTextFlags_CharsHexadecimal   },
    {"CharsUppercase"     , ImGuiInputTextFlags_CharsUppercase     },
    {"CharsNoBlank"       , ImGuiInputTextFlags_CharsNoBlank       },
    {"AutoSelectAll"      , ImGuiInputTextFlags_AutoSelectAll      },
    {"EnterReturnsTrue"   , ImGuiInputTextFlags_EnterReturnsTrue   },
    {"CallbackCompletion" , ImGuiInputTextFlags_CallbackCompletion },
    {"CallbackHistory"    , ImGuiInputTextFlags_CallbackHistory    },
    {"CallbackAlways"     , ImGuiInputTextFlags_CallbackAlways     },
    {"CallbackCharFilter" , ImGuiInputTextFlags_CallbackCharFilter },
    {"AllowTabInput"      , ImGuiInputTextFlags_AllowTabInput      },
    {"CtrlEnterForNewLine", ImGuiInputTextFlags_CtrlEnterForNewLine},
    {"NoHorizontalScroll" , ImGuiInputTextFlags_NoHorizontalScroll },
    {"AlwaysOverwrite"    , ImGuiInputTextFlags_AlwaysOverwrite    },
    {"ReadOnly"           , ImGuiInputTextFlags_ReadOnly           },
    {"Password"           , ImGuiInputTextFlags_Password           },
    {"NoUndoRedo"         , ImGuiInputTextFlags_NoUndoRedo         },
    {"CharsScientific"    , ImGuiInputTextFlags_CharsScientific    },
    {"CallbackResize"     , ImGuiInputTextFlags_CallbackResize     },
    {"CallbackEdit"       , ImGuiInputTextFlags_CallbackEdit       },
    {"EscapeClearsAll"    , ImGuiInputTextFlags_EscapeClearsAll    },
}},
{"ImGuiTreeNodeFlags", {
    {"None"                , ImGuiTreeNodeFlags_None                },
    {"Selected"            , ImGuiTreeNodeFlags_Selected            },
    {"Framed"              , ImGuiTreeNodeFlags_Framed              },
    {"AllowOverlap"        , ImGuiTreeNodeFlags_AllowOverlap        },
    {"NoTreePushOnOpen"    , ImGuiTreeNodeFlags_NoTreePushOnOpen    },
    {"NoAutoOpenOnLog"     , ImGuiTreeNodeFlags_NoAutoOpenOnLog     },
    {"DefaultOpen"         , ImGuiTreeNodeFlags_DefaultOpen         },
    {"OpenOnDoubleClick"   , ImGuiTreeNodeFlags_OpenOnDoubleClick   },
    {"OpenOnArrow"         , ImGuiTreeNodeFlags_OpenOnArrow         },
    {"Leaf"                , ImGuiTreeNodeFlags_Leaf                },
    {"Bullet"              , ImGuiTreeNodeFlags_Bullet              },
    {"FramePadding"        , ImGuiTreeNodeFlags_FramePadding        },
    {"SpanAvailWidth"      , ImGuiTreeNodeFlags_SpanAvailWidth      },
    {"SpanFullWidth"       , ImGuiTreeNodeFlags_SpanFullWidth       },
    {"SpanAllColumns"      , ImGuiTreeNodeFlags_SpanAllColumns      },
    {"NavLeftJumpsBackHere", ImGuiTreeNodeFlags_NavLeftJumpsBackHere},
    {"CollapsingHeader"    , ImGuiTreeNodeFlags_CollapsingHeader    },
}},
{"ImGuiPopupFlags", {
    {"None"                   , ImGuiPopupFlags_None                   },
    {"MouseButtonLeft"        , ImGuiPopupFlags_MouseButtonLeft        },
    {"MouseButtonRight"       , ImGuiPopupFlags_MouseButtonRight       },
    {"MouseButtonMiddle"      , ImGuiPopupFlags_MouseButtonMiddle      },
    {"MouseButtonMask_"       , ImGuiPopupFlags_MouseButtonMask_       },
    {"MouseButtonDefault_"    , ImGuiPopupFlags_MouseButtonDefault_    },
    {"NoReopen"               , ImGuiPopupFlags_NoReopen               },
    {"NoOpenOverExistingPopup", ImGuiPopupFlags_NoOpenOverExistingPopup},
    {"NoOpenOverItems"        , ImGuiPopupFlags_NoOpenOverItems        },
    {"AnyPopupId"             , ImGuiPopupFlags_AnyPopupId             },
    {"AnyPopupLevel"          , ImGuiPopupFlags_AnyPopupLevel          },
    {"AnyPopup"               , ImGuiPopupFlags_AnyPopup               },
}},
{"ImGuiSelectableFlags", {
    {"None"            , ImGuiSelectableFlags_None            },
    {"DontClosePopups" , ImGuiSelectableFlags_DontClosePopups },
    {"SpanAllColumns"  , ImGuiSelectableFlags_SpanAllColumns  },
    {"AllowDoubleClick", ImGuiSelectableFlags_AllowDoubleClick},
    {"Disabled"        , ImGuiSelectableFlags_Disabled        },
}},
{"ImGuiComboFlags", {
    {"None"           , ImGuiComboFlags_None           },
    {"PopupAlignLeft" , ImGuiComboFlags_PopupAlignLeft },
    {"HeightSmall"    , ImGuiComboFlags_HeightSmall    },
    {"HeightRegular"  , ImGuiComboFlags_HeightRegular  },
    {"HeightLarge"    , ImGuiComboFlags_HeightLarge    },
    {"HeightLargest"  , ImGuiComboFlags_HeightLargest  },
    {"NoArrowButton"  , ImGuiComboFlags_NoArrowButton  },
    {"NoPreview"      , ImGuiComboFlags_NoPreview      },
    {"WidthFitPreview", ImGuiComboFlags_WidthFitPreview},
    {"HeightMask_"    , ImGuiComboFlags_HeightMask_    },
}},
{"ImGuiTabBarFlags", {
    {"None"                        , ImGuiTabBarFlags_None                        },
    {"Reorderable"                 , ImGuiTabBarFlags_Reorderable                 },
    {"AutoSelectNewTabs"           , ImGuiTabBarFlags_AutoSelectNewTabs           },
    {"TabListPopupButton"          , ImGuiTabBarFlags_TabListPopupButton          },
    {"NoCloseWithMiddleMouseButton", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton},
    {"NoTabListScrollingButtons"   , ImGuiTabBarFlags_NoTabListScrollingButtons   },
    {"NoTooltip"                   , ImGuiTabBarFlags_NoTooltip                   },
    {"FittingPolicyResizeDown"     , ImGuiTabBarFlags_FittingPolicyResizeDown     },
    {"FittingPolicyScroll"         , ImGuiTabBarFlags_FittingPolicyScroll         },
    {"FittingPolicyMask_"          , ImGuiTabBarFlags_FittingPolicyMask_          },
    {"FittingPolicyDefault_"       , ImGuiTabBarFlags_FittingPolicyDefault_       },
}},
{"ImGuiTabItemFlags", {
    {"None"                        , ImGuiTabItemFlags_None                        },
    {"UnsavedDocument"             , ImGuiTabItemFlags_UnsavedDocument             },
    {"SetSelected"                 , ImGuiTabItemFlags_SetSelected                 },
    {"NoCloseWithMiddleMouseButton", ImGuiTabItemFlags_NoCloseWithMiddleMouseButton},
    {"NoPushId"                    , ImGuiTabItemFlags_NoPushId                    },
    {"NoTooltip"                   , ImGuiTabItemFlags_NoTooltip                   },
    {"NoReorder"                   , ImGuiTabItemFlags_NoReorder                   },
    {"Leading"                     , ImGuiTabItemFlags_Leading                     },
    {"Trailing"                    , ImGuiTabItemFlags_Trailing                    },
    {"NoAssumedClosure"            , ImGuiTabItemFlags_NoAssumedClosure            },
}},
{"ImGuiTableFlags", {
    {"None"                      , ImGuiTableFlags_None                      },
    {"Resizable"                 , ImGuiTableFlags_Resizable                 },
    {"Reorderable"               , ImGuiTableFlags_Reorderable               },
    {"Hideable"                  , ImGuiTableFlags_Hideable                  },
    {"Sortable"                  , ImGuiTableFlags_Sortable                  },
    {"NoSavedSettings"           , ImGuiTableFlags_NoSavedSettings           },
    {"ContextMenuInBody"         , ImGuiTableFlags_ContextMenuInBody         },
    {"RowBg"                     , ImGuiTableFlags_RowBg                     },
    {"BordersInnerH"             , ImGuiTableFlags_BordersInnerH             },
    {"BordersOuterH"             , ImGuiTableFlags_BordersOuterH             },
    {"BordersInnerV"             , ImGuiTableFlags_BordersInnerV             },
    {"BordersOuterV"             , ImGuiTableFlags_BordersOuterV             },
    {"BordersH"                  , ImGuiTableFlags_BordersH                  },
    {"BordersV"                  , ImGuiTableFlags_BordersV                  },
    {"BordersInner"              , ImGuiTableFlags_BordersInner              },
    {"BordersOuter"              , ImGuiTableFlags_BordersOuter              },
    {"Borders"                   , ImGuiTableFlags_Borders                   },
    {"NoBordersInBody"           , ImGuiTableFlags_NoBordersInBody           },
    {"NoBordersInBodyUntilResize", ImGuiTableFlags_NoBordersInBodyUntilResize},
    {"SizingFixedFit"            , ImGuiTableFlags_SizingFixedFit            },
    {"SizingFixedSame"           , ImGuiTableFlags_SizingFixedSame           },
    {"SizingStretchProp"         , ImGuiTableFlags_SizingStretchProp         },
    {"SizingStretchSame"         , ImGuiTableFlags_SizingStretchSame         },
    {"NoHostExtendX"             , ImGuiTableFlags_NoHostExtendX             },
    {"NoHostExtendY"             , ImGuiTableFlags_NoHostExtendY             },
    {"NoKeepColumnsVisible"      , ImGuiTableFlags_NoKeepColumnsVisible      },
    {"PreciseWidths"             , ImGuiTableFlags_PreciseWidths             },
    {"NoClip"                    , ImGuiTableFlags_NoClip                    },
    {"PadOuterX"                 , ImGuiTableFlags_PadOuterX                 },
    {"NoPadOuterX"               , ImGuiTableFlags_NoPadOuterX               },
    {"NoPadInnerX"               , ImGuiTableFlags_NoPadInnerX               },
    {"ScrollX"                   , ImGuiTableFlags_ScrollX                   },
    {"ScrollY"                   , ImGuiTableFlags_ScrollY                   },
    {"SortMulti"                 , ImGuiTableFlags_SortMulti                 },
    {"SortTristate"              , ImGuiTableFlags_SortTristate              },
    {"HighlightHoveredColumn"    , ImGuiTableFlags_HighlightHoveredColumn    },
}},
{"ImGuiTableColumnFlags", {
    {"None"                , ImGuiTableColumnFlags_None                },
    {"Disabled"            , ImGuiTableColumnFlags_Disabled            },
    {"DefaultHide"         , ImGuiTableColumnFlags_DefaultHide         },
    {"DefaultSort"         , ImGuiTableColumnFlags_DefaultSort         },
    {"WidthStretch"        , ImGuiTableColumnFlags_WidthStretch        },
    {"WidthFixed"          , ImGuiTableColumnFlags_WidthFixed          },
    {"NoResize"            , ImGuiTableColumnFlags_NoResize            },
    {"NoReorder"           , ImGuiTableColumnFlags_NoReorder           },
    {"NoHide"              , ImGuiTableColumnFlags_NoHide              },
    {"NoClip"              , ImGuiTableColumnFlags_NoClip              },
    {"NoSort"              , ImGuiTableColumnFlags_NoSort              },
    {"NoSortAscending"     , ImGuiTableColumnFlags_NoSortAscending     },
    {"NoSortDescending"    , ImGuiTableColumnFlags_NoSortDescending    },
    {"NoHeaderLabel"       , ImGuiTableColumnFlags_NoHeaderLabel       },
    {"NoHeaderWidth"       , ImGuiTableColumnFlags_NoHeaderWidth       },
    {"PreferSortAscending" , ImGuiTableColumnFlags_PreferSortAscending },
    {"PreferSortDescending", ImGuiTableColumnFlags_PreferSortDescending},
    {"IndentEnable"        , ImGuiTableColumnFlags_IndentEnable        },
    {"IndentDisable"       , ImGuiTableColumnFlags_IndentDisable       },
    {"AngledHeader"        , ImGuiTableColumnFlags_AngledHeader        },
    {"IsEnabled"           , ImGuiTableColumnFlags_IsEnabled           },
    {"IsVisible"           , ImGuiTableColumnFlags_IsVisible           },
    {"IsSorted"            , ImGuiTableColumnFlags_IsSorted            },
    {"IsHovered"           , ImGuiTableColumnFlags_IsHovered           },
}},
{"ImGuiTableRowFlags", {
    {"None"   , ImGuiTableRowFlags_None   },
    {"Headers", ImGuiTableRowFlags_Headers},
}},
{"ImGuiTableBgTarget", {
    {"None"  , ImGuiTableBgTarget_None  },
    {"RowBg0", ImGuiTableBgTarget_RowBg0},
    {"RowBg1", ImGuiTableBgTarget_RowBg1},
    {"CellBg", ImGuiTableBgTarget_CellBg},
}},
{"ImGuiFocusedFlags", {
    {"None"               , ImGuiFocusedFlags_None               },
    {"ChildWindows"       , ImGuiFocusedFlags_ChildWindows       },
    {"RootWindow"         , ImGuiFocusedFlags_RootWindow         },
    {"AnyWindow"          , ImGuiFocusedFlags_AnyWindow          },
    {"NoPopupHierarchy"   , ImGuiFocusedFlags_NoPopupHierarchy   },
    {"RootAndChildWindows", ImGuiFocusedFlags_RootAndChildWindows},
}},
{"ImGuiHoveredFlags", {
    {"None"                        , ImGuiHoveredFlags_None                        },
    {"ChildWindows"                , ImGuiHoveredFlags_ChildWindows                },
    {"RootWindow"                  , ImGuiHoveredFlags_RootWindow                  },
    {"AnyWindow"                   , ImGuiHoveredFlags_AnyWindow                   },
    {"NoPopupHierarchy"            , ImGuiHoveredFlags_NoPopupHierarchy            },
    {"AllowWhenBlockedByPopup"     , ImGuiHoveredFlags_AllowWhenBlockedByPopup     },
    {"AllowWhenBlockedByActiveItem", ImGuiHoveredFlags_AllowWhenBlockedByActiveItem},
    {"AllowWhenOverlappedByItem"   , ImGuiHoveredFlags_AllowWhenOverlappedByItem   },
    {"AllowWhenOverlappedByWindow" , ImGuiHoveredFlags_AllowWhenOverlappedByWindow },
    {"AllowWhenDisabled"           , ImGuiHoveredFlags_AllowWhenDisabled           },
    {"NoNavOverride"               , ImGuiHoveredFlags_NoNavOverride               },
    {"AllowWhenOverlapped"         , ImGuiHoveredFlags_AllowWhenOverlapped         },
    {"RectOnly"                    , ImGuiHoveredFlags_RectOnly                    },
    {"RootAndChildWindows"         , ImGuiHoveredFlags_RootAndChildWindows         },
    {"ForTooltip"                  , ImGuiHoveredFlags_ForTooltip                  },
    {"Stationary"                  , ImGuiHoveredFlags_Stationary                  },
    {"DelayNone"                   , ImGuiHoveredFlags_DelayNone                   },
    {"DelayShort"                  , ImGuiHoveredFlags_DelayShort                  },
    {"DelayNormal"                 , ImGuiHoveredFlags_DelayNormal                 },
    {"NoSharedDelay"               , ImGuiHoveredFlags_NoSharedDelay               },
}},
{"ImGuiDragDropFlags", {
    {"None"                    , ImGuiDragDropFlags_None                    },
    {"SourceNoPreviewTooltip"  , ImGuiDragDropFlags_SourceNoPreviewTooltip  },
    {"SourceNoDisableHover"    , ImGuiDragDropFlags_SourceNoDisableHover    },
    {"SourceNoHoldToOpenOthers", ImGuiDragDropFlags_SourceNoHoldToOpenOthers},
    {"SourceAllowNullID"       , ImGuiDragDropFlags_SourceAllowNullID       },
    {"SourceExtern"            , ImGuiDragDropFlags_SourceExtern            },
    {"SourceAutoExpirePayload" , ImGuiDragDropFlags_SourceAutoExpirePayload },
    {"AcceptBeforeDelivery"    , ImGuiDragDropFlags_AcceptBeforeDelivery    },
    {"AcceptNoDrawDefaultRect" , ImGuiDragDropFlags_AcceptNoDrawDefaultRect },
    {"AcceptNoPreviewTooltip"  , ImGuiDragDropFlags_AcceptNoPreviewTooltip  },
    {"AcceptPeekOnly"          , ImGuiDragDropFlags_AcceptPeekOnly          },
}},
{"ImGuiDataType", {
    {"S8"    , ImGuiDataType_S8    },
    {"U8"    , ImGuiDataType_U8    },
    {"S16"   , ImGuiDataType_S16   },
    {"U16"   , ImGuiDataType_U16   },
    {"S32"   , ImGuiDataType_S32   },
    {"U32"   , ImGuiDataType_U32   },
    {"S64"   , ImGuiDataType_S64   },
    {"U64"   , ImGuiDataType_U64   },
    {"Float" , ImGuiDataType_Float },
    {"Double", ImGuiDataType_Double},
    // lua type
    {"Integer", ImGuiDataType_Integer},
    {"Number" , ImGuiDataType_Number },
}},
{"ImGuiDir", {
    {"None" , ImGuiDir_None },
    {"Left" , ImGuiDir_Left },
    {"Right", ImGuiDir_Right},
    {"Up"   , ImGuiDir_Up   },
    {"Down" , ImGuiDir_Down },
}},
{"ImGuiSortDirection", {
    {"None"      , ImGuiSortDirection_None      },
    {"Ascending" , ImGuiSortDirection_Ascending },
    {"Descending", ImGuiSortDirection_Descending},
}},
{"ImGuiKey", {
    {"None"          , ImGuiKey_None          },
    {"Tab"           , ImGuiKey_Tab           },
    {"LeftArrow"     , ImGuiKey_LeftArrow     },
    {"RightArrow"    , ImGuiKey_RightArrow    },
    {"UpArrow"       , ImGuiKey_UpArrow       },
    {"DownArrow"     , ImGuiKey_DownArrow     },
    {"PageUp"        , ImGuiKey_PageUp        },
    {"PageDown"      , ImGuiKey_PageDown      },
    {"Home"          , ImGuiKey_Home          },
    {"End"           , ImGuiKey_End           },
    {"Insert"        , ImGuiKey_Insert        },
    {"Delete"        , ImGuiKey_Delete        },
    {"Backspace"     , ImGuiKey_Backspace     },
    {"Space"         , ImGuiKey_Space         },
    {"Enter"         , ImGuiKey_Enter         },
    {"Escape"        , ImGuiKey_Escape        },
    {"LeftCtrl"      , ImGuiKey_LeftCtrl      },
    {"LeftShift"     , ImGuiKey_LeftShift     },
    {"LeftAlt"       , ImGuiKey_LeftAlt       },
    {"LeftSuper"     , ImGuiKey_LeftSuper     },
    {"RightCtrl"     , ImGuiKey_RightCtrl     },
    {"RightShift"    , ImGuiKey_RightShift    },
    {"RightAlt"      , ImGuiKey_RightAlt      },
    {"RightSuper"    , ImGuiKey_RightSuper    },
    {"Menu"          , ImGuiKey_Menu          },
    {"N0"            , ImGuiKey_0             },
    {"N1"            , ImGuiKey_1             },
    {"N2"            , ImGuiKey_2             },
    {"N3"            , ImGuiKey_3             },
    {"N4"            , ImGuiKey_4             },
    {"N5"            , ImGuiKey_5             },
    {"N6"            , ImGuiKey_6             },
    {"N7"            , ImGuiKey_7             },
    {"N8"            , ImGuiKey_8             },
    {"N9"            , ImGuiKey_9             },
    {"A"             , ImGuiKey_A             },
    {"B"             , ImGuiKey_B             },
    {"C"             , ImGuiKey_C             },
    {"D"             , ImGuiKey_D             },
    {"E"             , ImGuiKey_E             },
    {"F"             , ImGuiKey_F             },
    {"G"             , ImGuiKey_G             },
    {"H"             , ImGuiKey_H             },
    {"I"             , ImGuiKey_I             },
    {"J"             , ImGuiKey_J             },
    {"K"             , ImGuiKey_K             },
    {"L"             , ImGuiKey_L             },
    {"M"             , ImGuiKey_M             },
    {"N"             , ImGuiKey_N             },
    {"O"             , ImGuiKey_O             },
    {"P"             , ImGuiKey_P             },
    {"Q"             , ImGuiKey_Q             },
    {"R"             , ImGuiKey_R             },
    {"S"             , ImGuiKey_S             },
    {"T"             , ImGuiKey_T             },
    {"U"             , ImGuiKey_U             },
    {"V"             , ImGuiKey_V             },
    {"W"             , ImGuiKey_W             },
    {"X"             , ImGuiKey_X             },
    {"Y"             , ImGuiKey_Y             },
    {"Z"             , ImGuiKey_Z             },
    {"F1"            , ImGuiKey_F1            },
    {"F2"            , ImGuiKey_F2            },
    {"F3"            , ImGuiKey_F3            },
    {"F4"            , ImGuiKey_F4            },
    {"F5"            , ImGuiKey_F5            },
    {"F6"            , ImGuiKey_F6            },
    {"F7"            , ImGuiKey_F7            },
    {"F8"            , ImGuiKey_F8            },
    {"F9"            , ImGuiKey_F9            },
    {"F10"           , ImGuiKey_F10           },
    {"F11"           , ImGuiKey_F11           },
    {"F12"           , ImGuiKey_F12           },
    {"F13"           , ImGuiKey_F13           },
    {"F14"           , ImGuiKey_F14           },
    {"F15"           , ImGuiKey_F15           },
    {"F16"           , ImGuiKey_F16           },
    {"F17"           , ImGuiKey_F17           },
    {"F18"           , ImGuiKey_F18           },
    {"F19"           , ImGuiKey_F19           },
    {"F20"           , ImGuiKey_F20           },
    {"F21"           , ImGuiKey_F21           },
    {"F22"           , ImGuiKey_F22           },
    {"F23"           , ImGuiKey_F23           },
    {"F24"           , ImGuiKey_F24           },
    {"Apostrophe"    , ImGuiKey_Apostrophe    },
    {"Comma"         , ImGuiKey_Comma         },
    {"Minus"         , ImGuiKey_Minus         },
    {"Period"        , ImGuiKey_Period        },
    {"Slash"         , ImGuiKey_Slash         },
    {"Semicolon"     , ImGuiKey_Semicolon     },
    {"Equal"         , ImGuiKey_Equal         },
    {"LeftBracket"   , ImGuiKey_LeftBracket   },
    {"Backslash"     , ImGuiKey_Backslash     },
    {"RightBracket"  , ImGuiKey_RightBracket  },
    {"GraveAccent"   , ImGuiKey_GraveAccent   },
    {"CapsLock"      , ImGuiKey_CapsLock      },
    {"ScrollLock"    , ImGuiKey_ScrollLock    },
    {"NumLock"       , ImGuiKey_NumLock       },
    {"PrintScreen"   , ImGuiKey_PrintScreen   },
    {"Pause"         , ImGuiKey_Pause         },
    {"Keypad0"       , ImGuiKey_Keypad0       },
    {"Keypad1"       , ImGuiKey_Keypad1       },
    {"Keypad2"       , ImGuiKey_Keypad2       },
    {"Keypad3"       , ImGuiKey_Keypad3       },
    {"Keypad4"       , ImGuiKey_Keypad4       },
    {"Keypad5"       , ImGuiKey_Keypad5       },
    {"Keypad6"       , ImGuiKey_Keypad6       },
    {"Keypad7"       , ImGuiKey_Keypad7       },
    {"Keypad8"       , ImGuiKey_Keypad8       },
    {"Keypad9"       , ImGuiKey_Keypad9       },
    {"KeypadDecimal" , ImGuiKey_KeypadDecimal },
    {"KeypadDivide"  , ImGuiKey_KeypadDivide  },
    {"KeypadMultiply", ImGuiKey_KeypadMultiply},
    {"KeypadSubtract", ImGuiKey_KeypadSubtract},
    {"KeypadAdd"     , ImGuiKey_KeypadAdd     },
    {"KeypadEnter"   , ImGuiKey_KeypadEnter   },
    {"KeypadEqual"   , ImGuiKey_KeypadEqual   },
    {"AppBack"       , ImGuiKey_AppBack       },
    {"AppForward"    , ImGuiKey_AppForward    },
    {"GamepadStart"      , ImGuiKey_GamepadStart      },
    {"GamepadBack"       , ImGuiKey_GamepadBack       },
    {"GamepadFaceUp"     , ImGuiKey_GamepadFaceUp     },
    {"GamepadFaceDown"   , ImGuiKey_GamepadFaceDown   },
    {"GamepadFaceLeft"   , ImGuiKey_GamepadFaceLeft   },
    {"GamepadFaceRight"  , ImGuiKey_GamepadFaceRight  },
    {"GamepadDpadUp"     , ImGuiKey_GamepadDpadUp     },
    {"GamepadDpadDown"   , ImGuiKey_GamepadDpadDown   },
    {"GamepadDpadLeft"   , ImGuiKey_GamepadDpadLeft   },
    {"GamepadDpadRight"  , ImGuiKey_GamepadDpadRight  },
    {"GamepadL1"         , ImGuiKey_GamepadL1         },
    {"GamepadR1"         , ImGuiKey_GamepadR1         },
    {"GamepadL2"         , ImGuiKey_GamepadL2         },
    {"GamepadR2"         , ImGuiKey_GamepadR2         },
    {"GamepadL3"         , ImGuiKey_GamepadL3         },
    {"GamepadR3"         , ImGuiKey_GamepadR3         },
    {"GamepadLStickUp"   , ImGuiKey_GamepadLStickUp   },
    {"GamepadLStickDown" , ImGuiKey_GamepadLStickDown },
    {"GamepadLStickLeft" , ImGuiKey_GamepadLStickLeft },
    {"GamepadLStickRight", ImGuiKey_GamepadLStickRight},
    {"GamepadRStickUp"   , ImGuiKey_GamepadRStickUp   },
    {"GamepadRStickDown" , ImGuiKey_GamepadRStickDown },
    {"GamepadRStickLeft" , ImGuiKey_GamepadRStickLeft },
    {"GamepadRStickRight", ImGuiKey_GamepadRStickRight},
    {"MouseLeft"  , ImGuiKey_MouseLeft  },
    {"MouseRight" , ImGuiKey_MouseRight },
    {"MouseMiddle", ImGuiKey_MouseMiddle},
    {"MouseX1"    , ImGuiKey_MouseX1    },
    {"MouseX2"    , ImGuiKey_MouseX2    },
    {"MouseWheelX", ImGuiKey_MouseWheelX},
    {"MouseWheelY", ImGuiKey_MouseWheelY},
}},
{"ImGuiMod", {
    {"None"    , ImGuiMod_None    },
    {"Ctrl"    , ImGuiMod_Ctrl    },
    {"Shift"   , ImGuiMod_Shift   },
    {"Alt"     , ImGuiMod_Alt     },
    {"Super"   , ImGuiMod_Super   },
    {"Mask_"   , ImGuiMod_Mask_   },
    {"Shortcut", ImGuiMod_Shortcut},
}},
{"ImGuiConfigFlags", {
    {"None"                , ImGuiConfigFlags_None                },
    {"NavEnableKeyboard"   , ImGuiConfigFlags_NavEnableKeyboard   },
    {"NavEnableGamepad"    , ImGuiConfigFlags_NavEnableGamepad    },
    {"NavEnableSetMousePos", ImGuiConfigFlags_NavEnableSetMousePos},
    {"NavNoCaptureKeyboard", ImGuiConfigFlags_NavNoCaptureKeyboard},
    {"NoMouse"             , ImGuiConfigFlags_NoMouse             },
    {"NoMouseCursorChange" , ImGuiConfigFlags_NoMouseCursorChange },
    {"IsSRGB"              , ImGuiConfigFlags_IsSRGB              },
    {"IsTouchScreen"       , ImGuiConfigFlags_IsTouchScreen       },
}},
{"ImGuiBackendFlags", {
    {"None"                , ImGuiBackendFlags_None                },
    {"HasGamepad"          , ImGuiBackendFlags_HasGamepad          },
    {"HasMouseCursors"     , ImGuiBackendFlags_HasMouseCursors     },
    {"HasSetMousePos"      , ImGuiBackendFlags_HasSetMousePos      },
    {"RendererHasVtxOffset", ImGuiBackendFlags_RendererHasVtxOffset},
}},
{"ImGuiCol", {
    {"Text"                 , ImGuiCol_Text                 },
    {"TextDisabled"         , ImGuiCol_TextDisabled         },
    {"WindowBg"             , ImGuiCol_WindowBg             },
    {"ChildBg"              , ImGuiCol_ChildBg              },
    {"PopupBg"              , ImGuiCol_PopupBg              },
    {"Border"               , ImGuiCol_Border               },
    {"BorderShadow"         , ImGuiCol_BorderShadow         },
    {"FrameBg"              , ImGuiCol_FrameBg              },
    {"FrameBgHovered"       , ImGuiCol_FrameBgHovered       },
    {"FrameBgActive"        , ImGuiCol_FrameBgActive        },
    {"TitleBg"              , ImGuiCol_TitleBg              },
    {"TitleBgActive"        , ImGuiCol_TitleBgActive        },
    {"TitleBgCollapsed"     , ImGuiCol_TitleBgCollapsed     },
    {"MenuBarBg"            , ImGuiCol_MenuBarBg            },
    {"ScrollbarBg"          , ImGuiCol_ScrollbarBg          },
    {"ScrollbarGrab"        , ImGuiCol_ScrollbarGrab        },
    {"ScrollbarGrabHovered" , ImGuiCol_ScrollbarGrabHovered },
    {"ScrollbarGrabActive"  , ImGuiCol_ScrollbarGrabActive  },
    {"CheckMark"            , ImGuiCol_CheckMark            },
    {"SliderGrab"           , ImGuiCol_SliderGrab           },
    {"SliderGrabActive"     , ImGuiCol_SliderGrabActive     },
    {"Button"               , ImGuiCol_Button               },
    {"ButtonHovered"        , ImGuiCol_ButtonHovered        },
    {"ButtonActive"         , ImGuiCol_ButtonActive         },
    {"Header"               , ImGuiCol_Header               },
    {"HeaderHovered"        , ImGuiCol_HeaderHovered        },
    {"HeaderActive"         , ImGuiCol_HeaderActive         },
    {"Separator"            , ImGuiCol_Separator            },
    {"SeparatorHovered"     , ImGuiCol_SeparatorHovered     },
    {"SeparatorActive"      , ImGuiCol_SeparatorActive      },
    {"ResizeGrip"           , ImGuiCol_ResizeGrip           },
    {"ResizeGripHovered"    , ImGuiCol_ResizeGripHovered    },
    {"ResizeGripActive"     , ImGuiCol_ResizeGripActive     },
    {"Tab"                  , ImGuiCol_Tab                  },
    {"TabHovered"           , ImGuiCol_TabHovered           },
    {"TabActive"            , ImGuiCol_TabActive            },
    {"TabUnfocused"         , ImGuiCol_TabUnfocused         },
    {"TabUnfocusedActive"   , ImGuiCol_TabUnfocusedActive   },
    {"PlotLines"            , ImGuiCol_PlotLines            },
    {"PlotLinesHovered"     , ImGuiCol_PlotLinesHovered     },
    {"PlotHistogram"        , ImGuiCol_PlotHistogram        },
    {"PlotHistogramHovered" , ImGuiCol_PlotHistogramHovered },
    {"TableHeaderBg"        , ImGuiCol_TableHeaderBg        },
    {"TableBorderStrong"    , ImGuiCol_TableBorderStrong    },
    {"TableBorderLight"     , ImGuiCol_TableBorderLight     },
    {"TableRowBg"           , ImGuiCol_TableRowBg           },
    {"TableRowBgAlt"        , ImGuiCol_TableRowBgAlt        },
    {"TextSelectedBg"       , ImGuiCol_TextSelectedBg       },
    {"DragDropTarget"       , ImGuiCol_DragDropTarget       },
    {"NavHighlight"         , ImGuiCol_NavHighlight         },
    {"NavWindowingHighlight", ImGuiCol_NavWindowingHighlight},
    {"NavWindowingDimBg"    , ImGuiCol_NavWindowingDimBg    },
    {"ModalWindowDimBg"     , ImGuiCol_ModalWindowDimBg     },
}},
{"ImGuiStyleVar", {
    {"Alpha"              , ImGuiStyleVar_Alpha              },
    {"DisabledAlpha"      , ImGuiStyleVar_DisabledAlpha      },
    {"WindowPadding"      , ImGuiStyleVar_WindowPadding      },
    {"WindowRounding"     , ImGuiStyleVar_WindowRounding     },
    {"WindowBorderSize"   , ImGuiStyleVar_WindowBorderSize   },
    {"WindowMinSize"      , ImGuiStyleVar_WindowMinSize      },
    {"WindowTitleAlign"   , ImGuiStyleVar_WindowTitleAlign   },
    {"ChildRounding"      , ImGuiStyleVar_ChildRounding      },
    {"ChildBorderSize"    , ImGuiStyleVar_ChildBorderSize    },
    {"PopupRounding"      , ImGuiStyleVar_PopupRounding      },
    {"PopupBorderSize"    , ImGuiStyleVar_PopupBorderSize    },
    {"FramePadding"       , ImGuiStyleVar_FramePadding       },
    {"FrameRounding"      , ImGuiStyleVar_FrameRounding      },
    {"FrameBorderSize"    , ImGuiStyleVar_FrameBorderSize    },
    {"ItemSpacing"        , ImGuiStyleVar_ItemSpacing        },
    {"ItemInnerSpacing"   , ImGuiStyleVar_ItemInnerSpacing   },
    {"IndentSpacing"      , ImGuiStyleVar_IndentSpacing      },
    {"CellPadding"        , ImGuiStyleVar_CellPadding        },
    {"ScrollbarSize"      , ImGuiStyleVar_ScrollbarSize      },
    {"ScrollbarRounding"  , ImGuiStyleVar_ScrollbarRounding  },
    {"GrabMinSize"        , ImGuiStyleVar_GrabMinSize        },
    {"GrabRounding"       , ImGuiStyleVar_GrabRounding       },
    {"TabRounding"        , ImGuiStyleVar_TabRounding        },
    {"TabBorderSize"      , ImGuiStyleVar_TabBorderSize      },
    {"TabBarBorderSize"   , ImGuiStyleVar_TabBarBorderSize   },
    {"TableAngledHeadersAngle", ImGuiStyleVar_TableAngledHeadersAngle},
    {"ButtonTextAlign"    , ImGuiStyleVar_ButtonTextAlign    },
    {"SelectableTextAlign", ImGuiStyleVar_SelectableTextAlign},
    {"SeparatorTextBorderSize", ImGuiStyleVar_SeparatorTextBorderSize},
    {"SeparatorTextAlign"     , ImGuiStyleVar_SeparatorTextAlign     },
    {"SeparatorTextPadding"   , ImGuiStyleVar_SeparatorTextPadding   },
}},
{"ImGuiButtonFlags", {
    {"None"             , ImGuiButtonFlags_None             },
    {"MouseButtonLeft"  , ImGuiButtonFlags_MouseButtonLeft  },
    {"MouseButtonRight" , ImGuiButtonFlags_MouseButtonRight },
    {"MouseButtonMiddle", ImGuiButtonFlags_MouseButtonMiddle},
}},
{"ImGuiColorEditFlags", {
    {"None"            , ImGuiColorEditFlags_None            },
    {"NoAlpha"         , ImGuiColorEditFlags_NoAlpha         },
    {"NoPicker"        , ImGuiColorEditFlags_NoPicker        },
    {"NoOptions"       , ImGuiColorEditFlags_NoOptions       },
    {"NoSmallPreview"  , ImGuiColorEditFlags_NoSmallPreview  },
    {"NoInputs"        , ImGuiColorEditFlags_NoInputs        },
    {"NoTooltip"       , ImGuiColorEditFlags_NoTooltip       },
    {"NoLabel"         , ImGuiColorEditFlags_NoLabel         },
    {"NoSidePreview"   , ImGuiColorEditFlags_NoSidePreview   },
    {"NoDragDrop"      , ImGuiColorEditFlags_NoDragDrop      },
    {"NoBorder"        , ImGuiColorEditFlags_NoBorder        },
    {"AlphaBar"        , ImGuiColorEditFlags_AlphaBar        },
    {"AlphaPreview"    , ImGuiColorEditFlags_AlphaPreview    },
    {"AlphaPreviewHalf", ImGuiColorEditFlags_AlphaPreviewHalf},
    {"HDR"             , ImGuiColorEditFlags_HDR             },
    {"DisplayRGB"      , ImGuiColorEditFlags_DisplayRGB      },
    {"DisplayHSV"      , ImGuiColorEditFlags_DisplayHSV      },
    {"DisplayHex"      , ImGuiColorEditFlags_DisplayHex      },
    {"Uint8"           , ImGuiColorEditFlags_Uint8           },
    {"Float"           , ImGuiColorEditFlags_Float           },
    {"PickerHueBar"    , ImGuiColorEditFlags_PickerHueBar    },
    {"PickerHueWheel"  , ImGuiColorEditFlags_PickerHueWheel  },
    {"InputRGB"        , ImGuiColorEditFlags_InputRGB        },
    {"InputHSV"        , ImGuiColorEditFlags_InputHSV        },
    {"DefaultOptions_" , ImGuiColorEditFlags_DefaultOptions_ },
}},
{"ImGuiSliderFlags", {
    {"None"           , ImGuiSliderFlags_None           },
    {"AlwaysClamp"    , ImGuiSliderFlags_AlwaysClamp    },
    {"Logarithmic"    , ImGuiSliderFlags_Logarithmic    },
    {"NoRoundToFormat", ImGuiSliderFlags_NoRoundToFormat},
    {"NoInput"        , ImGuiSliderFlags_NoInput        },
}},
{"ImGuiMouseButton", {
    {"Left"  , ImGuiMouseButton_Left  },
    {"Right" , ImGuiMouseButton_Right },
    {"Middle", ImGuiMouseButton_Middle},
}},
{"ImGuiMouseCursor", {
    {"None"      , ImGuiMouseCursor_None      },
    {"Arrow"     , ImGuiMouseCursor_Arrow     },
    {"TextInput" , ImGuiMouseCursor_TextInput },
    {"ResizeAll" , ImGuiMouseCursor_ResizeAll },
    {"ResizeNS"  , ImGuiMouseCursor_ResizeNS  },
    {"ResizeEW"  , ImGuiMouseCursor_ResizeEW  },
    {"ResizeNESW", ImGuiMouseCursor_ResizeNESW},
    {"ResizeNWSE", ImGuiMouseCursor_ResizeNWSE},
    {"Hand"      , ImGuiMouseCursor_Hand      },
    {"NotAllowed", ImGuiMouseCursor_NotAllowed},
}},
{"ImGuiMouseSource", {
    {"Mouse"      , ImGuiMouseSource_Mouse      },
    {"TouchScreen", ImGuiMouseSource_TouchScreen},
    {"Pen"        , ImGuiMouseSource_Pen        },
}},
{"ImGuiCond", {
    {"None"        , ImGuiCond_None        },
    {"Always"      , ImGuiCond_Always      },
    {"Once"        , ImGuiCond_Once        },
    {"FirstUseEver", ImGuiCond_FirstUseEver},
    {"Appearing"   , ImGuiCond_Appearing   },
}},
    };
    regfunc(datas1);
    
    // draw
    enum_data datas2 = {
{"ImDrawFlags", {
    {"None"                   , ImDrawFlags_None                   },
    {"Closed"                 , ImDrawFlags_Closed                 },
    {"RoundCornersTopLeft"    , ImDrawFlags_RoundCornersTopLeft    },
    {"RoundCornersTopRight"   , ImDrawFlags_RoundCornersTopRight   },
    {"RoundCornersBottomLeft" , ImDrawFlags_RoundCornersBottomLeft },
    {"RoundCornersBottomRight", ImDrawFlags_RoundCornersBottomRight},
    {"RoundCornersNone"       , ImDrawFlags_RoundCornersNone       },
    {"RoundCornersTop"        , ImDrawFlags_RoundCornersTop        },
    {"RoundCornersBottom"     , ImDrawFlags_RoundCornersBottom     },
    {"RoundCornersLeft"       , ImDrawFlags_RoundCornersLeft       },
    {"RoundCornersRight"      , ImDrawFlags_RoundCornersRight      },
    {"RoundCornersAll"        , ImDrawFlags_RoundCornersAll        },
    {"RoundCornersDefault_"   , ImDrawFlags_RoundCornersDefault_   },
    {"RoundCornersMask_"      , ImDrawFlags_RoundCornersMask_      },
}},
{"ImDrawListFlags", {
    {"None"                  , ImDrawListFlags_None                  },
    {"AntiAliasedLines"      , ImDrawListFlags_AntiAliasedLines      },
    {"AntiAliasedLinesUseTex", ImDrawListFlags_AntiAliasedLinesUseTex},
    {"AntiAliasedFill"       , ImDrawListFlags_AntiAliasedFill       },
    {"AllowVtxOffset"        , ImDrawListFlags_AllowVtxOffset        },
}},
    };
    regfunc(datas2);
    
    // font
    enum_data datas3 = {
{"ImFontAtlasFlags", {
    {"None"              , ImFontAtlasFlags_None              },
    {"NoPowerOfTwoHeight", ImFontAtlasFlags_NoPowerOfTwoHeight},
    {"NoMouseCursors"    , ImFontAtlasFlags_NoMouseCursors    },
    {"NoBakedLines"      , ImFontAtlasFlags_NoBakedLines      },
}},
    };
    regfunc(datas3);
    
    // viewport
    enum_data datas4 = {
{"ImGuiViewportFlags", {
    {"None"             , ImGuiViewportFlags_None             },
    {"IsPlatformWindow" , ImGuiViewportFlags_IsPlatformWindow },
    {"IsPlatformMonitor", ImGuiViewportFlags_IsPlatformMonitor},
    {"OwnedByApp"       , ImGuiViewportFlags_OwnedByApp       },
}},
    };
    regfunc(datas4);
}

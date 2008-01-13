HEADER
; IDL Visual Widget Builder Resource file. Version 1
; Generated on:	01/09/2008 17:58.44
VERSION 1
END

WID_BASE_0 BASE 5 5 901 556
TLB
CAPTION "IDL"
XPAD = 3
YPAD = 3
SPACE = 3
BEGIN
  WID_BASE_0_MBAR MENUBAR 0 0 0 0
  BEGIN
    W_MENU_0 PUSHBUTTON 0 0 0 0
    VALUE "&File"
    MENU
    BEGIN
      W_MENU_2 PUSHBUTTON 0 0 0 0
      VALUE "&Laod"
      ONACTIVATE "FileLoad"
      END
      W_MENU_5 PUSHBUTTON 0 0 0 0
      VALUE "&Export"
      GRAYED
      ONACTIVATE "FileExport"
      END
      W_MENU_4 PUSHBUTTON 0 0 0 0
      VALUE "E&xit"
      ONACTIVATE "FileExit"
      END
    END
    W_MENU_6 PUSHBUTTON 0 0 0 0
    VALUE "&View"
    MENU
    BEGIN
      W_MENU_7 PUSHBUTTON 0 0 0 0
      VALUE "&Whole"
      ONACTIVATE "ViewWhole"
      END
      W_MENU_8 PUSHBUTTON 0 0 0 0
      VALUE "Peak&1"
      ONACTIVATE "ViewPeak1"
      END
      W_MENU_9 PUSHBUTTON 0 0 0 0
      VALUE "Peak&2"
      ONACTIVATE "ViewPeak2"
      END
    END
  END
  WID_DRAW_0 DRAW 0 19 724 490
    ONEXPOSE "DrawExpose"
  END
  WID_TEXT_0 TEXT 724 19 169 490
  FRAME = 1
  WIDTH = 20
  HEIGHT = 1
  END
END

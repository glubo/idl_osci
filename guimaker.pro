; 
; IDL Widget Interface Procedures. This Code is automatically 
;     generated and should not be modified.

; 
; Generated on:	01/10/2008 13:33.45
; 
pro WID_BASE_0_event, Event

  wTarget = (widget_info(Event.id,/NAME) eq 'TREE' ?  $
      widget_info(Event.id, /tree_root) : event.id)


  wWidget =  Event.top

  case wTarget of

    Widget_Info(wWidget, FIND_BY_UNAME='W_MENU_2'): begin
      if( Tag_Names(Event, /STRUCTURE_NAME) eq 'WIDGET_BUTTON' )then $
        FileLoad, Event
    end
    Widget_Info(wWidget, FIND_BY_UNAME='W_MENU_5'): begin
      if( Tag_Names(Event, /STRUCTURE_NAME) eq 'WIDGET_BUTTON' )then $
        FileExport, Event
    end
    Widget_Info(wWidget, FIND_BY_UNAME='W_MENU_4'): begin
      if( Tag_Names(Event, /STRUCTURE_NAME) eq 'WIDGET_BUTTON' )then $
        FileExit, Event
    end
    Widget_Info(wWidget, FIND_BY_UNAME='WID_DRAW_0'): begin
      if( Tag_Names(Event, /STRUCTURE_NAME) eq 'WIDGET_DRAW' )then $
        if( Event.type eq 4 )then $
          DrawExpose, Event
    end
    else:
  endcase

end

pro WID_BASE_0, GROUP_LEADER=wGroup, _EXTRA=_VWBExtra_

  Resolve_Routine, 'guimaker_eventcb',/COMPILE_FULL_FILE  ; Load event callback routines
  
  WID_BASE_0 = Widget_Base( GROUP_LEADER=wGroup, UNAME='WID_BASE_0'  $
      ,XOFFSET=5 ,YOFFSET=5 ,SCR_XSIZE=901 ,SCR_YSIZE=556  $
      ,TITLE='IDL' ,SPACE=3 ,XPAD=3 ,YPAD=3 ,MBAR=WID_BASE_0_MBAR)

  
  W_MENU_0 = Widget_Button(WID_BASE_0_MBAR, UNAME='W_MENU_0' ,/MENU  $
      ,VALUE='&File')

  
  W_MENU_2 = Widget_Button(W_MENU_0, UNAME='W_MENU_2' ,VALUE='&Laod')
  
  W_MENU_5 = Widget_Button(W_MENU_0, UNAME='W_MENU_5' ,SENSITIVE=0  $
      ,VALUE='&Export')

  
  W_MENU_4 = Widget_Button(W_MENU_0, UNAME='W_MENU_4' ,VALUE='E&xit')
  
  WID_DRAW_0 = Widget_Draw(WID_BASE_0, UNAME='WID_DRAW_0' ,YOFFSET=19  $
      ,SCR_XSIZE=724 ,SCR_YSIZE=490 ,RETAIN=0 ,/EXPOSE_EVENTS)

  
  WID_TEXT_0 = Widget_Text(WID_BASE_0, UNAME='WID_TEXT_0' ,FRAME=1  $
      ,XOFFSET=724 ,YOFFSET=19 ,SCR_XSIZE=169 ,SCR_YSIZE=490  $
      ,XSIZE=20 ,YSIZE=1)

  Widget_Control, /REALIZE, WID_BASE_0

  XManager, 'WID_BASE_0', WID_BASE_0, /NO_BLOCK  

end
; 
; Empty stub procedure used for autoloading.
; 
pro guimaker, GROUP_LEADER=wGroup, _EXTRA=_VWBExtra_
  WID_BASE_0, GROUP_LEADER=wGroup, _EXTRA=_VWBExtra_
end

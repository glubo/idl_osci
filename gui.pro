
pro FileExit, Event
	WIDGET_CONTROL, Event.top, /DESTROY
end

pro FileLoadDir, Event
	common directory, files
	dir = DIALOG_PICKFILE(/READ, /DIRECTORY, /FIX_FILTER)
	
	IF N_ELEMENTS(files) NE 0 THEN destroy_dir, files
	files = load_dir(dir)

	draw = widget_info(Event.top, FIND_BY_UNAME='WID_DRAW_0')
	FillFileList, Event, files
	WIDGET_CONTROL, draw, GET_VALUE=drawID
	wset, drawID
	;replot
end

pro FillFileList, Event, files
	filelist = 	Widget_Info(Event.top, FIND_BY_UNAME='W_FILELIST')
	widget_control, filelist, COMBOBOX_SETVALUE=['']
	list = replicate('',N_ELEMENTS(files))
	for i=0, N_ELEMENTS(files)-1 do begin
		list[i]=files[i].filename
	endfor
end

pro FileList, Event
	print, Event.index
	print, Event.STR
end

pro gui_window_event, Event
	wTarget = (widget_info(Event.id, /NAME) eq 'TREE' ? $
		widget_info(Event.id, /tree_root) : Event.id)

	wWidget = Event.top

	case wTarget of
		Widget_Info(wWidget, FIND_BY_UNAME='W_MENU_FX'): begin
			if( Tag_Names(Event, /STRUCTURE_NAME) eq 'WIDGET_BUTTON' )then $
				FileExit, Event
		end
		Widget_Info(wWidget, FIND_BY_UNAME='W_MENU_FL'): begin
			if( Tag_Names(Event, /STRUCTURE_NAME) eq 'WIDGET_BUTTON' )then $
				FileLoadDir, Event
		end
		Widget_Info(wWidget, FIND_BY_UNAME='W_FILELIST'): begin
			if( Tag_Names(Event, /STRUCTURE_NAME) eq 'WIDGET_COMBOBOX' )then $
				FileList, Event
		end
		else:
	end
	
end

pro gui_window, GROUP_LEADER=wGroup, _EXTRA=_VWBExtra_
	Resolve_Routine, 'gui_window_event', /COMPILE_FULL_FILE

	GUI_WINDOW = Widget_Base (GROUP_LEADER=wGroup, UNAME="GUI_WINDOW", $
		XOFFSET=5, YOFFSET=5, SCR_XSIZE=800, SCR_YSIZE=600, ROW=2, $
		TITLE='Kulickator', SPACE=3, XPAD=3, YPAD=3, MBAR=GUI_WINDOW_MBAR)

	W_MENU_0 = Widget_Button(GUI_WINDOW_MBAR, UNAME='W_MENU_0' ,/MENU  $
		,VALUE='&File')
	W_MENU_2 = Widget_Button(W_MENU_0, UNAME='W_MENU_FL' ,VALUE='&Laod dir')
	W_MENU_5 = Widget_Button(W_MENU_0, UNAME='W_MENU_5' ,SENSITIVE=0  $
		,VALUE='&Export')
	W_MENU_4 = Widget_Button(W_MENU_0, UNAME='W_MENU_FX' ,VALUE='E&xit')

	GUI_WINDOWT = Widget_Base ( GUI_WINDOW, /ROW)
	W_L = Widget_Label(GUI_WINDOWT, Value='File:')
	W_File = Widget_ComboBox(GUI_WINDOWT, /DYNAMIC_RESIZE, UNAME='W_FILELIST');, Value=['Ahoj', 'Beta'])
	W_L2 = Widget_Label(GUI_WINDOWT, Value='Show:')
	W_Show = Widget_ComboBox(GUI_WINDOWT, /DYNAMIC_RESIZE);, Value=['Ahoj', 'Beta'])
	W_D = Widget_Draw(GUI_WINDOW,XSIZE=800, YSIZE=600, UNAME="WID_DRAW_0")

	;Widget_Control, /REALIZE, GUI_WINDOWT
	Widget_Control, /REALIZE, GUI_WINDOW
	XManager, 'GUI_WINDOW', GUI_WINDOW, /NO_BLOCK  
end

pro gui, GROUP_LEADER=wGroup, _EXTRA=_VWBExtra_
	gui_window, GROUP_LEADER=wGroup, _EXTRA=_VWBExtra_
end

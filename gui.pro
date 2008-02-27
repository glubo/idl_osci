
pro FileExit, Event
	WIDGET_CONTROL, Event.top, /DESTROY
end

pro replot
	common directory, files, selectedid, showid

	IF NOT KEYWORD_SET(files) then return

	case showid of
		1: begin
			data = *files[selectedid].analyzed.peak_1_data
		end
		2: begin
			data = *files[selectedid].analyzed.peak_2_data
		end
		3: begin
			ns = files[selectedid].analyzed.noise_start
			data = (*files[selectedid].raw.channel_a)[ns:*]
		end
		else: begin
			data = *files[selectedid].raw.channel_a
		end
	end

	x = indgen(N_ELEMENTS(data))*(0.001*files[selectedid].analyzed.musps)
	plot, x, data, XTITLE='t [ms]', YTITLE='U [A.U.]'
end

pro FileLoadDir, Event
	common directory, files, selectedid, showid
	dir = DIALOG_PICKFILE(/READ, /DIRECTORY, /FIX_FILTER)
	
	IF N_ELEMENTS(files) NE 0 THEN destroy_dir, files
	files = load_dir(dir)

	FillFileList, Event, files
	selectedid = 0
	showid = 0

	draw = widget_info(Event.top, FIND_BY_UNAME='WID_DRAW_0')
	WIDGET_CONTROL, draw, GET_VALUE=drawID
	wset, drawID
	replot

	UpdateDirInfo, Event, dir
end

pro UpdateFileInfo, Event
	common directory, files, selectedid, showid

	info = widget_info(Event.top, FIND_BY_UNAME='WID_IF')
	text = "name: "+files[selectedid].filename+newline()


	text = text + 'p1='+STRTRIM(STRING(files[selectedid].analyzed.peak_1),1)+' A.U.'+newline()
	text = text + 'p2='+STRTRIM(STRING(files[selectedid].analyzed.peak_2),1)+' A.U.'+newline()

	WIDGET_CONTROL, info, SET_VALUE = text
end

pro UpdateDirInfo, Event, path
	common directory, files, selectedid, showid

	info = widget_info(Event.top, FIND_BY_UNAME='WID_ID')
	text = "path: "+path+newline()

	N = N_ELEMENTS(files)
	peak1 = DBLARR(N)
	peak2 = DBLARR(N)

	peak1 = files.analyzed.peak_1
	peak2 = files.analyzed.peak_2

	p1 = MOMENT(peak1, SDEV=p1e)
	p2 = MOMENT(peak2, SDEV=p2e)

	text = text + 'p1=('+STRTRIM(STRING(p1[0]),1)+'+/-'+STRTRIM(STRING(p1e),1)+') A.U.'+newline()
	text = text + 'p2=('+STRTRIM(STRING(p2[0]),1)+'+/-'+STRTRIM(STRING(p2e),1)+') A.U.'+newline()

	WIDGET_CONTROL, info, SET_VALUE = text
end

pro FillFileList, Event, files
	filelist = 	Widget_Info(Event.top, FIND_BY_UNAME='W_FILELIST')
	list = replicate('',N_ELEMENTS(files))
	list=files.filename
	widget_control, filelist, SET_VALUE=list
end

pro FileList, Event
;called when item selected from File List
	common directory, files, selectedid, showid
	selectedid = Event.index

	draw = widget_info(Event.top, FIND_BY_UNAME='WID_DRAW_0')
	WIDGET_CONTROL, draw, GET_VALUE=drawID
	wset, drawID
	replot

	UpdateFileInfo, Event
end

pro ShowList, Event
;called when item selected from Show List
	common directory, files, selectedid, showid
	showid = Event.index

	draw = widget_info(Event.top, FIND_BY_UNAME='WID_DRAW_0')
	WIDGET_CONTROL, draw, GET_VALUE=drawID
	wset, drawID
	replot
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
		Widget_Info(wWidget, FIND_BY_UNAME='W_SHOWLIST'): begin
			if( Tag_Names(Event, /STRUCTURE_NAME) eq 'WIDGET_COMBOBOX' )then $
				ShowList, Event
		end
		else:
	end
	
end

pro gui_window, GROUP_LEADER=wGroup, _EXTRA=_VWBExtra_
	Resolve_Routine, 'gui_window_event', /COMPILE_FULL_FILE

	GUI_WINDOW = Widget_Base (GROUP_LEADER=wGroup, UNAME="GUI_WINDOW", $
		ROW=2, $
		TITLE='Kulickator', MBAR=GUI_WINDOW_MBAR)

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
	W_Show = Widget_ComboBox(GUI_WINDOWT, /DYNAMIC_RESIZE, UNAME="W_SHOWLIST", Value=['Whole Data', 'Peak 1', 'Peak 2', 'Noise'])

	GUI_WINDOWM = Widget_Base ( GUI_WINDOW, /ROW)

	W_D = Widget_Draw(GUI_WINDOWM,XSIZE=800, YSIZE=600, UNAME="WID_DRAW_0")
	GUI_WINDOWI = Widget_Base ( GUI_WINDOWM, /COLUMN)
	W_L = Widget_Label(GUI_WINDOWI, Value='Dir Info:')
	W_ID = Widget_Text(GUI_WINDOWI, UNAME="WID_ID", XSIZE=40, YSIZE=6)
	W_L = Widget_Label(GUI_WINDOWI, Value='File Info:')
	W_IF = Widget_Text(GUI_WINDOWI, UNAME="WID_IF", YSIZE=6)

	;Widget_Control, /REALIZE, GUI_WINDOWT
	Widget_Control, /REALIZE, GUI_WINDOW
	XManager, 'GUI_WINDOW', GUI_WINDOW, /NO_BLOCK  
end

pro gui, GROUP_LEADER=wGroup, _EXTRA=_VWBExtra_
	gui_window, GROUP_LEADER=wGroup, _EXTRA=_VWBExtra_
end

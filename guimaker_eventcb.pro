;
; IDL Event Callback Procedures
; guimaker_eventcb
;
; Generated on:	01/09/2008 17:50.19
;
;-----------------------------------------------------------------
; Activate Button Callback Procedure.
; Argument:
;   Event structure:
;
;   {WIDGET_BUTTON, ID:0L, TOP:0L, HANDLER:0L, SELECT:0}
;
;   ID is the widget ID of the component generating the event. TOP is
;       the widget ID of the top level widget containing ID. HANDLER
;       contains the widget ID of the widget associated with the
;       handler routine.

;   SELECT is set to 1 if the button was set, and 0 if released.
;       Normal buttons do not generate events when released, so
;       SELECT will always be 1. However, toggle buttons (created by
;       parenting a button to an exclusive or non-exclusive base)
;       return separate events for the set and release actions.

;   Retrieve the IDs of other widgets in the widget hierarchy using
;       id=widget_info(Event.top, FIND_BY_UNAME=name)

;-----------------------------------------------------------------
pro FileLoad, Event
	file = DIALOG_PICKFILE(/READ)
	nactidata, file
	draw = widget_info(Event.top, FIND_BY_UNAME='WID_DRAW_0')
	WIDGET_CONTROL, draw, GET_VALUE=drawID
	wset, drawID
	replot
	analyze_data
end
;-----------------------------------------------------------------
; Activate Button Callback Procedure.
; Argument:
;   Event structure:
;
;   {WIDGET_BUTTON, ID:0L, TOP:0L, HANDLER:0L, SELECT:0}
;
;   ID is the widget ID of the component generating the event. TOP is
;       the widget ID of the top level widget containing ID. HANDLER
;       contains the widget ID of the widget associated with the
;       handler routine.

;   SELECT is set to 1 if the button was set, and 0 if released.
;       Normal buttons do not generate events when released, so
;       SELECT will always be 1. However, toggle buttons (created by
;       parenting a button to an exclusive or non-exclusive base)
;       return separate events for the set and release actions.

;   Retrieve the IDs of other widgets in the widget hierarchy using
;       id=widget_info(Event.top, FIND_BY_UNAME=name)

;-----------------------------------------------------------------
pro FileExport, Event

end
;-----------------------------------------------------------------
; Activate Button Callback Procedure.
; Argument:
;   Event structure:
;
;   {WIDGET_BUTTON, ID:0L, TOP:0L, HANDLER:0L, SELECT:0}
;
;   ID is the widget ID of the component generating the event. TOP is
;       the widget ID of the top level widget containing ID. HANDLER
;       contains the widget ID of the widget associated with the
;       handler routine.

;   SELECT is set to 1 if the button was set, and 0 if released.
;       Normal buttons do not generate events when released, so
;       SELECT will always be 1. However, toggle buttons (created by
;       parenting a button to an exclusive or non-exclusive base)
;       return separate events for the set and release actions.

;   Retrieve the IDs of other widgets in the widget hierarchy using
;       id=widget_info(Event.top, FIND_BY_UNAME=name)

;-----------------------------------------------------------------
pro FileExit, Event
	WIDGET_CONTROL, Event.top, /DESTROY
end
;-----------------------------------------------------------------
; EXPOSE_EVENTS Callback Procedure.
; Argument:
;   Event structure:
;   
;   {WIDGET_DRAW, ID:0L, TOP:0L, HANDLER:0L, TYPE: 0, X:0, Y:0, 
;       PRESS:0B, RELEASE:0B, CLICKS:0}

;   ID is the widget ID of the component generating the event. TOP is 
;       the widget ID of the top level widget containing ID. HANDLER 
;       contains the widget ID of the widget associated with the 
;       handler routine.

;   TYPE returns a value that describes the type of draw widget 
;       interaction that generated an event: 0 - Button Press, 1 - 
;       Button Release, 2 - Motion, 3 - Viewport Moved, 4 - 
;       Visibility Changed (Expose)


;   Retrieve the IDs of other widgets in the widget hierarchy using 
;       id=widget_info(Event.top, FIND_BY_UNAME=name)

;-----------------------------------------------------------------
pro DrawExpose, Event
	draw = widget_info(Event.top, FIND_BY_UNAME='WID_DRAW_0')
	WIDGET_CONTROL, draw, GET_VALUE=drawID
	wset, drawID
	replot
end
;-----------------------------------------------------------------
; Activate Button Callback Procedure.
; Argument:
;   Event structure:
;   
;   {WIDGET_BUTTON, ID:0L, TOP:0L, HANDLER:0L, SELECT:0}
;   
;   ID is the widget ID of the component generating the event. TOP is 
;       the widget ID of the top level widget containing ID. HANDLER 
;       contains the widget ID of the widget associated with the 
;       handler routine.

;   SELECT is set to 1 if the button was set, and 0 if released. 
;       Normal buttons do not generate events when released, so 
;       SELECT will always be 1. However, toggle buttons (created by 
;       parenting a button to an exclusive or non-exclusive base) 
;       return separate events for the set and release actions.

;   Retrieve the IDs of other widgets in the widget hierarchy using 
;       id=widget_info(Event.top, FIND_BY_UNAME=name)

;-----------------------------------------------------------------
pro ViewWhole, Event
	common viewmode, viewid
	viewid = 0
	replot
end
;-----------------------------------------------------------------
; Activate Button Callback Procedure.
; Argument:
;   Event structure:
;   
;   {WIDGET_BUTTON, ID:0L, TOP:0L, HANDLER:0L, SELECT:0}
;   
;   ID is the widget ID of the component generating the event. TOP is 
;       the widget ID of the top level widget containing ID. HANDLER 
;       contains the widget ID of the widget associated with the 
;       handler routine.

;   SELECT is set to 1 if the button was set, and 0 if released. 
;       Normal buttons do not generate events when released, so 
;       SELECT will always be 1. However, toggle buttons (created by 
;       parenting a button to an exclusive or non-exclusive base) 
;       return separate events for the set and release actions.

;   Retrieve the IDs of other widgets in the widget hierarchy using 
;       id=widget_info(Event.top, FIND_BY_UNAME=name)

;-----------------------------------------------------------------
pro ViewPeak1, Event
	common viewmode, viewid
	viewid = 1
	replot
end
;-----------------------------------------------------------------
; Activate Button Callback Procedure.
; Argument:
;   Event structure:
;   
;   {WIDGET_BUTTON, ID:0L, TOP:0L, HANDLER:0L, SELECT:0}
;   
;   ID is the widget ID of the component generating the event. TOP is 
;       the widget ID of the top level widget containing ID. HANDLER 
;       contains the widget ID of the widget associated with the 
;       handler routine.

;   SELECT is set to 1 if the button was set, and 0 if released. 
;       Normal buttons do not generate events when released, so 
;       SELECT will always be 1. However, toggle buttons (created by 
;       parenting a button to an exclusive or non-exclusive base) 
;       return separate events for the set and release actions.

;   Retrieve the IDs of other widgets in the widget hierarchy using 
;       id=widget_info(Event.top, FIND_BY_UNAME=name)

;-----------------------------------------------------------------
pro ViewPeak2, Event
	common viewmode, viewid
	viewid = 2
	replot
end
;
; Empty stub procedure used for autoloading.
;
pro guimaker_eventcb
end

pro replot
	common data, channel_a, range_a, timebase
	common datainfo, musps, vpl, negative, t_fall, t_peak_max, noise_start, peak_1, peak_2, peak_1_data, peak_2_data
	common viewmode, viewid
	
	;pokud zatim nemame nastaveni vyobrazeni, nastavime vyobrazit cela data
	if NOT KEYWORD_SET(viewid) then viewid = 0


	if KEYWORD_SET(channel_a) then begin
		case viewid of
			0: channel = channel_a
			1: channel = peak_1_data
			2: channel = peak_2_data
			else: channel = channel_a
		end
		if KEYWORD_SET(musps) then begin
			x = indgen(N_ELEMENTS(channel))*(musps/1000.)
			plot, x, channel, YTICK_GET = V, /NODATA, COLOR='0000a0'x, XTITLE = 't [ms]'
			axis, 0.0,0.0, COLOR='0aff00'x, XTICKNAME = REPLICATE(' ', N_ELEMENTS(V))
			oplot, x, channel
		end else begin
			plot, channel
		end
	end
end

pro replot
	common data, channel_a, range_a, timebase
	common datainfo, musps, vpl, negative, t_fall, t_peak_max, noise_start, peak_1, peak_2, peak_1_data, peak_2_data
	common viewmode, viewid
	;x = indgen(N_ELEMENTS(peak))*musps
	
	;pokud zatim nemame nastaveni vyobrazeni, nastavime vyobrazit cela data
	if NOT KEYWORD_SET(viewid) then viewid = 0


	if KEYWORD_SET(channel_a) then begin
		case viewid of
			0: channel = channel_a
			1: channel = peak_1_data
			2: channel = peak_2_data
			else: channel = channel_a
		end
		plot, channel
	end
end

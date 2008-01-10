pro replot
	common data, channel_a, range_a, timebase
	common datainfo, musps, vpl, zero, negative, t_fall, t_peak_max, noise_start
	
	if KEYWORD_SET(channel_a) then begin
		plot, channel_a
		if KEYWORD_SET(zero) then axis, 0, zero
	end
end

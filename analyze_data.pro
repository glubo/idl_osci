pro analyze_data
	common data, channel_a, range_a, timebase
	common datainfo, musps, vpl, zero, negative, t_fall, t_peak_max, noise_start
	
	a = min(channel_a, peak_min)
	a = max(channel_a, peak_max)
	
	print, "Peak_min: ", peak_min
	print, "Peak_max: ", peak_max

	negative = peak_min > peak_max ; mame negativni castici?
	t_fall = abs(peak_min-peak_max) ; doba mezi peaky, tedy doba propadu castice valcem
	t_peak_max = t_fall/2 ; empiricky odhadnuta maximalni delka peaku

	noise_start = (negative?peak_min:peak_max) + t_peak_max
	print, "Noise_start: ", noise_start

	if (noise_start>N_ELEMENTS(channel_a)) then print, "ERROR:Noise starts after end of data" ;TODO vyhledove: nejak zlepsit zpracovani chyby
	
	noise = channel_a[noise_start:*]
	zero = mean(noise)

	print, "Zero: ", zero
end

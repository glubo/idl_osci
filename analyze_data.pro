pro analyze_data
	common data, channel_a, range_a, timebase
	common datainfo, musps, vpl, negative, t_fall, t_peak_max, noise_start, peak_1, peak_2, peak_1_data, peak_2_data
	
	a = min(channel_a, peak_min)
	a = max(channel_a, peak_max)
	
	print, "Peak_min: ", peak_min
	print, "Peak_max: ", peak_max

	negative = peak_min GT peak_max ; mame negativni castici?
	t_fall = abs(peak_min-peak_max) ; doba mezi peaky, tedy doba propadu castice valcem
	t_peak_max = t_fall/2 ; empiricky odhadnuta maximalni delka peaku

	;a nyni si urcime polohu nuly jako stredni hodnotu sumu
	noise_start = (negative?peak_min:peak_max) + t_peak_max
	print, "Noise_start: ", noise_start

	if (noise_start GE N_ELEMENTS(channel_a)) then print, "ERROR:Noise starts after end of data" ;TODO vyhledove: nejak zlepsit zpracovani chyby
	
	noise = channel_a[noise_start:*]
	zero = mean(noise)

	print, "Zero: ", zero

	channel_a = channel_a - zero

	musps = tb2musps(timebase)
	;a nyni si spocteme plochu peaku
	peak_1 = integrate_peak(channel_a, peak_max, t_peak_max, musps, saveit=peak_1_data)
	peak_2 = integrate_peak(channel_a, peak_min, t_peak_max, musps, saveit=peak_2_data)
	
	print, "Peak 1: ", peak_1
	print, "Peak 2: ", peak_2
end

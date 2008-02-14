function analyze_data, raw_data
	;{raw_data, status:0, timebase:0, range_a:0, range_b:0, channel_a:ptr_new(), channel_b:ptr_new()}
	retstruct={analyzed_data}
	;TODO:vpl
	
	a = min((*raw_data.channel_a), peak_min)
	a = max((*raw_data.channel_a), peak_max)
	
	print, "Peak_min: ", peak_min
	print, "Peak_max: ", peak_max

	retstruct.negative = peak_min GT peak_max ; mame negativni castici?
	retstruct.t_fall = abs(peak_min-peak_max) ; doba mezi peaky, tedy doba propadu castice valcem
	retstruct.t_peak_max = retstruct.t_fall/2 ; empiricky odhadnuta maximalni delka peaku

	;a nyni si urcime polohu nuly jako stredni hodnotu sumu
	retstruct.noise_start = (retstruct.negative?peak_min:peak_max) + retstruct.t_peak_max
	print, "Noise_start: ", retstruct.noise_start

	if (retstruct.noise_start GE N_ELEMENTS((*raw_data.channel_a))) then print, "ERROR:Noise starts after end of data" ;TODO vyhledove: nejak zlepsit zpracovani chyby
	
	noise = (*raw_data.channel_a)[retstruct.noise_start:*]
	zero = mean(noise)

	print, "Zero: ", zero

	(*raw_data.channel_a) = (*raw_data.channel_a) - zero

	retstruct.musps = tb2musps(raw_data.timebase)
	;a nyni si spocteme plochu peaku
	p1_data = 0
	p2_data = 0
	retstruct.peak_1 = integrate_peak((*raw_data.channel_a), peak_max, retstruct.t_peak_max, retstruct.musps, saveit=p1_data)
	retstruct.peak_2 = integrate_peak((*raw_data.channel_a), peak_min, retstruct.t_peak_max, retstruct.musps, saveit=p2_data)
	retstruct.peak_1_data = ptr_new(p1_data)
	retstruct.peak_2_data = ptr_new(p2_data)
	
	print, "Peak 1: ", retstruct.peak_1
	print, "Peak 2: ", retstruct.peak_2

	return, retstruct
end

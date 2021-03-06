function analyze_data, raw_data
	;{raw_data, status:0, timebase:0, range_a:0, range_b:0, channel_a:ptr_new(), channel_b:ptr_new()}
	retstruct={analyzed_data}
	;TODO:vpl

	retstruct.musps = tb2musps(raw_data.timebase)

	noiseperiod = ROUND(20000./retstruct.musps)

	a = min((*raw_data.channel_a), peak_min)
	a = max((*raw_data.channel_a), peak_max)

	;print, "Peak_min: ", peak_min
	;print, "Peak_max: ", peak_max

	retstruct.negative = peak_min GT peak_max ; mame negativni castici?
	retstruct.t_fall = abs(peak_min-peak_max) ; doba mezi peaky, tedy doba propadu castice valcem
	retstruct.t_peak_max = retstruct.t_fall*0.95 ; empiricky odhadnuta maximalni delka peaku

	retstruct.t_peak_max = noiseperiod*ceil(retstruct.t_peak_max/noiseperiod); zaokrouhlime ji nahoru na predpokladanou periodu sumu

	;a nyni si urcime polohu nuly jako stredni hodnotu sumu
	retstruct.noise_start = (retstruct.negative?peak_min:peak_max) + retstruct.t_peak_max
	;print, "Noise_start: ", retstruct.noise_start

	if (retstruct.noise_start GE N_ELEMENTS((*raw_data.channel_a))) then print, "ERROR:Noise starts after end of data" ;TODO vyhledove: nejak zlepsit zpracovani chyby

	noise = (*raw_data.channel_a)[retstruct.noise_start:*]
	zero = mean(noise)

	;print, "Zero: ", zero

	odp = odpory(0)
	ra = ra2mV(raw_data.range_a)/odp.R2_A*odp.R4_A/odp.R3_A*0.001
	(*raw_data.channel_a) = ((*raw_data.channel_a) - zero) * ra

	;a nyni si spocteme plochu peaku
	p1_data = 0
	p2_data = 0
	retstruct.peak_1 = integrate_peak((*raw_data.channel_a), peak_max, retstruct.t_peak_max, retstruct.musps, saveit=p1_data)/retstruct.musps*0.001
	retstruct.peak_2 = integrate_peak((*raw_data.channel_a), peak_min, retstruct.t_peak_max, retstruct.musps, saveit=p2_data)/retstruct.musps*0.001
	retstruct.peak_1_data = ptr_new(p1_data)
	retstruct.peak_2_data = ptr_new(p2_data)

	if raw_data.channel_b  then begin
		if retstruct.negative then begin
			a = max((*raw_data.channel_b), peak_B)
		end else begin
			a = min((*raw_data.channel_b), peak_B)
		end
		noise_start = peak_B - 0.3* retstruct.t_peak_max
		noise = (*raw_data.channel_b)[0:noise_start]
		zero = mean(noise)
		ra = ra2mV(raw_data.range_b)/odp.R2_B*odp.R4_B/odp.R3_B*0.001
		(*raw_data.channel_b) = ((*raw_data.channel_b) - zero)*ra

		p3_data = 0
		retstruct.peak_3 = abs(integrate_peak((*raw_data.channel_b), peak_B, retstruct.t_peak_max, retstruct.musps, saveit=p3_data)/retstruct.musps*0.001)
		retstruct.peak_3_data = ptr_new(p3_data)
	endif

	;print, "Peak 1: ", retstruct.peak_1
	;print, "Peak 2: ", retstruct.peak_2

	return, retstruct
end


function integrate_peak, channel, peak_pos, t_peak_max, musps
	;predpokladame, ze sum je pouze 50Hz, pricemz je to slusna
	;sinusovka a tedy pokud preintegrujeme po intervalu 
	;delitelnem 20 ns, pak je vysledna plocha zanesena sumem rovna nule
	;
	;dale predpokladame, ze peak ma extrem zhruba v polovine delky
	;a celkove je dlouhy nejvice t_peak_max
	
	;dale predpokladame, ze y je jiz ve spravnych jednotkach

	tstart = peak_pos - t_peak_max
	if tstart LE 0 then tstart = 0
	lenght = CEIL(t_peak_max*musps/20.)*20/musps
	tstop = tstart + lenght
	if tstop GE N_ELEMENTS(channel) then tstop = N_ELEMENTS(channel)-1

	peak = channel[tstart:tstop]
	x = indgen(N_ELEMENTS(peak)) * musps
	retval = INT_TABULATED(x, peak)
	return, retval
end

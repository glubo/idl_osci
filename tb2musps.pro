function tb2musps, tb
	if ( tb EQ 5 ) then return, 20.
	if ( tb EQ 16 ) then return, 1000.
	print, "I dont' know this timebase, please update tb2musps with timebase ", tb
	return, 1.0
end

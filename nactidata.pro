pro nactidata, file
	common data, channel_a, range_a, timebase
	openr, fd, file, /GET_LUN
	line=''
	range_a=0
	timebase=0
	lenght=0
	while (line NE '--- begin ---') do begin
		readf, fd, line
		;TODO vyhledove: spojit do jednoho stegex
		if ( NOT stregex(line, '^[a-zA-Z1-90_]+=[a-zA-Z1-90]+', /BOOLEAN) ) then continue
		pline = stregex(line, '(^[a-zA-Z1-90_]+)=([a-zA-Z1-90]+)', /SUBEXPR, /EXTRACT)

		;zdalo se, ze case nezere stringy
		if( pline[1] EQ 'Range_A') then range_a = UINT(pline[2])
		if( pline[1] EQ 'TimeBase') then timebase = UINT(pline[2])
		if( pline[1] EQ 'Lenght') then lenght = UINT(pline[2])
	end

	print, 'Range_A=',range_a
	print, 'Lenght=',lenght
	print, 'TimeBase=',timebase
	channel_a = BYTARR(lenght)
	readu, fd, channel_a
	free_lun, fd
end

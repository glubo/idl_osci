function query_glubo, file, fileinfo
	openr, fd, file, /GET_LUN
	line=''
	readf, fd, line
	free_lun, fd

	if ( stregex(line, '^[1-90.]*$', /BOOLEAN) ) then return, 1
	return, 0
end

function load_channel, fd
	line = ''
	length = 0
	while (line NE '--- begin ---') do begin
		readf, fd, line
		;TODO vyhledove: spojit do jednoho stegex
		if ( NOT stregex(line, '^[a-zA-Z1-90_]+=[a-zA-Z1-90]+', /BOOLEAN) ) then continue
		pline = stregex(line, '(^[a-zA-Z1-90_]+)=([a-zA-Z1-90]+)', /SUBEXPR, /EXTRACT)

		if( pline[1] EQ 'Lenght') then length = UINT(pline[2])
	end
	retchannel = ptr_new(BYTARR(length))
	readu, fd, (*retchannel)
	return, retchannel
end

function load_data, file
	retstruct = {raw_data}
	;nejdrive zkusime, jestli to neni soubor generovany generatorem
	g = query_glubo(file, fileinfo) ;byl problem, ze nektere kratsi soubory se tvarili jako ascii
	r = query_ascii(file, fileinfo)
	if ( g EQ 1) then begin
		; pokud ano, nactem ho a vyplnime informace
			retstruct.timebase = 5
			retstruct.range_a = 7
			retstruct.channel_a = ptr_new(dblarr(fileinfo.lines))
			openr, fd, file, /GET_LUN
			readf, fd, (*retstruct.channel_a)
			free_lun, fd
			return, retstruct
	end


	openr, fd, file, /GET_LUN
	line=''
	retstruct.range_a=0
	retstruct.timebase=0
	length=0
	while ~EOF(fd) do begin
		readf, fd, line
		;TODO vyhledove: spojit do jednoho stegex
		if ( NOT stregex(line, '^[a-zA-Z1-90_]+=[a-zA-Z1-90]+', /BOOLEAN) ) then begin
			if( line EQ '[Channel_A]') then retstruct.channel_a = load_channel(fd)
			if( line EQ '[Channel_B]') then retstruct.channel_b = load_channel(fd)
			continue
		endif
		pline = stregex(line, '(^[a-zA-Z1-90_]+)=([a-zA-Z1-90]+)', /SUBEXPR, /EXTRACT)

		;zdalo se, ze case nezere stringy
		if( pline[1] EQ 'Range_A') then retstruct.range_a = UINT(pline[2])
		if( pline[1] EQ 'TimeBase') then retstruct.timebase = UINT(pline[2])
		if( pline[1] EQ 'Lenght') then length = UINT(pline[2])
	end

	;print, 'Range_A=',retstruct.range_a
	;print, 'Lenght=',length
	;print, 'TimeBase=',retstruct.timebase
	free_lun, fd
	return, retstruct
end

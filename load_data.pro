function query_glubo, file, fileinfo
	openr, fd, file, /GET_LUN
	line=''
	readf, fd, line
	free_lun, fd

	if ( stregex(line, '^[1-90.]*$', /BOOLEAN) ) then return, 1
	return, 0
end

function load_data, file
	retstruct = {raw_data}
	;nejdrive zkusime, jestli to neni soubor generovany generatorem
	r = query_glubo(file, fileinfo)
	if ( r EQ 1) then begin
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
	while (line NE '--- begin ---') do begin
		readf, fd, line
		;TODO vyhledove: spojit do jednoho stegex
		if ( NOT stregex(line, '^[a-zA-Z1-90_]+=[a-zA-Z1-90]+', /BOOLEAN) ) then continue
		pline = stregex(line, '(^[a-zA-Z1-90_]+)=([a-zA-Z1-90]+)', /SUBEXPR, /EXTRACT)

		;zdalo se, ze case nezere stringy
		if( pline[1] EQ 'Range_A') then retstruct.range_a = UINT(pline[2])
		if( pline[1] EQ 'TimeBase') then retstruct.timebase = UINT(pline[2])
		if( pline[1] EQ 'Lenght') then length = UINT(pline[2])
	end

	print, 'Range_A=',retstruct.range_a
	print, 'Lenght=',length
	print, 'TimeBase=',retstruct.timebase
	retstruct.channel_a = ptr_new(BYTARR(length))
	readu, fd, (*retstruct.channel_a)
	free_lun, fd
	return, retstruct
end

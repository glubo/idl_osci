function load_dir, dirname
	;pred zmenenim adresare si pridame aktualni adresar do !PATH, abychom mohli nacitat fce a pro
	pathsep = PATH_SEP(/SEARCH_PATH) 
	cd, current=pwd
	!PATH = pwd + pathsep +!PATH

	pushd, dirname

	files = file_search("*.[01-9][01-9][01-9]", COUNT=N)

	if N EQ 0 then begin
		print, "load_dir: NO DATA FILES FOUND in ", dirname
		popd
		return, retstruct
	endif

	retstruct = replicate({tfile}, N)

	FOR i = 0, n-1 DO begin
		retstruct[i].filename = files[i]
		retstruct[i].raw = load_data(files[i])
		retstruct[i].analyzed = analyze_data(retstruct[i].raw)
	endfor

	popd
	return, retstruct
end

function load_dir, dirname
	retstruct = {tfile}
	pushd, dirname
	files = file_search("*.[0-9][0-9][0-9]")
	N = N_ELEMENTS(files)
	if N = 0 then
		print, "load_dir: NO DATA FILES FOUND in ", dirname
	endif

	popd
end

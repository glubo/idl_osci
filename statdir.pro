pro statdir, dir
	dir = load_dir(dir)
	for i =0,N_ELEMENTS(dir)-1 DO begin
		print, dir[i].analyzed.peak_1
	endfor
end

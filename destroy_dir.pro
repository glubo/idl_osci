pro destroy_dir, dir
	for i=0, N_ELEMENTS(dir)-1 do begin
		destroy_analyzed_data,dir[i].analyzed
		destroy_raw_data,dir[i].raw
	end
end

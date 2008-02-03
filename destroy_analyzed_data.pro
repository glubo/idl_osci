pro destroy_analyzed_data, adat
	ptr_free, adat.peak_1_data
	adat.peak_1_data = ptr_new()
	ptr_free, adat.peak_2_data
	adat.peak_2_data = ptr_new()
end

pro destroy_raw_data, raw_data
	ptr_free, raw_data.channel_a
	raw_data.channel_a = ptr_new()
	ptr_free, raw_data.channel_b
	raw_data.channel_b = ptr_new()
end

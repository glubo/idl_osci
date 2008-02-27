function newline
	if !VERSION.OS EQ 'linux' then begin
		return, STRING(10B)
	end
	return, STRING(13B)+STRING(10B)
end

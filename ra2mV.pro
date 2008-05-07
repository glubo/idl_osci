function ra2mV, range
	case range of:
		0: return 10./32.
		1: return 20./32.
		2: return 50./32.
		3: return 100./32.
		4: return 200./32.
		5: return 500./32.
		6: return 1000./32.
		7: return 2000./32.
		8: return 5000./32.
	end
	print, "Unknown Range: ", range
	return 500./32.
end

# Bryan A. Tremblay 8Nov2016MST
# Designed for Myrrah Novus book
# Will take prime inputs for different time periods and produce a millisecond output that is also prime
# Written to make some calculations for a book I was writing

primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997]

def find_ms(y, mo, d, h, m, s, ms):
	y_ms =  y * 365 * 24 * 60 * 60 * 1000
	mo_ms = mo * 30 * 24 * 60 * 60 * 1000
	d_ms =  d * 24 * 60 * 60 * 1000
	h_ms =  h * 60 * 60 * 1000
	m_ms =  m * 60 * 1000
	s_ms =  s * 1000
	total_ms = y_ms + mo_ms + d_ms + h_ms + m_ms + s_ms + ms
	return total_ms

def is_prime(num):
	if num == 2:
		return True
	if not num & 1:
		return False
	return pow(2, num-1, num) == 1

# We'd find that 211y 3mo 5d 19h 23m 37s 691ms works
for prime in primes:
	total = find_ms(211, 3, 5, 19, 23, 37, prime)
	if is_prime(total):
		print(prime)
		print(total)

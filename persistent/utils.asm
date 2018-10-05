	def	memcpy
memcpy	
L1		movb	*r2+,*r1+
		dec		r3
		jne		L1
		b		*r11

	def memset
memset
L2		movb	r2,*r1+
		dec		r3
		jne		L1
		b		*r11

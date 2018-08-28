# ts.s file, supplied by Dr. K.C. Wang:
       
	.global getebp
getebp:
        movl %ebp, %eax
        ret

#Code supplied by Dr. K.C. Wang for assignment purposes. 

# ts.s file:
       .global getebp
getebp:
        movl %ebp, %eax
        ret

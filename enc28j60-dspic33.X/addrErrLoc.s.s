
	.global 	_getErrLoc
	.section .text
_getErrLoc:
        mov    w15,w0
        sub    w0,#26
        return


	.global 	_MemReadLatch
	.section .text
_MemReadLatch:
	mov W0,TBLPAG
	tblrdl [W1],W0
	tblrdh [W1],W1
	return

.section *,code
.global _RtosKernelContext
_RtosKernelContext:
		PUSH	SR
		PUSH	W0
		MOV		#32, W0
		MOV		W0, SR
		PUSH	W1
		PUSH.D	W2
		PUSH.D	W4
		PUSH.D	W6
		PUSH.D 	W8
		PUSH.D 	W10
		PUSH.D	W12
		PUSH	W14
		PUSH	RCOUNT
		PUSH	TBLPAG

		PUSH	CORCON
		PUSH	PSVPAG
		#MOV		_uxCriticalNesting, W0
		PUSH	W0
		MOV		W15, _RtosKernelStackPos

		call 	_RtosTaskChange

		MOV		_RtosKernelStackPos, W15
		POP		W0
		#MOV		W0, _uxCriticalNesting
		POP		PSVPAG
		POP		CORCON
		POP		TBLPAG
		POP		RCOUNT
		POP		W14
		POP.D	W12
		POP.D	W10
		POP.D	W8
		POP.D	W6
		POP.D	W4
		POP.D	W2
		POP.D	W0
		POP		SR

        return

.section *,code
.global _RtosKernelStart
_RtosKernelStart:

		MOV		_RtosKernelStackPos, W15
		POP		W0
		#MOV		W0, _uxCriticalNesting
		POP		PSVPAG
		POP		CORCON
		POP		TBLPAG
		POP		RCOUNT
		POP		W14
		POP.D	W12
		POP.D	W10
		POP.D	W8
		POP.D	W6
		POP.D	W4
		POP.D	W2
		POP.D	W0
		POP		SR

        return

        .end


.extern spiReadByte
.extern spiWriteByte

.section *,code
.global spiReadByteS
spiReadByteS:
			push W0
			clr W0
			bclr PORTC, #8
/* ------------------------------------- */
			bset PORTC, #7
			btsc PORTC, #9 /* Read, skip if 0 */
			bset W0, #7
			bclr PORTC, #7
			
			
/* ------------------------------------- */
			bset PORTC, #7
			btsc PORTC, #9 /* Read, skip if 0 */
			bset W0, #6
			bclr PORTC, #7
			
			
/* ------------------------------------- */
			bset PORTC, #7
			btsc PORTC, #9 /* Read, skip if 0 */
			bset W0, #5
			bclr PORTC, #7
			
			
/* ------------------------------------- */
			bset PORTC, #7
			btsc PORTC, #9 /* Read, skip if 0 */
			bset W0, #4
			bclr PORTC, #7
			
			
/* ------------------------------------- */
			bset PORTC, #7
			btsc PORTC, #9 /* Read, skip if 0 */
			bset W0, #3
			bclr PORTC, #7
			
			
/* ------------------------------------- */
			bset PORTC, #7
			btsc PORTC, #9 /* Read, skip if 0 */
			bset W0, #2
			bclr PORTC, #7
			
			
/* ------------------------------------- */
			bset PORTC, #7
			btsc PORTC, #9 /* Read, skip if 0 */
			bset W0, #1
			bclr PORTC, #7
			
			
/* ------------------------------------- */
			bset PORTC, #7
			btsc PORTC, #9 /* Read, skip if 0 */
			bset W0, #0
			bclr PORTC, #7
			
			

			mov W0, _spiReadByte
			pop W0
			return

.section *,code
.global spiWriteByteS
spiWriteByteS:
			push W0
			mov _spiWriteByte, W0
/* ------------------------------------- */
			btsc W0, #7
			bset PORTC, #8
			btss W0, #7
			bclr PORTC, #8

			bclr PORTC, #7 /* 7 */
			bset PORTC, #7
/* ------------------------------------- */
			btsc W0, #6
			bset PORTC, #8
			btss W0, #6
			bclr PORTC, #8

			bclr PORTC, #7 /* 7 */
			bset PORTC, #7
/* ------------------------------------- */
			btsc W0, #5
			bset PORTC, #8
			btss W0, #5
			bclr PORTC, #8

			bclr PORTC, #7 /* 7 */
			bset PORTC, #7
/* ------------------------------------- */
			btsc W0, #4
			bset PORTC, #8
			btss W0, #4
			bclr PORTC, #8

			bclr PORTC, #7 /* 7 */
			bset PORTC, #7
/* ------------------------------------- */
			btsc W0, #3
			bset PORTC, #8
			btss W0, #3
			bclr PORTC, #8

			bclr PORTC, #7 /* 7 */
			bset PORTC, #7
/* ------------------------------------- */
			btsc W0, #2
			bset PORTC, #8
			btss W0, #2
			bclr PORTC, #8

			bclr PORTC, #7 /* 7 */
			bset PORTC, #7
/* ------------------------------------- */
			btsc W0, #1
			bset PORTC, #8
			btss W0, #1
			bclr PORTC, #8

			bclr PORTC, #7 /* 7 */
			bset PORTC, #7
/* ------------------------------------- */
			btsc W0, #0
			bset PORTC, #8
			btss W0, #0
			bclr PORTC, #8

			bclr PORTC, #7 /* 7 */
			bset PORTC, #7
			bclr PORTC, #7 /* 7 */
			pop W0
			return

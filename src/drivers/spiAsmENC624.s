
.extern spiReadByte
.extern spiWriteByte

.section *,code
.global spiENC624ReadByteS
spiENC624ReadByteS:
			push W0
			clr W0
			bclr PORTC, #0
/* ------------------------------------- */
			bset PORTC, #1
			btsc PORTB, #3 /* Read, skip if 0 */
			bset W0, #7
			bclr PORTC, #1
			
			
/* ------------------------------------- */
			bset PORTC, #1
			btsc PORTB, #3 /* Read, skip if 0 */
			bset W0, #6
			bclr PORTC, #1
			
			
/* ------------------------------------- */
			bset PORTC, #1
			btsc PORTB, #3 /* Read, skip if 0 */
			bset W0, #5
			bclr PORTC, #1
			
			
/* ------------------------------------- */
			bset PORTC, #1
			btsc PORTB, #3 /* Read, skip if 0 */
			bset W0, #4
			bclr PORTC, #1
			
			
/* ------------------------------------- */
			bset PORTC, #1
			btsc PORTB, #3 /* Read, skip if 0 */
			bset W0, #3
			bclr PORTC, #1
			
			
/* ------------------------------------- */
			bset PORTC, #1
			btsc PORTB, #3 /* Read, skip if 0 */
			bset W0, #2
			bclr PORTC, #1
			
			
/* ------------------------------------- */
			bset PORTC, #1
			btsc PORTB, #3 /* Read, skip if 0 */
			bset W0, #1
			bclr PORTC, #1
			
			
/* ------------------------------------- */
			bset PORTC, #1
			btsc PORTB, #3 /* Read, skip if 0 */
			bset W0, #0
			bclr PORTC, #1
			
			

			mov W0, _spiReadByte
			pop W0
			return

.section *,code
.global spiENC624WriteByteS
spiENC624WriteByteS:
			push W0
			mov _spiWriteByte, W0
/* ------------------------------------- */
			btsc W0, #7
			bset PORTC, #0
			btss W0, #7
			bclr PORTC, #0

			bclr PORTC, #1 /* 7 */
			bset PORTC, #1
/* ------------------------------------- */
			btsc W0, #6
			bset PORTC, #0
			btss W0, #6
			bclr PORTC, #0

			bclr PORTC, #1 /* 7 */
			bset PORTC, #1
/* ------------------------------------- */
			btsc W0, #5
			bset PORTC, #0
			btss W0, #5
			bclr PORTC, #0

			bclr PORTC, #1 /* 7 */
			bset PORTC, #1
/* ------------------------------------- */
			btsc W0, #4
			bset PORTC, #0
			btss W0, #4
			bclr PORTC, #0

			bclr PORTC, #1 /* 7 */
			bset PORTC, #1
/* ------------------------------------- */
			btsc W0, #3
			bset PORTC, #0
			btss W0, #3
			bclr PORTC, #0

			bclr PORTC, #1 /* 7 */
			bset PORTC, #1
/* ------------------------------------- */
			btsc W0, #2
			bset PORTC, #0
			btss W0, #2
			bclr PORTC, #0

			bclr PORTC, #1 /* 7 */
			bset PORTC, #1
/* ------------------------------------- */
			btsc W0, #1
			bset PORTC, #0
			btss W0, #1
			bclr PORTC, #0

			bclr PORTC, #1 /* 7 */
			bset PORTC, #1
/* ------------------------------------- */
			btsc W0, #0
			bset PORTC, #0
			btss W0, #0
			bclr PORTC, #0

			bclr PORTC, #1 /* 7 */
			bset PORTC, #1
			bclr PORTC, #1 /* 7 */
			pop W0
			return

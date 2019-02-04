/*	MACHINE.H		09/29/84-09/29/84	Lee Lorenzen	*/



						/* coerce short ptr to	*/
						/*   low word  of long	*/
#define LW(x) ( (LONG)((UWORD)(x)) )

						/* coerce short ptr to	*/
						/*   high word  of long	*/
#define HW(x) ((LONG)((UWORD)x) << 16)
						/* in LARGE.A86		*/
						/* return length of 	*/
						/*   string pointed at	*/
						/*   by long pointer	*/
EXTERN WORD	LSTRLEN();
						/* return long address	*/
						/*   of short ptr	*/
EXTERN LONG	ADDR();
						/* return long address	*/
						/*   of the data seg	*/
EXTERN LONG	LLDS();
						/* return long address	*/
						/*   of the code seg	*/
EXTERN LONG	LLCS();
						/* copy n words from	*/
						/*   src long ptr to	*/
						/*   dst long ptr i.e.,	*/
						/*   LWCOPY(dlp, slp, n)*/
EXTERN WORD	LWCOPY();
						/* copy n words from	*/
						/*   src long ptr to	*/
						/*   dst long ptr i.e.,	*/
						/*   LBCOPY(dlp, slp, n)*/
EXTERN BYTE	LBCOPY();
						/* return a single byte	*/
						/*   pointed at by long	*/
						/*   ptr		*/
EXTERN BYTE	LBGET();
						/* set a single byte	*/
						/*   pointed at by long	*/
						/*   ptr, LBSET(lp, bt)	*/
EXTERN BYTE	LBSET();
						/* return a single word	*/
						/*   pointed at by long	*/
						/*   ptr		*/
EXTERN WORD	LWGET();
						/* set a single word	*/
						/*   pointed at by long	*/
						/*   ptr, LWSET(lp, bt)	*/
EXTERN WORD	LWSET();
						/* return a single long	*/
						/*   pointed at by long	*/
						/*   ptr		*/
EXTERN LONG	LLGET();
						/* set a single long	*/
						/*   pointed at by long	*/
						/*   ptr, LLSET(lp, bt)	*/
EXTERN LONG	LLSET();
						/* return low word of	*/
						/*   a long value	*/
#define LLOWD(x) ((UWORD)(x))
						/* return high word of	*/
						/*   a long value	*/
#define LHIWD(x) ((UWORD)(x >> 16))
						/* return 0th byte of	*/
						/*   a long value given	*/
						/*   a short pointer to	*/
						/*   the long value	*/
#define LBYTE0(x) (*x)
						/* return 1st byte of	*/
						/*   a long value given	*/
						/*   a short pointer to	*/
						/*   the long value	*/
#define LBYTE1(x) (*(x+1))
						/* return 2nd byte of	*/
						/*   a long value given	*/
						/*   a short pointer to	*/
						/*   the long value	*/
#define LBYTE2(x) (*(x+2))
						/* return 3rd byte of	*/
						/*   a long value given	*/
						/*   a short pointer to	*/
						/*   the long value	*/
#define LBYTE3(x) (*(x+3))


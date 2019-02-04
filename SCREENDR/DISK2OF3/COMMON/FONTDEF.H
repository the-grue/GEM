/* fh_flags   */
#define	DEFAULT 1	/* this is the default font (face and size) */
#define	HORZ_OFF  2	/* there are left and right offset tables */
#define	STDFORM  4	/* is the font in standard format */
#define MONOSPACE 8     /* mono-spaced font */

/* style bits */
#define	THICKEN	0x1
#define	LIGHT	0x2
#define	SKEW	0x4
#define	UNDER	0x8
#define	OUTLINE 0x10
#define	SHADOW	0x20
#define	ROTATE	0x0c0
#define	SCALE	0x100

struct font_head {		/* descibes a font */
    UWORD font_id;
    WORD point;
    BYTE name[32];
    UWORD first_ade;
    UWORD last_ade;
    UWORD top;
    UWORD ascent;
    UWORD half;
    UWORD descent;		
    UWORD bottom;
    UWORD max_char_width;
    UWORD max_cell_width;
    UWORD left_offset;		/* amount character slants left when skewed */
    UWORD right_offset;		/* amount character slants right */
    UWORD thicken;		/* number of pixels to smear */
    UWORD ul_size;		/* size of the underline */
    UWORD lighten;		/* mask to and with to lighten  */
    UWORD skew;			/* mask for skewing */
    UWORD flags;		    

    UBYTE *hor_table;	  	/* horizontal offsets */
    UWORD *off_table;		/* character offsets  */
    UWORD *dat_table;		/* character definitions */
    UWORD form_width;
    UWORD form_height;

    struct font_head *next_font;/* pointer to next font */
    struct font_head *next_sect; /* pointer to next segment of font */	
    WORD lfu_low;		/* least frequently used counter for font */
    WORD lfu_high;
    WORD *file_off;		/* gdos data for offset into font file */
    WORD data_siz;		/* gdos data for size of font data */
    WORD reserve[8];		/* reserved data area for future use */
};

/*************************************************************
 * vfntobj.c
 * create a ".A86" file that can be rasmed into
 * a ".obj" file.  This is for the Ventura style
 * font.	 
 *
 * 11-16-87 jn
 *	MOdified to always include the 8 point font file.
 *	Also all the details are now taken care of.
 *
 *	USAGE: vfntcnv <ascii data file> <header file> <outfile>
 **************************************************************/

#include <hcportab.h>
#include <stdio.h>
#include <ctype.h>

#define	CHRBM_SIZE 32

FILE	*fopen();
BYTE	*fgets();

EXTERN VOID exit() ;
EXTERN WORD strlen() ;
EXTERN UBYTE *calloc() ;

VOID error() ;
VOID main() ;
VOID emit_cmt() ;
VOID emit_hdr() ;
VOID emit_wtbl() ;
VOID emit_fnt() ;
VOID init_hdr() ;
VOID open_files() ;
VOID open_8pt() ;
VOID open_10pt() ;
VOID mkchrbm() ;
VOID mkfntbm() ;

WORD	font_height,
	form_width,
	font_count,
	char_width ;

FILE	*font_file, *out_file, *hdr_file ;

UBYTE	*font_buff ;
UBYTE	chrbm[CHRBM_SIZE] ;
WORD	chrbmw ;

UBYTE	pfont_name[15];
UBYTE	hdr_name[15];
UBYTE	fname[33] ;
UBYTE	pfont_line[128];
UBYTE	pbuffer[128];

UBYTE	bitmsk[] = { 0x80, 0x40, 0x20, 0x10,
		     0x08, 0x04, 0x02, 0x01 } ;

UWORD		offsets[257];

UBYTE *hdrlbs[] = {
	"Font ID",
	"Font Point size",
	"Font Name, pad",
	"Lowest ADE",
	"Highest ADE",
	"Top line distance",
	"Ascent line distance",
	"Half line distance",
	"Desent line distance",
	"Bottom line distance",
	"Width of widest char in font",
	"Width of widest char cell in face",
	"Left offset",
	"Right offset",
	"Thickening",
	"Underline size",
	"Lightening mask",
	"Skewing mask",
	"Flags",
	"Long ptr to horz offset table",
	"Long ptr to char offset table",
	"Long ptr to font data",
	"Form width",
	"Form height",
	"Long ptr, next font header",
	"Long ptr, next section of font",
	"Least frequently used counter low word",
	"Least frequently used counter high word",
	"LONG word, offset in file to offset table",
	"Data Size, length of horz offset table",
	"7 reserved words",
	"Device Flags",
	"32 byte escape sequence buffer",
	"" } ;

UBYTE	*restbl[] = {
	"rd 1",
	"rw 1",
	"rw 1",
	"rd 1",
	"rw 1",
	"rw 7",
	"rw 1",
	"rb 32",
	"" } ;

UWORD hdrval[] = {
	1,	/* Font ID */
	6,	/* Font Point size */
	32,	/* Font Name, number of bytes to reserver */
	32,	/* Lowest ADE */
	127,	/* Highest ADE */
	5,	/* Top line distance */
	5,	/* Ascent line distance */
	4,	/* Half line distance */
	1,	/* Desent line distance */
	1,	/* Bottom line distance */
	5,	/* Width of widest char in font */
	6,	/* Width of widest char cell in face */
	1,	/* Left offset */
	0,	/* Right offset */
	1,	/* Thickening */
	1,	/* Underline size */
	0x5555,	/* Lightening mask */
	0x5555,	/* Skewing mask */
	0x08,	/* Flags */
	0,	/* Long ptr to horz offset table */
	0,	/* Long ptr to char offset table */
	0,	/* Long ptr to font data */
	72,	/* Form width */
	6,	/* Form height */
	0,	/* Long ptr to next font header */
	 } ;






/*--------------
 * main ....
 *------------*/
VOID main(argc, argv)
WORD	argc ;
UBYTE	**argv ;
{
UWORD	i, j, c ;
UWORD	font_id, first_char;
UWORD	nitems;
BYTE	*pline;
UWORD	bytes_pl,
	words_pl,
	font_size ;
LONG	hdr_8pt, wtbl_8pt, font_8pt, end_8pt,
	hdr_10pt, wtbl_10pt, font_10pt, end_10pt ;
FILE	*in8pt_file, *in10pt_file ;


	printf("Font Conversion Utility\n") ;
	printf("Date: 11-16-87\n\n") ;

	printf("Processing 8 point system font\n") ;

	open_8pt(&out_file, &font_file, &hdr_file) ;

	pline = fgets(pbuffer, 82, font_file);
	nitems = sscanf(pline, "%s", pfont_name);

	/* get count of items in font */

	pline = fgets(pbuffer, 82, font_file);
	nitems = sscanf(pline, "%d", &font_count);

	/* get height of font	*/

	pline = fgets(pbuffer, 82, font_file);
	nitems = sscanf(pline, "%d", &font_height);

	/* get width of entire strike form */

	pline = fgets(pbuffer, 82, font_file);
	nitems = sscanf(pline, "%d", &form_width);

	char_width = form_width / font_count ;

	bytes_pl = form_width / 8 ;
	words_pl = (bytes_pl + 1) / 2 ;
	bytes_pl = words_pl * 2 ;
	font_size = bytes_pl * (font_height) ;

	mkfntbm(font_size, bytes_pl) ;

	init_hdr() ;
	rewind(out_file) ;
	hdr_8pt = ftell(out_file) ;
	emit_cmt() ;
	emit_hdr(TRUE, argv[1]) ;
	wtbl_8pt = ftell(out_file) ;
	emit_wtbl() ;
	font_8pt = ftell(out_file) ;
	emit_fnt(font_size, bytes_pl) ;
	end_8pt =  ftell(out_file) ;

	/* close source file */

	fprintf(out_file,"\032");

	fclose(out_file) ;
	fclose(font_file) ;
	fclose(hdr_file) ;

	printf("Processing 10 point system font\n") ;

	open_10pt(argc, argv, &out_file, &font_file, &hdr_file) ;

	pline = fgets(pbuffer, 82, font_file);
	nitems = sscanf(pline, "%s", pfont_name);

	/* get count of items in font */

	pline = fgets(pbuffer, 82, font_file);
	nitems = sscanf(pline, "%d", &font_count);

	/* get height of font	*/

	pline = fgets(pbuffer, 82, font_file);
	nitems = sscanf(pline, "%d", &font_height);

	/* get width of entire strike form */

	pline = fgets(pbuffer, 82, font_file);
	nitems = sscanf(pline, "%d", &form_width);

	char_width = form_width / font_count ;

	bytes_pl = form_width / 8 ;
	words_pl = (bytes_pl + 1) / 2 ;
	bytes_pl = words_pl * 2 ;
	font_size = bytes_pl * (font_height) ;

	mkfntbm(font_size, bytes_pl) ;
	init_hdr() ;
	rewind(out_file) ;
	hdr_10pt = ftell(out_file) ;
	emit_cmt() ;
	emit_hdr(FALSE, "") ;
	wtbl_10pt = ftell(out_file) ;
	emit_wtbl() ;
	font_10pt = ftell(out_file) ;
	emit_fnt(font_size, bytes_pl) ;
	end_10pt =  ftell(out_file) ;

	/* close source file */

	fprintf(out_file,"\032");

	fclose(out_file) ;
	fclose(font_file) ;
	fclose(hdr_file) ;

	/* open both temp files as input,
	   and the output file */

	printf("Merging 8 and 10 point files\n") ;

	in8pt_file = fopen("8point.tmp", "rb") ;
	in10pt_file = fopen("10point.tmp", "rb") ;

	rewind(in8pt_file) ;
	rewind(in10pt_file) ;

	out_file = fopen(argv[3], "w") ;
	if (out_file == NULLPTR)
	  error("Output file open error: %s\n", argv[3]) ;

	fprintf(out_file, "\n\tdseg\n\n") ;
	fprintf(out_file, "public\tfirst\n\n") ;

	j = wtbl_8pt - hdr_8pt ;
	fseek(in8pt_file, hdr_8pt, SEEK_SET) ;
	for ( i = 0 ; i < j ; i++) {
	  c = fgetc(in8pt_file) ;
	  c &= 0xff ;
	  fputc(c, out_file) ;
	}

	j = wtbl_10pt - hdr_10pt ;
	fseek(in10pt_file, hdr_10pt, SEEK_SET) ;
	for ( i = 0 ; i < j ; i++) {
	  c = fgetc(in10pt_file) ;
	  c &= 0xff ;
	  fputc(c, out_file) ;
	}

	j = font_8pt - wtbl_8pt ;
	fseek(in8pt_file, wtbl_8pt, SEEK_SET) ;
	for ( i = 0 ; i < j ; i++) {
	  c = fgetc(in8pt_file) ;
	  c &= 0xff ;
	  fputc(c, out_file) ;
	}

	j = font_10pt - wtbl_10pt ;
	fseek(in10pt_file, wtbl_10pt, SEEK_SET) ;
	for ( i = 0 ; i < j ; i++) {
	  c = fgetc(in10pt_file) ;
	  c &= 0xff ;
	  fputc(c, out_file) ;
	}

	j = end_8pt - font_8pt ;
	fseek(in8pt_file, font_8pt, SEEK_SET) ;
	for ( i = 0 ; i < j ; i++) {
	  c = fgetc(in8pt_file) ;
	  c &= 0xff ;
	  fputc(c, out_file) ;
	}

	j = end_10pt - font_10pt ;
	fseek(in10pt_file, font_10pt, SEEK_SET) ;
	for ( i = 0 ; i < j ; i++) {
	  c = fgetc(in10pt_file) ;
	  c &= 0xff ;
	  fputc(c, out_file) ;
	}

	fprintf(out_file,"\032");

	fclose(in8pt_file) ;
	fclose(in10pt_file) ;
	fclose(out_file) ;

	exit(0) ;


}



/*----------------------------
 * emit the comment header....
 *---------------------------*/
VOID emit_cmt()
{
FILE	*rfp ;

	fprintf(out_file, "eject\n\n") ;
	fprintf(out_file, "\t;----------------------------------------\n") ;
	fprintf(out_file,
	  "\t;                    Font Name: %s\n", pfont_name) ;
	fprintf(out_file,
	  "\t; Number of characters in font: %d\n", font_count) ;
	fprintf(out_file,
	  "\t; Height of characters in font: %d\n", font_height) ;
	fprintf(out_file,
	  "\t;   Width of character in font: %d\n", char_width) ;
	fprintf(out_file, "\t;----------------------------------------\n\n") ;

  #if 0
	rfp = fopen("rules.dat", "r") ;
	if (rfp == NULLPTR) {
	  fprintf(stderr, "Rules file not found\n") ;
	  return ;
	}

	while(!feof(rfp)) {
	  fgets(pfont_line, 82, rfp) ;
	  if (!feof(rfp))
	    fprintf(out_file, "\t; %s", pfont_line) ;
	}

	fclose(rfp) ;
#endif
}



/*---------------------
 * emit the font header
 *-------------------*/
VOID emit_hdr(pt8, pt10_name)
WORD	pt8 ;
UBYTE	*pt10_name ;
{
WORD	i, j, li ;
UBYTE	name[20] ;

	fprintf(out_file, "\t;----------------------------------------\n") ;
	fprintf(out_file, "\t; Ventura Style Font header\n") ;
	if (pt8 == TRUE) {
	  fprintf(out_file, "\t; 8 point system font header\n") ;
	}
	else {
	  fprintf(out_file, "\t; 10 point system font header\n") ;
	}
	fprintf(out_file, "\t;----------------------------------------\n\n") ;

	/* At this time just emit a default header,
	   At a latter date use the font header file
	   to construct specific header */

	if (pt8 == TRUE) {
	  fprintf(out_file, "first:\n") ;
	}
	fprintf(out_file, "%s_hdr:\n\n", pfont_name) ;

	for ( li = 0 ; li < 2 ; li++) {
	  fprintf(out_file, "\tdw\t%d\t\t; %s\n", hdrval[li], hdrlbs[li]) ;
	}

	i = strlen(fname) ;
	fprintf(out_file, "\tdb\t'%s'\n", fname ) ;
	fprintf(out_file, "\trb\t%d\t\t; %s\n", 32 - i, hdrlbs[li++]) ;

	for ( ; li <  16 ; li++) {
	  fprintf(out_file, "\tdw\t%d\t\t; %s\n", hdrval[li], hdrlbs[li]) ;
	}

	for ( ; li <  19 ; li++) {
	  fprintf(out_file, "\tdw\t%04xh\t\t; %s\n", hdrval[li], hdrlbs[li]) ;
	}

	fprintf(out_file, "\tdw\t%d, %d\t\t; %s\n", 
	  hdrval[li], hdrval[li], hdrlbs[li++]) ;

	fprintf(out_file, "\tdw\toffset %s_wtbl  ; %s\n",
	  pfont_name, hdrlbs[li++]) ;
	fprintf(out_file, "\tdw\tseg %s_wtbl\n", pfont_name) ;

	fprintf(out_file, "\tdw\toffset %s_data  ; %s\n",
	  pfont_name, hdrlbs[li++]) ;
	fprintf(out_file, "\tdw\tseg %s_data \n", pfont_name) ;

	for ( ; li <  24 ; li++) {
	  fprintf(out_file, "\tdw\t%d\t\t; %s\n", hdrval[li], hdrlbs[li]) ;
	}

	if (pt8 == TRUE) {
	  j = strlen(pt10_name) ;
	  for (i = 0 ; i < j ; i++ ) {
	    if (pt10_name[i] == '.') {
	      name[i] = 0 ;
	      break ;
	    }
	    else {
	      name[i] = pt10_name[i] ;
	    }
	  }
	  fprintf(out_file, "\tdw offset fnt%s_hdr\t; %s\n",
	    name, hdrlbs[li] ) ;
	  fprintf(out_file, "\tdw seg fnt%s_hdr\n", name) ;
	  li++ ;
	}
	else {
	  fprintf(out_file, "\tdw\t%d, %d\t\t; %s\n", 
	    hdrval[li], hdrval[li], hdrlbs[li]) ;
	  li++ ;
	}

	fprintf(out_file, "\t\t;***** Ventura specific extensions *****\n") ;

	for ( i = 0 ; i < 8 ; i++ )
	  fprintf(out_file, "\t%s\t\t\t; %s \n", restbl[i], hdrlbs[li++]) ;

	fprintf(out_file, "\n\n") ;

}



/*---------------------
 * emit the width table
 *---------------------*/
VOID emit_wtbl()
{
WORD	i ;


	fprintf(out_file, "eject\n") ;
	fprintf(out_file, "\n\n") ;

	fprintf(out_file, "\t;----------------------------------------\n") ;
	fprintf(out_file, "\t; Font width table\n") ;
	fprintf(out_file, "\t;----------------------------------------\n\n") ;

	fprintf(out_file, "%s_wtbl:\n\n", pfont_name) ;

	for ( i = 0 ; i < font_count + 1 ; i++ ) {
	  if (i % 8 == 0) {
	    fprintf(out_file, "\n\tdw\t%d", i * char_width) ;
	  }
	  else {
	    fprintf(out_file, ", %d", i * char_width) ;
	  }
	}
	fprintf(out_file, "\n\n") ;

#if 0
	for ( i = 0 ; i < font_count + 1 ; i++ ) {
	  fprintf(out_file, "\tdw\t%d\t\t; ADE %03xh, ascii: %c\n",
	    i * char_width, i + hdrval[3], (isalnum(i)) ? i : '.') ;
	}
#endif

}




/*------------------------------------------
 * emit the .A86 data structure for the font
 * put a comment out for each line.
 *-----------------------------------------*/
VOID emit_fnt(font_size, bytes_pl)
WORD	font_size, bytes_pl ;
{
WORD	i ;

	fprintf(out_file, "eject\n") ;
	fprintf(out_file, "\n\n") ;
	
	fprintf(out_file, "\t;----------------------------------------\n") ;
	fprintf(out_file, "\t; Font data\n") ;
	fprintf(out_file, "\t;----------------------------------------\n\n") ;

	fprintf(out_file, "%s_data:\n\n", pfont_name) ;

	for (i = 0 ; i < font_size ; i++ ) {
	  if (i % bytes_pl == 0) {
	    fprintf(out_file,"\t; Line %d\n", i / bytes_pl) ;
	  }
	  if (i % 8 == 0) {
	    fprintf(out_file,"\tdb\t") ;
	  }
	  if (i % 8 != 7) {
	    fprintf(out_file, "%03xh, ", font_buff[i]) ;
	  }
	  else {
	    fprintf(out_file, "%03xh\n", font_buff[i]) ;
	  }
	}
}



/*--------------------------
 * init the font header info
 *--------------------------*/
VOID init_hdr()
{
WORD	i, ret ;

	for (i = 0 ; i < 2 ; i++) {
	  ret = fscanf(hdr_file, "%d", &hdrval[i]) ;
	  if (ret == -1) {
	    printf("Error on item: %d of header file\n", i) ;
	  }
	}

	ret = fscanf(hdr_file, "%s", fname) ;
	if (ret == -1) {
	  printf("Error on name in header file\n") ;
	}
	i = strlen(fname) ;

	hdrval[2] = 32 - i ;

	for ( i = 3 ; i < 16 ; i++ ) {
	  fscanf(hdr_file, "%d", &hdrval[i]) ;
	  if (ret == -1) {
	    printf("Error on item: %d of header file\n", i) ;
	  }
	}

	for ( i = 16 ; i < 19 ; i++ ) {
	  fscanf(hdr_file, "%x", &hdrval[i]) ;
	  if (ret == -1) {
	    printf("Error on item: %d of header file\n", i) ;
	  }
	}

	for ( i = 22 ; i < 24 ; i++ ) {
	  fscanf(hdr_file, "%d", &hdrval[i]) ;
	  if (ret == -1) {
	    printf("Error on item: %d of header file\n", i) ;
	  }
	}

}




/*------------------
 * make font bit map
 *-----------------*/
VOID mkfntbm(font_size, bytes_pl) 
WORD	font_size ;
{
UWORD	srcx,
	destx,
	y,
	cur_char ;


	/* allocate memory for the font BM */

	font_buff = (UBYTE *) calloc(font_size + 16, sizeof(BYTE)) ;
	if (font_buff == NULLPTR) {
	  error("Memory Allocation error: %d bytes\n", font_size) ;
	}
	
	/* Initialize the dest X.
	   This will be common over all the chars */

	destx = 0 ;

	for (cur_char = 0 ; cur_char < font_count ; cur_char++) {
	  mkchrbm() ;
	  for ( srcx = 0 ; srcx < char_width ; srcx++, destx++ ) {
	    for ( y = 0 ; y < font_height ; y++ ) {
	      if (chrbm[(y * chrbmw) + (srcx >> 3)] & bitmsk[srcx & 0x07]) {
	        font_buff[(y*bytes_pl)+(destx >> 3)] |= bitmsk[destx & 0x07];
	      }
	    }
	  }
	}
}



/*-------------------------------------
 * make a character bit map
 * return a left justified char bit map
 * in chrbm
 *-----------------------------------*/
VOID mkchrbm()
{
WORD	i, x, y ;

	/* the char bm is assumed to 16 bits wide.
	   It's max height is 16 pixels.
	   Clear the char bitmap. */
	
	chrbmw = 2 ;

	for ( i = 0 ; i < CHRBM_SIZE ; i++ )
	  chrbm[i] = 0 ;

	/* read the first ID.
	   This will go into the bit bucket */

	fgets(pfont_line, 82, font_file) ;

	for ( y = 0 ; y < font_height ; y++ ) {
	  fgets(pfont_line, 82, font_file) ;
	  for ( x = 0 ; x < char_width ; x++) {
	    if (pfont_line[x] == '1') {
	      chrbm[(y * 2) + (x / 8)] |= bitmsk[x % 8] ;
	    }
	  }
	}

	


}


/*--------------------------------------------
 * open the 10 point font and the output files
 *------------------------------------------*/
VOID open_10pt(argc, argv, out_file, font_file, hdr_file)
WORD	argc ;
UBYTE	**argv ;
FILE	**out_file, **font_file, **hdr_file ;
{

	/* open font file */

	*font_file = fopen(argv[1],"r");
	if (*font_file == NULLPTR)
	  error("Font input file open error: %s\n", argv[1]) ;

	/* open header file */

	*hdr_name = argv[2] ;
	*hdr_file = fopen(argv[2],"r");
	if (*hdr_file == NULLPTR)
	  error("Header input file open error: %s\n", argv[2]) ;

	/* open output file */

	*out_file = fopen("10point.tmp", "w") ;
	if (*out_file == NULLPTR)
	  error("Output file open error: %s\n", "10point.tmp") ;

}

/*-------------------------------------------
 * open the 8 point font and the output files
 *------------------------------------------*/
VOID open_8pt(out_file, font_file, hdr_file)
FILE	**out_file, **font_file, **hdr_file ;
{

	/* open font file */

	*font_file = fopen("6x6.ful","r");
	if (*font_file == NULLPTR)
	  error("Font input file open error: %s\n", "6x6.ful") ;

	/* open header file */

	*hdr_name = "6x6ful.hdr" ;
	*hdr_file = fopen("6x6ful.hdr","r");
	if (*hdr_file == NULLPTR)
	  error("Header input file open error: %s\n", "6x6ful.hdr") ;

	/* open output file */

	*out_file = fopen("8point.tmp", "w") ;
	if (*out_file == NULLPTR)
	  error("Output file open error: %s\n", "8point.tmp") ;

}

/*----------------
 * error handler.
 *---------------*/
VOID error(s, p)
BYTE	*s, *p ;
{
	printf(s, p) ;
	exit() ;

}

/************************************************************************/
/*   File:   RSCREATE.C                                                 */
/************************************************************************/
/*                                                                      */
/*           GGGGG        EEEEEEEE     MM      MM                       */
/*         GG             EE           MMMM  MMMM                       */
/*         GG   GGG       EEEEE        MM  MM  MM                       */
/*         GG   GG        EE           MM      MM                       */
/*           GGGGG        EEEEEEEE     MM      MM                       */
/*                                                                      */
/************************************************************************/
/*                                                                      */
/*           +--------------------------+                               */
/*           | Digital Research, Inc.   |                               */
/*           | 70 Garden Court          |                               */
/*           | Monterey, CA.     93940  |                               */
/*           +--------------------------+                               */
/*                                                                      */
/*   The  source code  contained in  this listing is a non-copyrighted  */
/*   work which  can be  freely used.  In applications of  this source  */
/*   code you  are requested to  acknowledge Digital Research, Inc. as  */
/*   the originator of this code.                                       */
/*                                                                      */
/*   Author:    LKW                                                     */
/*   PRODUCT:   GEM RSC-File Converter                                  */
/*   Module:    RSCREATE.C                                              */
/*   Version:   1.1                                                     */
/*   Created:   May 18, 1984                                            */
/*   Modified:  Robert Schneider, April 25, 1989                        */
/*              Cleaned up for use with ANSI-C COMPILERS                */
/************************************************************************/

#include "portab.h"

#if HIGH_C
pragma On (Pointers_compatible_with_ints);
#endif

#include "aes.h"
#include "dosbind.h"

MLOCAL LONG sub_pointer();

RSHDR starthdr = 
{
   0,      /* rsh_vrsn    */
   0,      /* rsh_object  */
   0,      /* rsh_tedinfo */
   0,      /* rsh_iconblk */
   0,      /* rsh_bitblk  */
   0,      /* rsh_frstr   */
   0,      /* rsh_string   string data     */
   0,      /* rsh_imdata   image data      */
   0,      /* rsh_frimg   */
   0,      /* rsh_trindex */
   0,      /* rsh_nobs    */
   0,      /* rsh_ntree   */
   0,      /* rsh_nted    */
   0,      /* rsh_nib     */
   0,      /* rsh_nbb     */
   0,      /* rsh_nstring */
   0,      /* rsh_nimages */
   0       /* rsh_rssize  */
};

#include "example.rsh"

WORD  endfile;

main()   
{
   WORD jnk1, handle;
   LONG cnt;                        /* in bytes   */

   starthdr.rsh_vrsn = 0;

   starthdr.rsh_object = sub_pointer(rs_object,&starthdr);

   starthdr.rsh_tedinfo = sub_pointer(rs_tedinfo,&starthdr);

   starthdr.rsh_iconblk = sub_pointer(rs_iconblk,&starthdr);

   starthdr.rsh_bitblk = sub_pointer(rs_bitblk,&starthdr);

   starthdr.rsh_frstr = sub_pointer(rs_frstr,&starthdr);

   starthdr.rsh_string = sub_pointer(rs_strings,&starthdr);

   starthdr.rsh_imdata = sub_pointer(rs_imdope[0].image,&starthdr);

   starthdr.rsh_frimg = sub_pointer(rs_frimg,&starthdr);

   starthdr.rsh_trindex = sub_pointer(rs_trindex,&starthdr);

   starthdr.rsh_nobs    = NUM_OBS;
   starthdr.rsh_ntree   = NUM_TREE;
   starthdr.rsh_nted    = NUM_TI;
   starthdr.rsh_nib     = NUM_IB;
   starthdr.rsh_nbb     = NUM_BB;
   starthdr.rsh_nimages = NUM_FRIMG;
   starthdr.rsh_nstring = NUM_FRSTR;

   fix_trindex();
   fix_objects();
   fix_tedinfo();
   fix_iconblk();
   fix_bitblk();
   fix_frstr();
   fix_frimg();

   handle = dos_create(pname, F_ATTR); 

   cnt = sub_pointer(&endfile,&starthdr);
   starthdr.rsh_rssize = (UWORD)cnt;
   dos_write(handle, cnt, &starthdr); 

   dos_close( handle );
}

fix_trindex()
{
   WORD test, ii;

   for (ii = 0; ii < NUM_TREE; ii++)
   {
      test = (WORD) rs_trindex[ii];
      rs_trindex[ii] = (WORD) sub_pointer(&rs_object[test], &starthdr);
   }
}

fix_objects()
{
   WORD   test, ii;

   for (ii = 0; ii < NUM_OBS; ii++)
   {
      test = (WORD) rs_object[ii].ob_spec;
      switch (rs_object[ii].ob_type & 0xff)
      {
         case G_TITLE:
         case G_STRING:
         case G_BUTTON:
            fix_str(&rs_object[ii].ob_spec);
            break;

         case G_TEXT:
         case G_BOXTEXT:
         case G_FTEXT:
         case G_FBOXTEXT:
           if (test != NIL)
              rs_object[ii].ob_spec = sub_pointer(&rs_tedinfo[test], &starthdr);
            break;

         case G_ICON:
            if (test != NIL)
              rs_object[ii].ob_spec = sub_pointer(&rs_iconblk[test], &starthdr);
            break;

         case G_IMAGE:
            if (test != NIL)
               rs_object[ii].ob_spec = sub_pointer(&rs_bitblk[test], &starthdr);
            break;

         default:
            break;
      }
   }
}

fix_tedinfo()
{
   WORD  ii;

   for (ii = 0; ii < NUM_TI; ii++)
   {
      fix_str(&rs_tedinfo[ii].te_ptext);
      fix_str(&rs_tedinfo[ii].te_ptmplt);
      fix_str(&rs_tedinfo[ii].te_pvalid);
   }
}

fix_frstr()
{
   WORD ii;

   for (ii = 0; ii < NUM_FRSTR; ii++)
      fix_str(&rs_frstr[ii]);
}

fix_str( where )
LONG *where;
{
   if (*where != NIL)
      *where = sub_pointer(rs_strings[(WORD)*where], &starthdr);
}

fix_iconblk()
{
   WORD   ii;

   for (ii = 0; ii < NUM_IB; ii++)
   {
      fix_img(&rs_iconblk[ii].ib_pmask);
      fix_img(&rs_iconblk[ii].ib_pdata);
      fix_str(&rs_iconblk[ii].ib_ptext);
   }
}

fix_bitblk()
{
   WORD ii;

   for (ii = 0; ii < NUM_BB; ii++)
      fix_img(&rs_bitblk[ii].bi_pdata);
}

fix_frimg()
{
   WORD ii;

   for (ii = 0; ii < NUM_FRIMG; ii++)
      fix_bb(&rs_frimg[ii]);
}

fix_bb(where)
LONG   *where;
{
   if (*where != NIL)
      *where = sub_pointer(&rs_bitblk[(WORD)*where], &starthdr);
}
   
fix_img(where)
LONG   *where;
{
   if (*where != NIL)
      *where = sub_pointer(rs_imdope[(WORD)*where].image, &starthdr);
}

MLOCAL LONG sub_pointer( p1, p2 )
VOID *p1, *p2;
{
   LONG l1,l2;

   l1 = FPSEG(p1) * 16l;
   l1 += FPOFF(p1);
   l2 = FPSEG(p2) * 16l;
   l2 += FPOFF(p2);

   return(l1 - l2);
}

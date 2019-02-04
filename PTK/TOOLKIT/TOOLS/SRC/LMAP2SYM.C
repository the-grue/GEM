/******************************************************************************
 *
 *    LMAPSYM.C
 *
 * Microsoft LINK "MAP" -to- GEMSID "SYM" converter
 * modified for LINK v3.0 by J. Weaver Jr. [71446,1362]
 * 30 May 1985
 * 22 Feb 1988, modified for large model symbol names, Mei chung
 * 29 Mar 1989, modified for Turbo-C-Map-Files, Robert Schneider
 ******************************************************************************/

#include "portab.h"
#include "stdio.h"

BYTE *find_abs(str)
BYTE *str;
{
   while (*str)
   {
      if (*str == 'A')
         if (*(str + 1) == 'b')
            if (*(str + 2) == 's')
               return(str);
      str++;
   }

   return(0l);
}

main()
{
   WORD off;
   UWORD seg, old_seg, data_seg;
   BYTE buf[80];
   BYTE var[39];
   BYTE *p, *seg_name;
   WORD done = 0;

   while (!done)
   {
      if ((p = fgets(buf, sizeof(buf), stdin)) == 0)
      {
         fprintf(stderr,"exit 1\n");
         exit(1);
      }

      if (!strncmp(buf, " Start", 6))
      {
         while (!done)
         {
            if ((p = fgets(buf, sizeof(buf), stdin)) == 0)
            {
               fprintf(stderr,"exit 2\n");
               exit(2);
            }
            seg_name = buf + 22;
            if (!strncmp(seg_name, "DATA", 4))
            {
               sscanf(buf, "%4x",&data_seg);
               done = 1;
            }
            if (!strncmp(seg_name, "_DATA", 5))
            {
               sscanf(buf, "%4x",&data_seg);
               done = 1;
            }
         }
      }
   }
   done = 0;
   while(!done)
   {
      if ((p = fgets(buf, sizeof(buf), stdin)) == 0)
      {
         fprintf(stderr,"exit 3\n");
         exit(3);
      }
      if (!strncmp(buf, "  Address         Publics by Value", 34))
         done = 1;
   }
   old_seg =0xffff;
   while (fgets(buf, sizeof(buf), stdin))
   {
      if ((p = find_abs(buf)) != 0)
      {
         *p++ = ' ';
         *p++ = ' ';
         *p++ = ' ';
      }
      if (sscanf(buf,"%4x:%4x %s", &seg, &off, var) < 3)
         continue;
      if (seg != old_seg)
      {
         if (seg < data_seg)
            fprintf(stdout, " 0000 LABELS\t%04X CODE\n", seg);
         else
            fprintf(stdout, " 0000 VARIABLES\t%04X DATA\n", seg);
         old_seg = seg;
      }
      fprintf(stdout, "%04X %s\n", off, var);
   }
   exit(0);
} /*  LMAP2SYM.C  */


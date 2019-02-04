/******************************************************************************
 *
 *    MAPSYM.C
 *
 * Microsoft LINK "MAP" -to- GEMSID "SYM" converter
 * modified for LINK v3.0 by J. Weaver Jr. [71446,1362]
 * 30 May 1985
 *
 ******************************************************************************/

#include "portab.h"
#include "stdio.h"

main()
{
   WORD seg, off, flg;
   BYTE buf[200];
   BYTE var[32];
   BYTE *p;
   WORD done = 0;

   while (!done)
   {
      if ((p = fgets(buf, sizeof(buf), stdin)) == 0)
         exit(1);
      if (!strncmp(buf, "  Address         Publics by Value",34))
      {
         fprintf(stderr,"String found '%s'\n",buf);
         done = 1;
      }
   }

   fprintf(stdout, " 0000 LABELS\n");
   flg = 1;
   while (fscanf(stdin, "%4x:%4x %s", &seg, &off, var) > 0)
   {
      if (flg && seg)
      {
         flg = 0;
         fprintf(stdout, " 0000 VARIABLES\n");
      }
      fprintf(stdout, "%04x %s\n", off, var);
   }
   exit(0);
}

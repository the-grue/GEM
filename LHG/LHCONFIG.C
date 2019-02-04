
version()
{
puts("LHconfig 1.1 ");
puts("(C)1994-1999 Ken Mauro, all rights reserved.");
puts("             Free for non commercial use.	  ");
 /* June 1, 1999 */
}

#include "stdio.h"

#define buflen	64

main(argc,argv)
	int argc;
	char *argv[];
{
	FILE *fp;

	int low,high,c,i,j;
	int dkey,dkey2,len,max,fptr;
	unsigned int chk=0;
	char buffer[buflen],buf2[4096],filespec[16];

	dkey=24; /*encoding key for lhgem.asm*/
	dkey2=14;

	sprintf(&filespec[0],"LHGEM.EXE");
	sprintf(&buffer[0],"LOADHIGH GEM.EXE");

	version();
	puts("\n");
	puts("If your system is a standard GEM 3.x installation,");
	puts("Simply select 1 or 2 to configure.\n");

	puts("If your system DOES NOT include the GEM DESKTOP.APP,");
	puts("or you wish to directly execute your application..\n");

	puts("Example usage:  LHapp \\keypub\\keypub.app");
	puts("Example usage:  LHapp \\gemapps\\artline.app\n\n");




	puts("(1) ... Configure LHgem.exe for MS-DOS\n");

	puts("(2) ... Configure HIgem.exe for DR-DOS\n");

	puts("(3) ... Exit without any changes\n");

	puts("\n\n\n");


AGAIN:
	c=getch();
	switch(c){
		  case 0x31:
			sprintf(&filespec[0],"LHGEM.EXE");
			sprintf(&buffer[0],"LOADHIGH GEM.EXE");
			break;
		  case 0x32:
			sprintf(&filespec[0],"HIGEM.EXE");
			sprintf(&buffer[0],"HIGHLOAD GEM.EXE");
			break;
		  case 0x33:
			copyr(1);
			break;
			}

  if(c==27) copyr(1);
  if(c==0x31) goto START;
  if(c==0x32) goto START;
  goto AGAIN;


 START:

	if(argc==2) {
		strcat(&buffer," ");
		strcat(&buffer,argv[1]);
		}
	len=strlen(buffer);
	buffer[len]=0x0d;			/* add CR for command line */
	buffer[len+1]=0x00;


	if((fp = fopen(&filespec[0],"r+b")) != NULL) {

		fseek(fp,0L,SEEK_END);
		max=ftell(fp);
		rewind(fp);

		printf("%s has been configured to:  %s\n",filespec,&buffer);

		for(i=0;i<buflen;i++){
			c=buffer[i];
			c+=(char) dkey;	/* 64 is the encoding key */
			buffer[i]=c;
			}

		for(i=0;i<max;i++){
			c=fgetc(fp);
			if(c==47){
				c=fgetc(fp);
				if(c==67){
					fseek(fp,1L,SEEK_CUR);
					for(j=0;j<buflen;j++){
						c=buffer[j];
						fputc(c,fp);
						}
					fputc(buflen,fp);  /*low byte*/
					fputc(0,fp);
					fputc(dkey,fp); /*lbyte key*/
					fputc(0,fp);

					fseek(fp,-4L,SEEK_END);

					low = fgetc(fp);
					high= fgetc(fp);
				if(low==0){
					low = fgetc(fp);
					high= fgetc(fp);
					len = (low & 0xff)+((high & 0xff)<<8);

					fptr=(max-len)-6;


					fseek(fp,(long) fptr,SEEK_SET);
					for(j=0;j<len;j++){
						c=fgetc(fp);
						chk+=c;
						c+=(char)dkey2;
						buf2[j]=c;
						}

					fseek(fp,(long) fptr,SEEK_SET);
					for(j=0;j<len;j++){
						c=buf2[j];
						fputc(c,fp);
						}
					putw(chk,fp);
					fputc(dkey2,fp);
					fputc(0,fp);
					} /* for if low==0 */
					break;
					}
				}
			}
		fclose(fp);
	} else printf("Patch not applied - %s not found.",filespec);
copyr(0);

}

copyr(e)
	int e;
{
puts("Done.");
if (e==1) exit(1);
}


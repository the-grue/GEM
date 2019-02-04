/*
** hcompact.c
**	- Fool Metaware's High C need for library
**	- High C outputs an external reference to the symbol 'COMPACT?'
**	  in order to guarantee that the user is linking with the correct model
**	  of the run time library
**	- Compile and link this module in for stand alone applications
*/

void compact()
{
}
pragma alias(compact,"COMPACT?");

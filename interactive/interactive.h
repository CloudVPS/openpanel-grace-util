#ifndef _gracesh_H
#define _gracesh_H 1
#include <grace/application.h>
#include <grace/terminal.h>

//  -------------------------------------------------------------------------
/// Main application class.
//  -------------------------------------------------------------------------
class graceshApp : public application
{
public:
		 	 graceshApp (void) :
				application ("nl.madscience.apps.interactive"),
				term (this, fin, fout)
			 {
			 }
			~graceshApp (void)
			 {
			 }

	int		 main (void);
	
	int		 ctrlhandler (int ki, termbuffer &tb)
			 {
			 	term.set ("exit");
			 	return 1;
			 }
	
	terminal<graceshApp> term;
};

#endif

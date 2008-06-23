#ifndef _shoxutil_H
#define _shoxutil_H 1
#include <grace/application.h>

//  -------------------------------------------------------------------------
/// Main application class.
//  -------------------------------------------------------------------------
class shoxutilApp : public application
{
public:
		 	 shoxutilApp (void) :
				application ("nl.madscience.tools.shoxutil")
			 {
			 	opt = $("-r", $("long", "--read")) ->
			 		  $("-w", $("long", "--write")) ->
			 		  $("--read",
			 		  		$("argc", 1) ->
			 		  		$("help", "shox file to read and convert to xml")
			 		   ) ->
			 		  $("--write",
			 		  		$("argc", 1) ->
			 		  		$("help", "shox file to write from xml on stdin")
			 		   );
			 }

			~shoxutilApp (void)
			 {
			 }

	int		 main (void);
};

#endif

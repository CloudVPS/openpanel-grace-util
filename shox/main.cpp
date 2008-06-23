#include "shox.h"

$appobject (shoxutilApp);

//  =========================================================================
/// Main method.
//  =========================================================================
int shoxutilApp::main (void)
{
	value v;
	string out;

	if (argv.exists ("--read"))
	{
		v.loadshox (argv["--read"]);
		out = v.toxml ();
		fout.puts (out);
		return 0;
	}
	if (argv.exists ("--write"))
	{
		while (! fin.eof())
		{
			out.strcat (fin.read (1024));
		}
		
		v.fromxml (out);
		v.saveshox (argv["--write"]);
		return 0;
	}
	return 0;
}


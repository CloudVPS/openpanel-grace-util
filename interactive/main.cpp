#include "interactive.h"
#include <grace/filesystem.h>
#include <grace/system.h>
#include <grace/pcre.h>

#include "compilerflags.h"

$appobject (graceshApp);

const char *CODE =
"#include <grace/application.h>\n"
"#include <grace/str.h>\n"
"#include <grace/strutil.h>\n"
"#include <grace/filesystem.h>\n"
"#include <grace/system.h>\n"
"#include <grace/http.h>\n"
"#include <grace/process.h>\n"
"#include <grace/netdb.h>\n"
"#include <grace/generators.h>\n"
"#include <grace/pcre.h>\n"
"#include <grace/version.h>\n"
"\n"
"class cliapp : public application\n"
"{\n"
"public:\n"
"	cliapp (void) : application (\"cliapp\") {}\n"
"	~cliapp (void) {}\n"
"   %s\n"
"   httpsocket http;\n"
"   void print (const value &v) { if (v.count()) fout.writeln (v.tojson()); else fout.writeln (v); }\n"
"	int main (void)\n"
"	{\n"
"		value __env; __env.loadshox (\".__cli.shox\");\n"
"		%s\n"
"		%s;\n"
"		%s;\n"
"		__env.saveshox (\".__cli.shox\");\n"
"	}\n"
"};\n"
"$appobject (cliapp);\n";

//  =========================================================================
/// Main method.
//  =========================================================================
int graceshApp::main (void)
{
	value env;
	value envtypes;
	value funcs;
	string inputline;
	
	term.addkeyresponse (4, &graceshApp::ctrlhandler);

	while (1)
	{
		int bracecnt = 0;
		char inquote = 0;
		statstring funcname;
		string oldfunc;
		
		string ln = term.readline (">>> ");
		inputline.crop ();
		
		do
		{
			for (int i=0; i<ln.strlen(); ++i)
			{
				if (ln[i] == '\\') i++;
				else if (inquote)
				{
					if (ln[i] == inquote) inquote = 0;
				}
				else
				{
					if ((ln[i] == '\"')||(ln[i] == '\''))
					{
						inquote = ln[i];
					}
					else if (ln[i] == '{') bracecnt++;
					else if (ln[i] == '}') bracecnt--;
				}
			}
			
			// Need an extra space or the bloody regexp will become
			// mutinous.
			if (ln.strncmp ("print ", 6) == 0)
			{
				ln.insert (" ");
			}
			
			// Add parentheses around print statements that have none.
			pcregexp e ("([[:space:]]|^)print[[:space:]]+([^\\(][^;]*)(;.*)?$");
			ln = e.replace (ln, "\\1print (\\2)\\3", true);
			
			// Stupid hack to allow for string operators on string literals.
			pcregexp ee ("([^\\\\])\"([^\"]*[^\\\\])\"\\.");
			ln = ee.replace (ln, "\\1((string)\"\\2\").", true);
			
			ln.chomp ();

			if (inputline && (inputline[-1] != '{') &&
				(inputline[-1] != '}')) inputline += ";";

			inputline += ln;
			
			if (bracecnt)
			{
				ln = term.readline ("  > ");
			}
		} while (bracecnt);
		
		if (inputline == "exit")
		{
			term.off ();
			term.termbuf.savehistory ("home:.gracesh-history");
			return 0;
		}
		
		if (inputline.strncmp ("value ", 6) == 0)
		{
			ln.cropafter (' ');
			ln.cropat (' ');
			ln.cropat ('=');
			env[ln] = "";
			envtypes[ln] = "value";
			inputline = inputline.mid (6);
		}
		
		if (inputline.strncmp ("string ", 7) == 0)
		{
			ln.cropafter (' ');
			ln.cropat (' ');
			ln.cropat ('=');
			env[ln] = "";
			envtypes[ln] = "string";
			inputline = inputline.mid (7);
		}

		if (inputline.strncmp ("int ", 4) == 0)
		{
			ln.cropafter (' ');
			ln.cropat (' ');
			ln.cropat ('=');
			env[ln] = "";
			envtypes[ln] = "int";
			inputline = inputline.mid (4);
		}
		
		if (inputline.strncmp ("bool ", 5) == 0)
		{
			ln.cropafter (' ');
			ln.cropat (' ');
			ln.cropat ('=');
			env[ln] = "";
			envtypes[ln] = "int";
			inputline = inputline.mid (5);
		}
		
		if (inputline.strncmp ("double ", 7) == 0)
		{
			ln.cropafter (' ');
			ln.cropat (' ');
			ln.cropat ('=');
			env[ln] = "";
			envtypes[ln] = "double";
			inputline = inputline.mid (7);
		}
		
		if (inputline.strncmp ("statstring ", 11) == 0)
		{
			ln.cropafter (' ');
			ln.cropat (' ');
			ln.cropat ('=');
			env[ln] = "";
			envtypes[ln] = "statstring";
			inputline = inputline.mid (11);
		}
		
		if (inputline.strncmp ("timestamp ", 10) == 0)
		{
			ln.cropafter (' ');
			ln.cropat (' ');
			ln.cropat ('=');
			env[ln] = "";
			envtypes[ln] = "timestamp";
			inputline = inputline.mid (10);
		}

		if (inputline.strncmp ("ipaddress ", 10) == 0)
		{
			ln.cropafter (' ');
			ln.cropat (' ');
			ln.cropat ('=');
			env[ln] = "";
			envtypes[ln] = "ipaddress";
			inputline = inputline.mid (10);
		}

		if (inputline.strncmp ("function ", 9) == 0)
		{
			inputline = inputline.mid (9);
			string fdec = inputline.copyuntil ('(');
			fdec = fdec.rtrim (" \t");
			
			string tfuncname = fdec.copyafterlast (' ');
			while (tfuncname && ((tfuncname[0]=='&')||(tfuncname[0]=='*')))
			{
				tfuncname = tfuncname.mid (1);
			}
			funcname = tfuncname;
			
			oldfunc = funcs[funcname];
			funcs[funcname] = inputline;
			continue;
		}
		
		string funccode;
		string refcode;
		string writeback;
		foreach (f, funcs)
		{
			funccode.strcat (f);
			funccode.strcat ("\n");
		}
		foreach (n, env)
		{
			if (envtypes[n.id()] == "value")
			{
				refcode += "value &%s = __env[\"%s\"]; " %format (n.id(), n.id());
			}
			else
			{
				refcode += "%s %s = __env[\"%s\"]; " %format (envtypes[n.id()],
															 n.id(), n.id());
				writeback += "__env[\"%s\"] = %s;" %format (n.id(), n.id());
			}
		}
		string code = CODE %format (funccode, refcode, inputline, writeback);
		fs.save (".__cli.cpp", code);
		env.saveshox (".__cli.shox");
		if (core.sh ("g++ " INCLUDES " -o .__cli .__cli.cpp " LIBS))
		{
			if (funcname)
			{
				if (oldfunc)
				{
					funcs[funcname] = oldfunc;
				}
				else
				{
					funcs.rmval (funcname);
				}
			}
		}
		
		term.off ();
		
		core.sh ("./.__cli");
		
		env.loadshox (".__cli.shox");
		fs.rm (".__cli");
		fs.rm (".__cli.cpp");
		fs.rm (".__cli.shox");
	}
	return 0;
}


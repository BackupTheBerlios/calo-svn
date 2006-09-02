/* \file
 * The main source file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <gtkmm/main.h>

#include "URIFetcher.h"
#include "TextDumper.h"
#include "PDFCreator.h"


static void quit (URIFetcher* fetcher);

Glib::RefPtr< Glib::MainLoop > loop;

int main (int argc , char **argv)
{
	g_type_init();
	g_thread_init (NULL);           // needed by libsoup

	URIFetcher *fetcher = URIFetcher::create();
	//fetcher->fetch ("http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-demo.txt");
	//fetcher->fetch ("http://www.lynx.org/");
	//fetcher->fetch ("http://www.ark.in-berlin.de/plde_unicode.txt");
	//fetcher->finish (quit);


	loop = Glib::MainLoop::create();
	loop->run();

}

static void quit (URIFetcher* fetcher)
{
	//const Glib::ustring& str = fetcher->str();
	//TextDumper *dumper = TextDumper::create();
	//const Glib::ustring& dump = dumper->render (str);
	
	//PDFCreator *pdf = PDFCreator::create();
	//*pdf << dump;
	//pdf->save();

	//delete pdf;
	//delete dumper;
	//delete fetcher;
	
	loop->quit();
}



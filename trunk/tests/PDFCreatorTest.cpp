/* \file
 * The main source file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include "PDFCreator.h"


int main (int argc , char **argv)
{
	PDFCreator *pdf = PDFCreator::create ("test.pdf", 700, 1000);
	Glib::ustring str;
	std::cin >> str;
	*pdf << str;
	pdf->save();

	delete pdf;
}




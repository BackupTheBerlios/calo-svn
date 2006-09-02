## Makefile for program src directory in GNU hello.
## Copyright (C) 1996, 1997, 2001 Free Software Foundation, Inc.
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA.

## Process this file with automake to produce Makefile.in.

bin_PROGRAMS = calo
calo_SOURCES =  exceptions.h		\
		main.cpp		\
		CairoPDFCreator.cpp	\
		CairoPDFCreator.h	\
		DumpProcessor.cpp	\
		DumpProcessor.h		\
		FetchAndRenderPipeline.cpp\
		FetchAndRenderPipeline.h\
		InputPipe.cpp		\
		InputPipe.h		\
		LynxTextDumper.cpp	\
		LynxTextDumper.h	\
		LynxToCairoProcessor.cpp\
		LynxToCairoProcessor.h	\
		PDFContext.cpp		\
		PDFContext.h		\
		PDFCreator.cpp		\
		PDFCreator.h		\
		SoupURIFetcher.cpp	\
		SoupURIFetcher.h	\
		TempFile.cpp		\
		TempFile.h		\
		TextDumper.cpp		\
		TextDumper.h		\
		URIFetcher.cpp		\
		URIFetcher.h

check_PROGRAMS = testPDFCreator	testPipeline

testPDFCreator_SOURCES = testPDFCreator.cpp	\
			 PDFContext.cpp PDFContext.h	\
			 PDFCreator.cpp PDFCreator.h	\
			 CairoPDFCreator.cpp CairoPDFCreator.h

testPipeline_SOURCES = testPipeline.cpp	\
		FetchAndRenderPipeline.cpp FetchAndRenderPipeline.h	\
		URIFetcher.cpp URIFetcher.h				\
		SoupURIFetcher.cpp SoupURIFetcher.h			\
		TempFile.cpp TempFile.h					\
		TextDumper.cpp TextDumper.h 				\
		InputPipe.cpp InputPipe.h				\
		LynxTextDumper.cpp LynxTextDumper.h			\
		DumpProcessor.cpp DumpProcessor.h			\
		LynxToCairoProcessor.cpp LynxToCairoProcessor.h		\
		URIFetcher.cpp URIFetcher.h				\
		PDFContext.cpp PDFContext.h				\
		PDFCreator.cpp PDFCreator.h				\	
		CairoPDFCreator.cpp CairoPDFCreator.h

#hello_LDADD = @LIBINTL@ @ALLOCA@

localedir = $(datadir)/locale

#INCLUDES = -I. -I$(srcdir) -I.. -I../intl -I$(top_srcdir)/intl
INCLUDES = $(PACKAGE_CFLAGS)
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
LIBS = $(PACKAGE_LIBS) 
AM_CXXFLAGS = -Wall

## # Automake cannot handle this now (version 1.0).
## $(bin_PROGRAMS): $(LDADD)

## src/Makefile.am ends here

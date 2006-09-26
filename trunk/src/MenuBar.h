#ifndef __MENU_BAR_H
#define __MENU_BAR_H

/* \file
 * Singleton containing/handling the menu hierarchy.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/uimanager.h>

namespace Gtk
{
	class Widget;
	class Window;
}

/// MenuBar

class MenuBar
{
public:
	static Gtk::Widget* create (Gtk::Window*);

	Glib::RefPtr<Gtk::UIManager> _uim;

protected:
	MenuBar (Gtk::Window*);

	void on_menu_file_new();
	void on_menu_file_save_as_default();
	void on_menu_file_quit();
	void on_menu_edit_copy();
	void on_menu_edit_paste();
	void on_menu_edit_feed();
	void on_menu_help_about();
	void on_menu_others();
	
	Glib::RefPtr<Gtk::ActionGroup> _ag;
};



#endif

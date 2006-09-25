/* \file
 * Code that creates the menu bar.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <gtkmm/actiongroup.h>
#include <gtkmm/stock.h>
#include <gtkmm/widget.h>
#include "MenuBar.h"

static MenuBar* _theContext = NULL;

Gtk::Widget* MenuBar::create()
{
	if (_theContext == NULL)
		_theContext = new MenuBar;
	return _theContext->_uim->get_widget ("/Menubar");
}

MenuBar::MenuBar()
{
	Glib::RefPtr<Gtk::ActionGroup> ag = Gtk::ActionGroup::create();

	//File menu:
	ag->add( Gtk::Action::create("FileNew", 
			Gtk::Stock::NEW, "_New", "Create a new file"),
		sigc::mem_fun(*this, &MenuBar::on_menu_file_new) );
	ag->add( Gtk::Action::create("FileSaveAsDefault", 
			Gtk::Stock::SAVE, "_Save as Default", "Save file as default feed file"),
		sigc::mem_fun(*this, &MenuBar::on_menu_file_save_as_default) );
	ag->add( Gtk::Action::create("FileQuit", 
			Gtk::Stock::QUIT, "_Quit", "Leave the program"),
		sigc::mem_fun(*this, &MenuBar::on_menu_file_quit) );

	ag->add( Gtk::Action::create("FileMenu", "File") );

	//Edit menu:
	ag->add( Gtk::Action::create("EditMenu", "Edit") );
	ag->add( Gtk::Action::create("EditCopy", Gtk::Stock::COPY),
    		sigc::mem_fun(*this, &MenuBar::on_menu_others) );
	ag->add( Gtk::Action::create("EditPaste", Gtk::Stock::PASTE),
		sigc::mem_fun(*this, &MenuBar::on_menu_others) );
	ag->add( Gtk::Action::create("EditFeed", "Edit the selected feed entry"), Gtk::AccelKey("<control>E"),
		sigc::mem_fun(*this, &MenuBar::on_menu_others) );

	//Help menu:
	ag->add( Gtk::Action::create("HelpMenu", "Help") );
	ag->add( Gtk::Action::create("HelpAbout", Gtk::Stock::HELP),
		sigc::mem_fun(*this, &MenuBar::on_menu_others) );

	_uim = Gtk::UIManager::create();
	_uim->insert_action_group (ag);

	//Layout the actions in the menubar
  
	Glib::ustring ui_info = 
        "<ui>"
        "  <menubar name='MenuBar'>"
        "    <menu action='FileMenu'>"
        "      <menuitem action='FileNew'/>"
        "      <menuitem action='FileSaveAsDefault'/>"
        "      <separator/>"
        "      <menuitem action='FileQuit'/>"
        "    </menu>"
        "    <menu action='EditMenu'>"
        "      <menuitem action='EditCopy'/>"
        "      <menuitem action='EditPaste'/>"
        "      <menuitem action='EditFeed'/>"
        "    </menu>"
        "    <menu action='HelpMenu'>"
        "      <menuitem action='HelpAbout'/>"
        "    </menu>"
        "  </menubar>"
        "</ui>";

#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try
	{      
		_uim->add_ui_from_string(ui_info);
	}
	catch(const Glib::Error& ex)
	{
		std::cerr << "building menus failed: " <<  ex.what();
	}
#else
	std::auto_ptr<Glib::Error> ex;
	_uim->add_ui_from_string(ui_info, ex);
	if(ex.get())
	{ 
		std::cerr << "building menus failed: " <<  ex->what();
	}
#endif //GLIBMM_EXCEPTIONS_ENABLED
} 

void MenuBar::on_menu_file_new()
{
}

void MenuBar::on_menu_file_save_as_default()
{
}

void MenuBar::on_menu_file_quit()
{
}

void MenuBar::on_menu_edit_copy()
{
}

void MenuBar::on_menu_edit_paste()
{
}

void MenuBar::on_menu_edit_feed()
{
}

void MenuBar::on_menu_help_about()
{
}

void MenuBar::on_menu_others()
{
}

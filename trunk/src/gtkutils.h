/* Copyright (C) 2004 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#ifndef _MYG_GTKUTILS_H_
#define _MYG_GTKUTILS_H_

#include <sigc++/sigc++.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/menuitem.h>
#include <gdkmm/pixbufanimation.h>


class MGPixbufCache {
    static MGPixbufCache *_instance;

    std::list<std::string> _path;
    std::map<std::string,Glib::RefPtr<Gdk::Pixbuf> > _cache;
    std::map<std::string,Glib::RefPtr<Gdk::PixbufAnimation> > _acache;

  public:
    static MGPixbufCache *instance();
    
    void add_search_path(const std::string &path);
    
    Glib::RefPtr<Gdk::Pixbuf> load(const std::string &file, bool cache=true);
    Glib::RefPtr<Gdk::PixbufAnimation> load_anim(const std::string &file, bool cache=true);
};

#define PIXCACHE MGPixbufCache::instance()


# define GTKMM_MESSAGE_DIALOG(var, message, type, buttons, modal)\
    var(message, true, type, buttons, modal)

# define GTKMM_MESSAGE_DIALOG2(var, parent, message, type, buttons, modal)\
    var(parent, message, true, type, buttons, modal)



void myg_make_file_entry(Gtk::Entry &entry, Gtk::Button &button, bool dir_only= false);


void myg_make_image_button(Gtk::Button &button,
                           const Glib::RefPtr<Gdk::Pixbuf> &image,
                           const Glib::ustring &label);

void myg_image_button_set(Gtk::Button &button,
                          const Glib::RefPtr<Gdk::Pixbuf> &image,
                          const Glib::ustring &label);

void myg_image_button_set(Gtk::Button &button,
                          const Gtk::StockID& lhs,
                          const Glib::ustring &label);

Gtk::MenuItem *myg_make_separator_item();
Gtk::MenuItem *myg_make_stock_image_item(const Gtk::StockID& id,
                                         const Glib::ustring &label);
Gtk::MenuItem *myg_make_image_item(const std::string &file,
                                   const Glib::ustring &label);



Gtk::MenuItem* myg_menu_add(Gtk::Menu &menu, const Glib::ustring &label,
                  const sigc::slot<void> &slot, const std::string &id);

void myg_menu_add(Gtk::Menu &menu, const Gtk::StockID &icon,
                  const Glib::ustring &label, const sigc::slot<void> &slot, 
                  const std::string &id);

void myg_menu_add(Gtk::Menu &menu, const std::string &icon, 
                  const Glib::ustring &label, const sigc::slot<void> &slot,
                  const std::string &id);

void myg_menu_set_sensitive(Gtk::Menu &menu, const std::string &id, bool flag);

std::string myg_menu_get_id_at_index(Gtk::Menu &menu, int index);
int myg_menu_get_index_with_id(Gtk::Menu &menu, const std::string &id);

void myg_menu_add(Gtk::Menu &menu);



bool myg_ask_string(Gtk::Window &parent, const Glib::ustring &title,
                    const Glib::ustring &msg, Glib::ustring &input,
                    bool password= false);
bool myg_ask_string(const Glib::ustring &title,
                    const Glib::ustring &msg, Glib::ustring &input,
                    bool password= false);

void myg_show_error(Gtk::Window &parent, const Glib::ustring &msg);
void myg_show_error(const Glib::ustring &msg);

void myg_show_warning(Gtk::Window &parent, const Glib::ustring &msg);
void myg_show_warning(const Glib::ustring &msg);

void myg_show_info(Gtk::Window &parent, const Glib::ustring &msg);
void myg_show_info(const Glib::ustring &msg);



#endif /* _MYG_GTKUTILS_H_ */


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

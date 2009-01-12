/*
 * AT-SPI - Assistive Technology Service Provider Interface
 * (Gnome Accessibility Project; http://developer.gnome.org/projects/gap)
 *
 * Copyright 2008 Novell, Inc.
 * Copyright 2001, 2002 Sun Microsystems Inc.,
 * Copyright 2001, 2002 Ximian, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <atk/atk.h>
#include <droute/droute.h>

#include "spi-common/spi-dbus.h"

static DBusMessage *
impl_getNLinks (DBusConnection * bus, DBusMessage * message, void *user_data)
{
  AtkHypertext *hypertext = (AtkHypertext *) user_data;
  gint rv;
  DBusMessage *reply;

  g_return_val_if_fail (ATK_IS_HYPERTEXT (user_data),
                        droute_not_yet_handled_error (message));
  rv = atk_hypertext_get_n_links (hypertext);
  reply = dbus_message_new_method_return (message);
  if (reply)
    {
      dbus_message_append_args (reply, DBUS_TYPE_INT32, &rv,
				DBUS_TYPE_INVALID);
    }
  return reply;
}

static DBusMessage *
impl_getLink (DBusConnection * bus, DBusMessage * message, void *user_data)
{
  AtkHypertext *hypertext = (AtkHypertext *) user_data;
  DBusError error;
  dbus_int32_t linkIndex;
  AtkHyperlink *link;

  g_return_val_if_fail (ATK_IS_HYPERTEXT (user_data),
                        droute_not_yet_handled_error (message));
  dbus_error_init (&error);
  if (!dbus_message_get_args
      (message, &error, DBUS_TYPE_INT32, &linkIndex, DBUS_TYPE_INVALID))
    {
      return SPI_DBUS_RETURN_ERROR (message, &error);
    }
  link = atk_hypertext_get_link (hypertext, linkIndex);
  return spi_dbus_return_object (message, ATK_OBJECT (link), FALSE);
}

static DBusMessage *
impl_getLinkIndex (DBusConnection * bus, DBusMessage * message,
		   void *user_data)
{
  AtkHypertext *hypertext = (AtkHypertext *) user_data;
  DBusError error;
  dbus_int32_t characterIndex;
  dbus_int32_t rv;
  DBusMessage *reply;

  g_return_val_if_fail (ATK_IS_HYPERTEXT (user_data),
                        droute_not_yet_handled_error (message));
  dbus_error_init (&error);
  if (!dbus_message_get_args
      (message, &error, DBUS_TYPE_INT32, &characterIndex, DBUS_TYPE_INVALID))
    {
      return SPI_DBUS_RETURN_ERROR (message, &error);
    }
  rv = atk_hypertext_get_link_index (hypertext, characterIndex);
  reply = dbus_message_new_method_return (message);
  if (reply)
    {
      dbus_message_append_args (reply, DBUS_TYPE_INT32, &rv,
				DBUS_TYPE_INVALID);
    }
  return reply;
}

static DRouteMethod methods[] = {
  {impl_getNLinks, "getNLinks"},
  {impl_getLink, "getLink"},
  {impl_getLinkIndex, "getLinkIndex"},
  {NULL, NULL}
};

void
spi_initialize_hypertext (DRoutePath *path)
{
  droute_path_add_interface (path,
                             SPI_DBUS_INTERFACE_HYPERTEXT,
                             methods,
                             NULL);
};
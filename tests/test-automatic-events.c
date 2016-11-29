/*
  libconic - Internet Connectivity library
  
  Copyright (C) 2006 Nokia Corporation. All rights reserved.

  Contact: Patrik Flykt <patrik.flykt@nokia.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License
  version 2.1 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
  02110-1301 USA
*/

#include <string.h>
#include <glib.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>

#include <conic.h>

#define USER_DATA_MAGIC 0xacdcacdc

static void connection_cb(ConIcConnection *connection,
			  ConIcConnectionEvent *event,
			  gpointer user_data);

static GMainLoop *loop;
static ConIcConnection *connection;
static DBusConnection *dbus_connection;

static gboolean start(gpointer data)
{
	g_debug("start(%p)", data);

	GValue value;

	memset(&value, 0, sizeof(value));
	g_value_init(&value, G_TYPE_BOOLEAN);
	
	connection = con_ic_connection_new();
	g_assert(connection != NULL);

	g_signal_connect(G_OBJECT(connection), "connection-event",
			 G_CALLBACK(connection_cb),
			 GINT_TO_POINTER(USER_DATA_MAGIC));

	g_value_set_boolean(&value, TRUE);
	g_object_set_property(G_OBJECT(connection), "automatic-connection-events",
			      &value);
	return FALSE;
}

static gboolean stop(gpointer data)
{
	g_debug("stop(%p)", data);

	GValue value;

	memset(&value, 0, sizeof(value));
	g_value_init(&value, G_TYPE_BOOLEAN);
	g_value_set_boolean(&value, FALSE);
	g_object_set_property(G_OBJECT(connection),
			      "automatic-connection-events",
			      &value);

	g_object_unref(connection);
	g_main_loop_quit(loop);

	return FALSE;
}


static void connection_cb(ConIcConnection *connection,
			  ConIcConnectionEvent *event,
			  gpointer user_data)
{
	g_debug("connection_cb(%p, %p)", (void *)event, user_data);

	const gchar *iap_id, *bearer;
	ConIcConnectionStatus status;
	ConIcConnectionError error;

	g_assert(GPOINTER_TO_INT(user_data) == USER_DATA_MAGIC);
	g_assert(CON_IC_IS_CONNECTION_EVENT(event));

	status = con_ic_connection_event_get_status(event);
	error = con_ic_connection_event_get_error(event);
	iap_id = con_ic_event_get_iap_id(CON_IC_EVENT(event));
	bearer = con_ic_event_get_bearer_type(CON_IC_EVENT(event));
	
	switch (status) {
	case CON_IC_STATUS_CONNECTED:
		g_debug("CONNECTED (%s, %s, %i, %i)",
			iap_id, bearer, status, error);
		break;
	case CON_IC_STATUS_DISCONNECTED:
		g_debug("DISCONNECTED (%s, %s, %i, %i)",
			iap_id, bearer, status, error);
		break;
	case CON_IC_STATUS_DISCONNECTING:
		g_debug("DISCONNECTING (%s, %s, %i, %i)",
			iap_id, bearer, status, error);
		break;
	default:
		break;
	}
}


static GMainLoop *init_glib(void)
{
	g_type_init();
	return g_main_loop_new(NULL, FALSE);

}

static void init_dbus(void)
{
	DBusError error;
	
	dbus_error_init(&error);
	dbus_connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	if (dbus_connection == NULL)
		g_error("Error when connecting to the session bus: %s",
			error.message);

	dbus_connection_setup_with_g_main(dbus_connection, NULL);
}

static void finalize_dbus(void)
{
	dbus_connection_unref(dbus_connection);
}

int main(int argc, char *argv[]) 
{
	loop = init_glib();
	init_dbus();

	start(NULL);
	g_main_loop_run(loop);
	stop(NULL);

	g_main_loop_unref(loop);
	finalize_dbus();
	
	return 0;
}

/**
 * XMPP - libpurple transport
 *
 * Copyright (C) 2009, Jan Kaluza <hanzz@soc.pidgin.im>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */

#ifndef _ABSTRACT_USER_H
#define _ABSTRACT_USER_H

#include <string>
#include <list>
#include "account.h"
#include "value.h"
#include "resourcemanager.h"
#ifndef TESTS
#include "filetransferrepeater.h"
#endif

typedef enum { 	CALLER_ADHOC,
				CALLER_SEARCH,
				CALLER_DUMMY
				} AdhocDataCallerType;

struct AdhocData {
	std::string id;
	std::string from;
	std::string node;
	AdhocDataCallerType callerType;
};

struct RoomData {
	std::string resource;
	std::string nickname;
	operator bool() const {
		return !resource.empty();
	}
};

class AbstractUser : public ResourceManager
{
	public:
		AbstractUser() : removeTimer(0), m_protocolData(NULL), m_filetransfers(NULL)
			{ m_filetransfers = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL); }
		virtual ~AbstractUser() { g_hash_table_destroy(m_filetransfers); }
		virtual const std::string &userKey() = 0;
		virtual long storageId() = 0;
		virtual PurpleAccount *account() = 0;
		virtual const std::string &jid() = 0;
// 		virtual PurpleValue *getSetting(const char *key) = 0;
		virtual bool hasTransportFeature(int feature) = 0;
		virtual int getFeatures() = 0;
		virtual const std::string &username() = 0;
		virtual const char *getLang() { return "en"; }
// 		virtual GHashTable *settings() {return NULL;}
		virtual bool isConnected() = 0;
		virtual bool readyForConnect() = 0;
		virtual void connect() = 0;
		virtual void disconnected() = 0;
		virtual void setConnected(bool connected) {}
		AdhocData & adhocData() { return m_adhocData; }
		void setAdhocData(AdhocData data) { m_adhocData = data; }
		
		void setProtocolData(void *protocolData) { m_protocolData = protocolData; }
		void *protocolData() { return m_protocolData; }
		RoomData &getRoomData(const std::string &room) { return m_roomData[room]; }
		void setRoomData(const std::string &room, const RoomData &data) { m_roomData[room] = data; }

		guint removeTimer;

	private:
		void *m_protocolData;
		AdhocData m_adhocData;
		std::map<std::string, RoomData> m_roomData;
		GHashTable *m_filetransfers;
		
};

#endif

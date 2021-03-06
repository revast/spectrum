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

#ifndef _HI_USER_H
#define _HI_USER_H

#include <time.h>
#include <gloox/clientbase.h>
#include <gloox/messagesession.h>
#include <gloox/siprofileft.h>
#include <gloox/vcard.h>
#include <glib.h>
#include "purple.h"
#include "abstractuser.h"
#include "account.h"
#include "rostermanager.h"
#include "spectrummessagehandler.h"
#include "settingsmanager.h"

class FiletransferRepeater;

class RosterRow;
class SpectrumTimer;

using namespace gloox;

class User;

// Representation of XMPP User
class User : public AbstractUser, public SpectrumRosterManager, public SpectrumMessageHandler, public SettingsManager {
	public:
		User(JID jid, const std::string &username, const std::string &password, const std::string &userKey, long id, const std::string &encoding, const std::string &language, bool vip);
		virtual ~User();

		// Connects the user to legacy network.
		void connect();

		// Returns true if user has this TransportFeature.
		bool hasTransportFeature(int feature);

		// Handles received presence.
		void receivedPresence(const Presence &presence);
		void forwardStatus(int presence, const std::string &stanzaStatus);

		void purpleBuddyTypingStopped(const std::string &uin);
		void purpleBuddyTyping(const std::string &uin);
		void purpleBuddyTypingPaused(const std::string &uin);
		void connected();
		void disconnected();
		void setConnected(bool connected) { m_connected = connected; }
		void handleVCard(const VCard* vcard);

		std::string actionData;

		// Connected
		bool isConnected() { return m_connected; }

		PurpleAccount *account() { return m_account; }
		int reconnectCount() { return m_reconnectCount; }
		bool isVIP() { return m_vip; }
		bool readyForConnect() { return m_readyForConnect; }
		void setReadyForConnect(bool ready) { m_readyForConnect = ready; }
		const std::string & username() { return m_username; }
		const std::string & jid() { return m_jid; }

		const char *getLang() { return m_lang; }
		void setLang(const char *lang) { g_free(m_lang); m_lang = g_strdup(lang); }

		const std::string & userKey() { return m_userKey; }
		void setFeatures(int f) { m_features = f; }
		int getFeatures() { return m_features; }
		long storageId() { return m_userID; }
		bool loadingBuddiesFromDB() { return m_loadingBuddiesFromDB; }

	private:
		std::string m_jid;			// Jabber ID of this user
		long m_userID;				// userID for Database
		std::string m_userKey;
		PurpleAccount *m_account;	// PurpleAccount to which this user is connected
		bool m_vip;					// true if the user is VIP
		bool m_readyForConnect;		// true if the user user wants to connect and we're ready to do it
		bool m_connected;			// true if this user is connected to legacy account
		int m_reconnectCount;		// number of passed reconnect tries
		std::string m_password;		// password used to connect to legacy network
		std::string m_username;		// legacy network user name
		std::string m_encoding;
		char *m_lang;			// xml:lang
		int m_features;
		bool m_loadingBuddiesFromDB;
		std::string m_photoHash;
		int m_presenceType;
		int m_glooxPresenceType;
		std::string m_statusMessage;
		std::list <Tag *> m_autoConnectRooms;
		SpectrumTimer *m_reconnectTimer;
};

#endif

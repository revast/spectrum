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

#ifndef SPECTRUM_TIMER_H
#define SPECTRUM_TIMER_H

#include <string>
#include "purple.h"
#include "glib.h"

typedef gboolean (*SpectrumTimerCallback)(void *);

// Wrapper of purple_timeout_*.
// SpectrumTimer calls callback function after some time. If callback function
// returns true, timer will start again automatically and function is called
// repeatedly.
class SpectrumTimer {
	public:
		// Creates new Timer.
		// `time` - in ms
		// `callback` - function called after time.
		// `data` - data passed to callback function
		SpectrumTimer (int time, SpectrumTimerCallback callback, void *data = NULL);
		virtual ~SpectrumTimer ();

		// Starts timer. If it's already started, nothing whill happen.
		void start();

		// Stops timer.  If it's already stopped, nothing whill happen.
		void stop();

		// Deletes the timer later. Call this function if you want to delete this
		// SpectrumTimer in its own callback.
		void deleteLater();

		// Don't call this function by hand. It should be private, but it can't be,
		// because purple_timout_add calls static function which has to call public
		// SpectrumTimer::timeout().
		gboolean timeout();
		
		bool isRunning() {return m_id != 0;}

	private:
		void *m_data;						// Data to be passed to callback.
		SpectrumTimerCallback m_callback;	// Callback which is called on timeout.
		int m_timeout;						// Miliseconds.
		guint m_id;							// Timer ID.
		GMutex *m_mutex;					// Mutex.
		bool m_deleteLater;					// True if SpectrumTimer should be remove after callback.
		bool m_inCallback;					// True if deleteLater has been called in callback.
		bool m_startAgain;					// True if start has been called in callback
};

#endif

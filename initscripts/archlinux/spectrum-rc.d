#!/bin/sh

# modified by Moritz Wilhelmy <crap@wzff.de> - configuration now in /etc/conf.d/spectrum
# spectrum (this script) - manage multiple spectrum (XMPP Transport) instances
# archlinux init script created by Nick Campbell <s0ma@theangryfist.net>

# based on script created by Thilo Cestonaro <thilo@cestona.ro>
# Distributable under the terms of the GNU GPL version 2.

. /etc/rc.conf
. /etc/rc.d/functions
. /etc/conf.d/spectrum

check_user() {
	if ! getent passwd $SPECTRUM_USER >/dev/null; then
		printhl "unknown user '$SPECTRUM_USER', please change /etc/conf.d/spectrum"
		exit 1;
	fi
}


case "$1" in
  start)
    check_user
    for i in $SPECTRUM_CONF/*.cfg; do
      if egrep -q '^\s*enable\s*=\s*1\s*$' "$i"; then
        CONF="`basename "$i" .cfg`"
        stat_busy "Starting spectrum instance: ${CONF}"
	spectrumctl --su $SPECTRUM_USER -q -c $i start
    	  if [ $? -gt 0 ]; then
            stat_fail
          else
	    add_daemon spectrum
            stat_done
          fi
      fi
    done
    ;;
  stop)
    for i in $SPECTRUM_CONF/*.cfg; do
      if egrep -q '^\s*enable\s*=\s*1\s*$' "$i"; then
        CONF="`basename $i .cfg`"
        stat_busy "Stopping spectrum instance: ${CONF}"
	spectrumctl -q -c $i stop
        if [ $? -gt 0 ]; then
          stat_fail
        else
          rm_daemon spectrum
          stat_done
        fi
      fi
    done
    ;;
  restart)
    $0 stop
    sleep 3
    $0 start
    ;;
  reload)
    for i in $SPECTRUM_CONF/*.cfg; do
      if egrep -q '^\s*enable\s*=\s*1\s*$' "$i"; then
        CONF="`basename $i .cfg`"
        stat_busy "Reloading spectrum instance: ${CONF}"
	spectrumctl -q -c $i reload

	if [ $? -gt 0 ]; then
          stat_fail
      	else
          stat_done
        fi
      fi
    done
    ;;
  status)
    stat_busy "Checking spectrum status"
    ck_status spectrum-aim
    spectrumctl list
    ;;
  *)
    echo "usage: $0 {start|stop|restart|reload|status}"
esac

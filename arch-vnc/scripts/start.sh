#!/bin/bash

set -e
chsh -s /bin/fish

# Run init.sh if it exists
[[ -f /scripts/init.sh ]] && /scripts/init.sh && rm /scripts/init.sh

# current user
CUSER=${USERNAME:-root}
# add user if specified
if [ ! -z $USERNAME ]; then
	HOMEDIR="/home/$USERNAME"
	# Check if user exists
	if ! id "$USERNAME" &> /dev/null; then
		useradd -m -G wheel -s /usr/bin/fish $USERNAME
		# delete password
		passwd -d $USERNAME
		echo "%wheel ALL=(ALL:ALL) ALL" >> /etc/sudoers
	fi
else
	HOMEDIR="/root"
fi

mkdir -p $HOMEDIR/.vnc/
echo $VNC_PASSWD | vncpasswd -f > $HOMEDIR/.vnc/passwd
chmod 0600 $HOMEDIR/.vnc/passwd
chown $CUSER:$CUSER $HOMEDIR/.vnc/passwd

sudo -u $USERNAME bash /scripts/user.sh

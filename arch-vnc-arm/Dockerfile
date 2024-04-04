ARG VERSION

FROM menci/archlinuxarm

RUN echo "allow-weak-key-signatures" >> /etc/pacman.d/gnupg/gpg.conf

RUN pacman-key --init && pacman-key --populate archlinuxarm

RUN pacman -Syy --noconfirm archlinuxarm-keyring

RUN pacman -Sy --noconfirm pacman

RUN pacman -Syu --noconfirm

RUN pacman -Syu --noconfirm \
	wget tigervnc alacritty \
	which xfce4 xorg python-setuptools ttf-dejavu \
    vim nano htop fish expect sudo

RUN pacman -Sy --noconfirm gcc binutils make glm rapidjson git ccze blender

RUN mkdir -p /CS434CG

COPY ./blender /blender

RUN pacman -U "http://tardis.tiny-vps.com/aarm/packages/o/opensubdiv/opensubdiv-3.5.0-1-aarch64.pkg.tar.xz" --noconfirm

RUN pacman -U /blender/* --noconfirm

RUN rm -rf /blender/

# copy directory contents to /scripts
COPY ./scripts /scripts

WORKDIR /root

EXPOSE 5900 6080

CMD [ "/scripts/start.sh" ]

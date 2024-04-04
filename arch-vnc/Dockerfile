ARG VERSION

FROM archlinux:${VERSION:-latest}

RUN pacman -Syu --noconfirm archlinux-keyring \
	wget tigervnc alacritty \
	which xfce4 xorg python-setuptools ttf-dejavu \
    vim nano htop fish expect sudo

RUN pacman -Syu --noconfirm gcc binutils make glm rapidjson git ccze blender



RUN mkdir -p /CS434CG

# copy directory contents to /scripts
COPY ./scripts /scripts

WORKDIR /root

EXPOSE 5900 6080

CMD [ "/scripts/start.sh" ]

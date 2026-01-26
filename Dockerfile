FROM archlinux:latest

RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm \
      vim \
      gcc \
      base-devel \
      raylib \
      libx11 \
      libxrandr \
      libxi \
      libxcursor \
      libxinerama \
      mesa \
      alsa-lib && \
    pacman -Scc --noconfirm

WORKDIR /app
CMD ["bash"]

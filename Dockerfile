FROM archlinux:latest

RUN pacman-key --init && pacman-key --populate archlinux

RUN pacman -S --noconfirm reflector && \
    reflector --country Italy,Germany,France --age 12 --protocol https --sort rate \
      --save /etc/pacman.d/mirrorlist && \
    sed -i 's/^#ParallelDownloads/ParallelDownloads/' /etc/pacman.conf

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

FROM archlinux:latest

RUN pacman -Syu --noconfirm

RUN pacman -S --noconfirm \
    base-devel \
    gcc \
    make \
    vim \
    raylib \
    libx11 \
    libxrandr \
    libxi \
    libxcursor \
    libxinerama \
    mesa \
    libglvnd \
    alsa-lib \
    vulkan-icd-loader \
    && pacman -Scc --noconfirm

ENV NVIDIA_VISIBLE_DEVICES=all
ENV NVIDIA_DRIVER_CAPABILITIES=graphics,utility,compute

WORKDIR /src
CMD ["bash"]

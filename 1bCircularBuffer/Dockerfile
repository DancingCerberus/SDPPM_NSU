FROM archlinux:latest

RUN pacman -Syu --noconfirm \
    && pacman -S --noconfirm cmake git base-devel tree

COPY . /app

WORKDIR /app

RUN cmake -B /app/build -DCMAKE_BUILD_TYPE=Release

RUN cmake --build /app/build --config Release

WORKDIR /app/build/test

CMD ["./Tests"]

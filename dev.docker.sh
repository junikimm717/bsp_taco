#!/usr/bin/env sh

CONTAINER='bsp-to-taco'
IMAGE='docker.io/junikimm717/nvim2025:finch'
DIR="$(realpath "$(dirname "$0")")"

case "$1" in
  pull|p)
    docker pull "$IMAGE"
    (docker container ls | grep "$CONTAINER" > /dev/null 2>&1) && docker container rm -fv "$CONTAINER"
    ;;
  clear|c)
    (docker container ls -a | grep "$CONTAINER" > /dev/null 2>&1)\
    && {
      docker container kill "$CONTAINER" > /dev/null 2>&1;
      docker container rm "$CONTAINER"
    }
    ;;
  *)
    set +x
    if ! (docker container ls -a | grep "$CONTAINER" > /dev/null 2>&1); then
      docker run\
        -dt\
        --name "$CONTAINER"\
        --group-add $(id -g)\
        -v "$DIR:/workspace"\
        --privileged\
        --rm\
        "$IMAGE"
    fi || exit 1
    docker exec\
      -e ENV=/root/.profile\
      -it "$CONTAINER" /bin/bash
    ;;
esac

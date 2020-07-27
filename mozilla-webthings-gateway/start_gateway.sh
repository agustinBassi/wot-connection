#!/bin/sh

docker run \
--interactive \
--rm \
--env TZ=America/Argentina/Buenos_Aires \
--volume "$PWD"/data:/home/node/.mozilla-iot \
--network="host" \
--log-opt max-size=1m \
--log-opt max-file=10 \
--name webthings-gateway \
mozillaiot/gateway:latest
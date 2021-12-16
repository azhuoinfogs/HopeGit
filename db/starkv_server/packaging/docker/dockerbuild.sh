#!/bin/bash
set -x
$1
docker build --rm -f "Dockerfile" -t starengine/starengine:$1 "."
docker login -u starengine -p ********  #replace the docker registry username and password
docker push starengine/starengine:$1
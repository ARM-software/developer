#!/bin/bash

set -x

source terraform_variables.sh

RED='\033[0;31m'
GREEN="\033[0;92m"
NC='\033[0m' # No Color

if [[ "$1" == "create" ]]; then
  OPERATION="apply"
  MESSAGE="Creating"
  COLOUR=$GREEN
elif [[ "$1" == "destroy" ]]; then
  OPERATION="destroy"
  MESSAGE="Destroying"
  COLOUR=$RED
else
  echo -e "${RED}First argument has to be [create|destroy]${NC}"
  exit -1
fi

#statements
if [[ "$2" == "x86" ]]; then
    echo -e "${COLOUR} $MESSAGE x86 cluster...${NC}"
    bash -c "terraform $OPERATION $TERRAFORM_VARS_X86"
elif [[ "$2" == "aarch64" ]]; then
    echo -e "${COLOUR} $MESSAGE aarch64 cluster...${NC}"
    bash -c "terraform $OPERATION $TERRAFORM_VARS_ARM"
else
  echo -e "${RED}Second argument has to be [x86|aarch64]${NC}"
  exit -1
fi

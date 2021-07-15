#!/bin/bash

set -x

REDIS_MEMTIER_IP=$(head -n 1 REDIS_MEMTIER_ADDRESSES | tr -s '\n')

scp -i "<PRIVATE_KEY>" ubuntu@$REDIS_MEMTIER_IP:* $1

#!/bin/bash

REDIS_MEMTIER_IP=$(head -n 1 REDIS_MEMTIER_ADDRESSES | tr -s '\n' ' ')

ssh -i "<PRIVATE_KEY>" -o StrictHostKeyChecking=no ubuntu@$REDIS_MEMTIER_IP

#!/bin/bash

REDIS_SERVER_IP=$(head -n 1 REDIS_SERVER_ADDRESSES | tr -s '\n' ' ')

ssh -i "<PRIVATE_KEY>" -o StrictHostKeyChecking=no ubuntu@$REDIS_SERVER_IP

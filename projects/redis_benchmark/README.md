# Create environment for running Redis benchmarks on AWS

This repository contains all the files and scripts to create aarch64/x86 clusters
in AWS to run [`memtier`](https://github.com/RedisLabs/memtier_benchmark) Redis benchmarks.

The environment creates separate environment for aarch64 and x86. The [Terraform](https://www.terraform.io)
script creates 2 instances in the same placement group, one for Redis (with The
size and type defined in `terraform_variables.sh`). The instance for memtier is
of the same type as for Redis, but `xlarge` size. Terraform script stores
internal and external DNS names of the instances in files `REDIS_SERVER_ADDRESSES`
and `REDIS_MEMTIER_ADDRESSES`, which can be used to ssh to the instances, and run
benchmarks.

Users can run `ssh_redis_server.sh` and `ssh_redis_memtier.sh` scripts in order
to ssh to the instances. Jumping on memtier instance is necessary to run the
benchmarks.

## Creating/destroying the cluster

First, update `terraform_variables.sh` file with the right variables:
```bash
AARCH64_INSTANCE_TYPE="r6g"
X86_INSTANCE_TYPE="r5"
INSTANCE_SIZE="2xlarge"
VPC="<YOUR VPC>"
AWS_KEY_NAME="<YOUR AWS KEY NAME>"
```

Then, add your private key location to the following files:
```bash
ssh_redis_server.sh
ssh_redis_memtier.sh
download_bench_files.sh
```

If it is the first time running Terraform, initialize the state:
```bash
$ terraform init
```

Then, run the following command:

```bash
$ ./create_cluster.sh <create|destroy> <x86|aarch64>
```

## Running benchmarks on memtier instance

When the cluster is up and running, find the internal DNS name of Redis server
from `REDIS_SERVER_ADDRESSES` file. The first line in the file is public DNS name
of the instance, and the second line is the internal one.

The following runs benchmarks on memtier instance and stores the results in `bench_out`.

```bash
$ ./ssh_redis_memtier

#  example: this command runs memtier with sequential key pattern (100% hit ratio)
$ memtier_benchmark -s <INTERNAL_DNS_NAME> --run-count 5 --data-size=128 --out-file bench_out --ratio 1:1 --threads=5 --key-pattern S:S -c 50
```

After benchmarks are done, you can download the files running  `download_bench_files.sh`.

## Useful links

https://github.com/RedisLabs/memtier_benchmark/issues/71

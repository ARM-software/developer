terraform {
  required_providers {
    aws = {
      source = "hashicorp/aws"
      version = "~> 3.0"
    }
  }
}

#######################################
# Providers
#######################################
# Tells terraform to use the aws APIs
provider "aws" {
  region = var.aws_region
  profile = "default"
}

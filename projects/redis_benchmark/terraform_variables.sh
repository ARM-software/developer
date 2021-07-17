AARCH64_INSTANCE_TYPE="r6g"
X86_INSTANCE_TYPE="r5"
INSTANCE_SIZE="2xlarge"
VPC="<YOUR VPC>"
AWS_KEY_NAME="<YOUR AWS KEY NAME>"

TERRAFORM_VARS_X86="-var=\"instance_type=$X86_INSTANCE_TYPE\" -var=\"instance_size=$INSTANCE_SIZE\" -var=\"vpc=$VPC\" -var=\"ami_id=ami-08962a4068733a2b6\" -var=\"key_name=$AWS_KEY_NAME\""
TERRAFORM_VARS_ARM="-var=\"instance_type=$AARCH64_INSTANCE_TYPE\" -var=\"instance_size=$INSTANCE_SIZE\" -var=\"vpc=$VPC\" -var=\"ami_id=ami-064446ad1d755489e\" -var=\"key_name=$AWS_KEY_NAME\""

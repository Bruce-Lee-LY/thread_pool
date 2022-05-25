# Copyright 2022. All Rights Reserved.
# Author: Bruce-Lee-LY
# Date: 22:42:44 on Tue, May 17, 2022
#
# Description: run sample script

#!/bin/bash

set -euo pipefail

WORK_PATH=$(cd $(dirname $0) && pwd) && cd $WORK_PATH

rm -rf log && mkdir -p log/sample

nohup $WORK_PATH/output/sample/tp_sample > log/sample/tp_sample.log 2>&1 &

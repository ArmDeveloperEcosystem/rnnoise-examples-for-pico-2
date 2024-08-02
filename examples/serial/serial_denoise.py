#
# SPDX-FileCopyrightText: Copyright 2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
# SPDX-License-Identifier: BSD-3-Clause
#

import sys

import serial

port = sys.argv[1]
input_file = sys.argv[2]
output_file = sys.argv[3]

with serial.Serial(port, 115200, timeout=1) as ser:
  with open(input_file, 'rb') as input:
    with open(output_file, 'wb') as output:
      while True:
        in_ = input.read(480 * 2)
        if len(in_) == 0:
          break

        ser.write(in_)
        out = ser.read(480 * 2)
        output.write(out)

        print('.', end='')

print(' done!')

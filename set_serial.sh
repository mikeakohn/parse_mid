#!/usr/bin/env bash

setserial -a -g /dev/ttyUSB0
setserial -v /dev/ttyUSB0 spd_cust divisor 768
setserial -a -g /dev/ttyUSB0


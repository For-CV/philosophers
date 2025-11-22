#/bin/bash

# Script para ejecutar test unitarios

elog=error_log.txt
log=log.txt
log_dir="$(date +%y%m%d%H%M%S)/"
cd tests
mkdir -p $log_dir
mv test_runner_c $log_dir
cd $log_dir
./test_runner_c 2>$elog 1>$log
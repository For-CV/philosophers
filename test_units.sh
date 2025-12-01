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

tests=test_integration.txt

if [ ! -f "$tests" ]; then
    echo "Error: El archivo $tests no existe."
    exit 1
fi

i=0
while read -r linea || [ -n "$linea" ]; do
	$linea &
	PID_PHILO=$!
	wait $PID_PHILO
	EXIT_CODE=$?
	if [ $EXIT_CODE -eq 0 ]; then
    	echo "✅ Prueba Superada: El programa terminó correctamente."
	else
    	echo "❌ Fallo: El programa salió con error (Código $EXIT_CODE) o Segfault."
	fi
	$((i+=1)) 
	echo $i
done < "$tests"
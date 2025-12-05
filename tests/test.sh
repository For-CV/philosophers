#/bin/bash

# Retorna el archivo al que redirigir la salida en función del número de línea
ft_get_log() {
    local log_files=(
        "odd_easy_survive.txt"
        "odd_die.txt"
        "even_10ms_survive.txt"
        "even_difficult_survive?.txt"
        "one_philo.txt"
        "even_die.txt"
        "even_10ms_survive.txt"
        "odd_survive_10t.txt"          
        "large_odd_7t.txt"             
        "large_odd.txt"                
        "even_10ms_survive.txt"        
        "large_even_survive?.txt"      
        "even_short_time.txt"          
        "invalid_input.txt"            
        "invalid_input.txt"            
        "invalid_input.txt"            
        "invalid_input.txt"            
        "invalid_input.txt"            
    )

    echo "${log_files[$1]}"
	
}

# Función para hacer los tests de la parte obligatoria
ft_test_source()
{
i=0
while read -r linea || [ -n "$linea" ]; do
	archivo=$(ft_get_log $i)
	
	# Contar argumentos
	set -- $linea
	arg_count=$#

	if [ $arg_count -eq 6 ]; then
		duracion=60
		echo "Running Valgrind for: $linea"
		timeout --foreground "$duracion" valgrind --show-leak-kinds=all --track-origins=yes --leak-check=full $linea >>"$log_dir$archivo" 2>>"$log_dir$archivo"
		EXIT_CODE=$?
	else
		duracion=5
		timeout --foreground "$duracion" $linea >>"$log_dir$archivo" 2>>"$log_dir$archivo"
		EXIT_CODE=$?
	fi

	if [ $EXIT_CODE -eq 0 ] || [ $EXIT_CODE -eq 124 ]; then
    	echo "✅ Prueba Superada: El programa terminó correctamente."
	else
    	echo "❌ Fallo: El programa salió con error (Código $EXIT_CODE) o Segfault."
	fi
	((i+=1)) 
	echo $i
done < "$tests"
}

# Script para ejecutar test unitarios

elog=error_log.txt
log=log.txt
log_dir="$(date +%y%m%d%H%M%S)/"
cd tests
mkdir -p $log_dir
mv test_runner_c $log_dir
cd $log_dir
./test_runner_c 2>$elog 1>$log

# Script para ejecutar test integrales

cd ..
tests=./test_integration.txt

if [ ! -f "$tests" ]; then
    echo "Error: El archivo $tests no existe."
    exit 1
fi

ft_test_source
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

ft_run_normal_test() {
	local linea="$1"
	local archivo="$2"
	local arg_count="$3"
	local duracion=5
	local test_line=${linea/philo/tester}

	timeout --foreground "$duracion" $test_line >>"$log_dir$archivo" 2>>"$log_dir$archivo"
	local EXIT_CODE=$?

	# Determinar el código de salida esperado
	local EXPECTED_CODE=0
	if [ $arg_count -eq 5 ]; then
		if [[ "$archivo" == *"survive"* ]]; then
			EXPECTED_CODE=124
		fi
	fi

	if [ $EXIT_CODE -eq $EXPECTED_CODE ]; then
    	echo -e "✅ \e[32mEl programa terminó correctamente.\e[0m"
	elif [[ "$archivo" == "invalid_input.txt" ]] && [ $EXIT_CODE -eq 1 ]; then
        echo -e "✅ \e[32mEl programa terminó correctamente (Input inválido).\e[0m"
	else
		if [ $EXPECTED_CODE -eq 124 ]; then
			echo -e "❌ \e[31mFallo: Se esperaba Timeout (124) pero salió con $EXIT_CODE (Probable muerte de filósofo).\e[0m"
		else
    		echo -e "❌ \e[31mFallo: El programa salió con error (Código $EXIT_CODE) o Segfault.\e[0m"
		fi
	fi
}

ft_run_valgrind_test() {
	local linea="$1"
	local archivo="$2"
	local arg_count="$3"
	local test_line=${linea/philo/tester}

	if [ $arg_count -eq 6 ]; then
		local duracion=60
		echo -e "Running Valgrind for: $linea"
		timeout --foreground "$duracion" valgrind --read-var-info=yes --error-exitcode=137 --show-leak-kinds=all --track-origins=yes --leak-check=full $test_line >>"$log_dir/val_logs/$archivo" 2>>"$log_dir/val_logs/$archivo"
		local EXIT_CODE=$?
		if [ $EXIT_CODE -eq 137 ]; then
			echo -e "❌ \e[31m[Valgrind] Fallo: Errores de memoria detectados.\e[0m"
		else
			echo -e "✅ \e[32m[Valgrind] El programa terminó correctamente.\e[0m"
		fi
	fi
}

ft_run_helgrind_test() {
        local linea="$1"
        local archivo="$2"
        local duracion=10
        local test_line=${linea/philo/tester}

        echo -e "Running Helgrind for: $linea"
        timeout --foreground "$duracion" valgrind --tool=helgrind --read-var-info=yes --error-exitcode=138 $test_line >>"$log_dir/hel_logs/$archivo" 2>>"$log_dir/hel_logs/$archivo"
        local EXIT_CODE=$?
        if [ $EXIT_CODE -eq 138 ]; then
                echo -e "❌ \e[31m[Helgrind] Fallo: Errores de concurrencia detectados.\e[0m"
        elif [ $EXIT_CODE -eq 124 ]; then
                if grep -q "ERROR SUMMARY: [1-9]" "$log_dir/hel_logs/$archivo"; then
                        echo -e "❌ \e[31m[Helgrind] Fallo: Errores de concurrencia detectados (Timeout).\e[0m"
                else
                        echo -e "✅ \e[32m[Helgrind] El programa terminó correctamente (Timeout).\e[0m"
                fi
        else
                echo -e "✅ \e[32m[Helgrind] El programa terminó correctamente.\e[0m"
        fi
}
ft_run_tsan_test() {
	local linea="$1"
	local archivo="$2"
	local duracion=10
	local tsan_line=${linea/philo/philo_tsan}

	echo -e "Running TSan for: $tsan_line"
	TSAN_OPTIONS="exitcode=66" timeout --foreground "$duracion" $tsan_line >>"$log_dir/tsan_logs/$archivo" 2>>"$log_dir/tsan_logs/$archivo"
	local EXIT_CODE=$?
	if [ $EXIT_CODE -eq 66 ]; then
		echo -e "❌ \e[31m[TSan] Fallo: Errores de concurrencia detectados (Data Race).\e[0m"
	else
		echo -e "✅ \e[32m[TSan] El programa terminó correctamente.\e[0m"
	fi
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
	
	test=$((i + 1))
	echo -e "\n\e[34mTest $test: $linea\e[0m"

	ft_run_normal_test "$linea" "$archivo" "$arg_count"
	ft_run_valgrind_test "$linea" "$archivo" "$arg_count"
	ft_run_helgrind_test "$linea" "$archivo"
	ft_run_tsan_test "$linea" "$archivo"

	((i+=1))
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
mkdir -p val_logs
mkdir -p hel_logs
mkdir -p tsan_logs
./test_runner_c 2>$elog 1>$log

# Script para ejecutar test integrales

cd ..
tests=./test_integration.txt

if [ ! -f "$tests" ]; then
    echo -e  "❌ \e[31mError: El archivo $tests no existe.\e[0m"
    exit 1
fi

ft_test_source
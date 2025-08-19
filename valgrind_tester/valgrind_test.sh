#!/bin/bash
# filepath: valgrind_tester/valgrind_test.sh
# Testing completo de memory leaks y race conditions
# ✅ SIMPLIFICADO: Ejecutar solo desde valgrind_tester/

echo ""
echo "🧹 =================================================="
echo "🧹 VALGRIND TESTING SUITE - PHILOSOPHERS v3.2"
echo "🧹 SIMPLIFICADO: Ejecutar desde valgrind_tester/"
echo "🧹 =================================================="
echo ""

# ✅ RUTAS FIJAS SIMPLIFICADAS
PHILO_EXECUTABLE="../philo"
LOGS_DIR="./logs"

# Verificar que philo existe
if [ ! -f "$PHILO_EXECUTABLE" ]; then
    echo "❌ ERROR: $PHILO_EXECUTABLE no encontrado"
    echo "💡 Este script debe ejecutarse desde la carpeta valgrind_tester/"
    echo "💡 El ejecutable 'philo' debe estar en el directorio padre"
    echo ""
    echo "📁 Estructura esperada:"
    echo "   03_PHILOSOPHERS_42/"
    echo "   ├── philo                    # ← Ejecutable aquí"
    echo "   └── valgrind_tester/"
    echo "       ├── valgrind_test.sh     # ← Script aquí"
    echo "       └── logs/                # ← Logs aquí"
    echo ""
    exit 1
fi

# Verificar que valgrind está instalado
if ! command -v valgrind &> /dev/null; then
    echo "❌ ERROR: Valgrind no está instalado"
    echo "💡 Instalar con: sudo apt install valgrind"
    echo ""
    exit 1
fi

echo "✅ Valgrind encontrado: $(valgrind --version)"
echo "✅ Ejecutable encontrado: $PHILO_EXECUTABLE"
echo ""

# ✅ CREAR DIRECTORIO DE LOGS
mkdir -p "$LOGS_DIR"
echo "📝 Logs se guardarán en: $LOGS_DIR/"
echo ""

# ✅ FUNCIÓN: Filtrar errores de timeout
filter_timeout_errors() {
    local log_file="$1"
    local filtered_file="${log_file}.filtered"
    
    # Filtrar líneas problemáticas relacionadas con SIGTERM/timeout
    grep -v -E "(Exiting thread still holds.*locks|Process terminating with default action of signal 15|SIGTERM)" "$log_file" > "$filtered_file"
    
    # Recalcular ERROR SUMMARY sin errores de timeout
    local original_errors=$(grep "ERROR SUMMARY:" "$log_file" | grep -o "[0-9]\+ errors" | grep -o "[0-9]\+" 2>/dev/null || echo "0")
    local timeout_errors=$(grep -c "Exiting thread still holds.*locks" "$log_file" 2>/dev/null || echo "0")
    local real_errors=$((original_errors - timeout_errors))
    
    if [ $real_errors -lt 0 ]; then
        real_errors=0
    fi
    
    echo "ERROR SUMMARY: $real_errors errors from contexts (timeout errors filtered)" >> "$filtered_file"
    
    echo "$filtered_file"
}

# ✅ FUNCIÓN: Test simplificado
run_valgrind_test() {
    local test_num="$1"
    local test_name="$2"
    local valgrind_opts="$3"
    local command_args="$4"
    local timeout_val="$5"
    local expected_exit="$6"
    local log_file="$LOGS_DIR/valgrind_${test_num}.log"
    
    echo "🧹 =============================================="
    echo "🧹 VALGRIND TEST $test_num: $test_name"
    echo "🧹 =============================================="
    echo "🔧 Comando: timeout ${timeout_val}s valgrind $valgrind_opts $PHILO_EXECUTABLE $command_args"
    echo "📝 Log: logs/valgrind_${test_num}.log"
    echo "🎯 Exit esperado: $expected_exit"
    echo ""
    echo "🚀 EJECUTANDO..."
    echo "----------------------------------------------"
    
    timeout ${timeout_val}s valgrind $valgrind_opts $PHILO_EXECUTABLE $command_args > "$log_file" 2>&1
    local exit_code=$?
    
    echo ""
    echo "----------------------------------------------"
    
    # Analizar códigos de salida
    case $exit_code in
        0)
            echo "✅ Test completado exitosamente"
            ;;
        1)
            if [ "$expected_exit" = "1" ]; then
                echo "✅ Test falló como esperado (argumentos inválidos)"
            else
                echo "❌ Test falló inesperadamente"
            fi
            ;;
        124)
            echo "⏰ Test terminado por timeout (normal para tests largos)"
            ;;
        *)
            echo "⚠️  Test terminado con código: $exit_code"
            ;;
    esac
    
    # Filtrar errores de timeout para tests de threading
    local filtered_log=""
    if [[ "$valgrind_opts" == *"helgrind"* ]] || [[ "$valgrind_opts" == *"drd"* ]]; then
        filtered_log=$(filter_timeout_errors "$log_file")
    fi
    
    # Analizar memory leaks
    echo ""
    echo "📊 RESUMEN DE MEMORY LEAKS:"
    if [ "$expected_exit" = "1" ]; then
        if grep -q "All heap blocks were freed -- no leaks are possible" "$log_file"; then
            echo "✅ NO HAY MEMORY LEAKS (manejo de errores limpio)"
        elif grep -q "definitely lost" "$log_file"; then
            echo "❌ MEMORY LEAKS EN MANEJO DE ERRORES:"
            grep -E "(definitely lost|indirectly lost)" "$log_file" | head -3
        else
            echo "⚠️  Revisar log para detalles"
        fi
    else
        if grep -q "All heap blocks were freed -- no leaks are possible" "$log_file"; then
            echo "✅ NO HAY MEMORY LEAKS"
        elif grep -q "definitely lost" "$log_file"; then
            echo "❌ MEMORY LEAKS DETECTADOS:"
            grep -E "(definitely lost|indirectly lost|possibly lost)" "$log_file" | head -5
        else
            echo "⚠️  Revisar log file para detalles"
        fi
    fi
    
    # Analizar threading errors
    if [ "$expected_exit" != "1" ] && [ -n "$filtered_log" ]; then
        echo ""
        echo "📊 RESUMEN DE THREADING/MUTEX (filtrado):"
        if grep -q "ERROR SUMMARY: 0 errors" "$filtered_log"; then
            echo "✅ NO HAY ERRORES DE THREADING"
        else
            local timeout_errors_only=$(grep -c "Exiting thread still holds.*locks" "$log_file" 2>/dev/null || echo "0")
            local total_errors=$(grep "ERROR SUMMARY:" "$log_file" | grep -o "[0-9]\+ errors" | grep -o "[0-9]\+" 2>/dev/null || echo "0")
            
            if [ "$timeout_errors_only" -eq "$total_errors" ] 2>/dev/null && [ "$total_errors" -gt 0 ]; then
                echo "✅ SOLO ERRORES DE TIMEOUT (ignorados)"
            else
                echo "⚠️  Errores de threading detectados"
            fi
        fi
    fi
    
    echo ""
    echo ""
}

# ✅ FUNCIÓN: Test simple
run_simple_test() {
    local test_num="$1"
    local test_name="$2"
    local command_args="$3"
    local expected_exit="$4"
    local timeout_val="${5:-5}"
    
    echo "🔧 =============================================="
    echo "🔧 SIMPLE TEST $test_num: $test_name"
    echo "🔧 =============================================="
    echo "🔧 Comando: $PHILO_EXECUTABLE $command_args"
    echo "🎯 Exit esperado: $expected_exit"
    echo ""
    
    timeout ${timeout_val}s $PHILO_EXECUTABLE $command_args > /dev/null 2>&1
    local exit_code=$?
    
    if [ "$exit_code" -eq "$expected_exit" ]; then
        echo "✅ Test PASÓ - Exit code: $exit_code"
    else
        echo "❌ Test FALLÓ - Exit code: $exit_code (esperado: $expected_exit)"
    fi
    echo ""
}

echo "⏳ Comenzando tests completos de Valgrind..."
echo ""

# ========================================
# SECCIÓN A: TESTS DE ARGUMENTOS INVÁLIDOS
# ========================================

echo "🔥 ================================================"
echo "🔥 SECCIÓN A: VALIDACIÓN DE ARGUMENTOS"
echo "🔥 ================================================"
echo ""

run_simple_test "A1" "Sin argumentos" "" "1"
run_simple_test "A2" "Pocos argumentos (3)" "5 800 200" "1"
run_simple_test "A3" "Demasiados argumentos (6)" "5 800 200 200 5 extra" "1"
run_simple_test "A4" "Número negativo" "-5 800 200 200" "1"
run_simple_test "A5" "Cero filósofos" "0 800 200 200" "1"
run_simple_test "A6" "Argumento no numérico" "abc 800 200 200" "1"
run_simple_test "A7" "Número muy grande" "999999999 800 200 200" "1"

run_valgrind_test "A8" \
    "Memory leaks - Argumentos inválidos" \
    "--leak-check=full --show-leak-kinds=all --track-origins=yes" \
    "-1 800 200 200" \
    "3" \
    "1"

# ========================================
# SECCIÓN B: EDGE CASES
# ========================================

echo "⚡ ================================================"
echo "⚡ SECCIÓN B: EDGE CASES"
echo "⚡ ================================================"
echo ""

run_valgrind_test "B1" \
    "Edge case - 1 filósofo" \
    "--leak-check=full --show-leak-kinds=all --track-origins=yes" \
    "1 800 200 200" \
    "3" \
    "0"

run_valgrind_test "B2" \
    "Edge case - 2 filósofos" \
    "--leak-check=full --show-leak-kinds=all" \
    "2 1000 200 200" \
    "5" \
    "0"

run_valgrind_test "B3" \
    "Edge case - Muerte inmediata" \
    "--leak-check=full --show-leak-kinds=all --track-origins=yes" \
    "4 100 200 200" \
    "3" \
    "0"

run_valgrind_test "B4" \
    "Edge case - Tiempos mínimos" \
    "--leak-check=full --show-leak-kinds=all" \
    "3 200 60 60" \
    "3" \
    "0"

run_valgrind_test "B5" \
    "Edge case - Sin límite comidas" \
    "--tool=helgrind --history-level=approx" \
    "3 800 200 200" \
    "6" \
    "124"

run_valgrind_test "B6" \
    "Edge case - Muchos filósofos" \
    "--leak-check=full --show-leak-kinds=all" \
    "10 800 200 200 2" \
    "8" \
    "0"

run_valgrind_test "B7" \
    "Edge case - 1 comida por filósofo" \
    "--leak-check=full --show-leak-kinds=all --track-origins=yes" \
    "4 800 200 200 1" \
    "5" \
    "0"

# ========================================
# SECCIÓN C: CASOS NORMALES
# ========================================

echo "✨ ================================================"
echo "✨ SECCIÓN C: CASOS NORMALES"
echo "✨ ================================================"
echo ""

run_valgrind_test "C1" \
    "Normal - Caso básico" \
    "--leak-check=full --show-leak-kinds=all --track-origins=yes" \
    "4 800 200 200 5" \
    "8" \
    "0"

run_valgrind_test "C2" \
    "Normal - Terminación natural" \
    "--leak-check=full --show-leak-kinds=all --track-origins=yes" \
    "3 800 200 200 3" \
    "8" \
    "0"

run_valgrind_test "C3" \
    "Normal - Race conditions (Helgrind)" \
    "--tool=helgrind --history-level=approx" \
    "5 800 200 200 4" \
    "10" \
    "124"

run_valgrind_test "C4" \
    "Normal - Data races (DRD)" \
    "--tool=drd --check-stack-var=yes" \
    "4 600 150 150 3" \
    "8" \
    "124"

# ========================================
# SECCIÓN D: STRESS TESTS
# ========================================

echo "💥 ================================================"
echo "💥 SECCIÓN D: STRESS TESTS"
echo "💥 ================================================"
echo ""

run_valgrind_test "D1" \
    "Stress - Muchas comidas" \
    "--leak-check=full --show-leak-kinds=all" \
    "4 800 200 200 10" \
    "15" \
    "0"

run_valgrind_test "D2" \
    "Stress - Threading intensivo" \
    "--tool=helgrind --history-level=none" \
    "8 800 100 100 5" \
    "12" \
    "124"

run_valgrind_test "D3" \
    "Stress - Memory muchos filósofos" \
    "--leak-check=full --show-leak-kinds=all" \
    "15 800 200 200 3" \
    "12" \
    "0"

run_valgrind_test "D4" \
    "Stress - Timing crítico" \
    "--tool=helgrind --history-level=approx" \
    "6 500 100 100" \
    "10" \
    "124"

# ========================================
# SECCIÓN E: TESTS DE MEMORIA ESPECÍFICOS
# ========================================

echo "🧠 ================================================"
echo "🧠 SECCIÓN E: TESTS DE MEMORIA ESPECÍFICOS"
echo "🧠 ================================================"
echo ""

run_valgrind_test "E1" \
    "Memoria - Invalid access" \
    "--tool=memcheck --track-origins=yes --read-var-info=yes" \
    "3 400 150 100 2" \
    "6" \
    "0"

run_valgrind_test "E2" \
    "Memoria - Uninitialized" \
    "--tool=memcheck --track-origins=yes --track-fds=yes" \
    "4 500 200 150 3" \
    "8" \
    "0"

run_valgrind_test "E3" \
    "Memoria - Leaks detallados" \
    "--leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes" \
    "5 600 200 200 2" \
    "8" \
    "0"

echo "🎉 =================================================="
echo "🎉 TODOS LOS TESTS COMPLETADOS"
echo "🎉 =================================================="
echo ""

# ========================================
# RESUMEN FINAL SIMPLIFICADO
# ========================================

echo "📋 RESUMEN FINAL:"
echo "=================="

echo ""
echo "🔥 VALIDACIÓN DE ARGUMENTOS:"
echo -n "   Test A8: "
if [ -f "$LOGS_DIR/valgrind_A8.log" ]; then
    if grep -q "All heap blocks were freed" "$LOGS_DIR/valgrind_A8.log"; then
        echo "✅ SIN LEAKS en manejo de errores"
    else
        echo "❌ LEAKS en manejo de errores"
    fi
else
    echo "⚠️  Log no encontrado"
fi

echo ""
echo "⚡ EDGE CASES (Memory):"
for test in B1 B2 B3 B4 B6 B7; do
    echo -n "   Test $test: "
    if [ -f "$LOGS_DIR/valgrind_${test}.log" ]; then
        if grep -q "All heap blocks were freed" "$LOGS_DIR/valgrind_${test}.log"; then
            echo "✅ SIN LEAKS"
        else
            echo "❌ CON LEAKS"
        fi
    else
        echo "⚠️  No ejecutado"
    fi
done

echo ""
echo "✨ CASOS NORMALES:"
for test in C1 C2; do
    echo -n "   Test $test (Memory): "
    if [ -f "$LOGS_DIR/valgrind_${test}.log" ]; then
        if grep -q "All heap blocks were freed" "$LOGS_DIR/valgrind_${test}.log"; then
            echo "✅ SIN LEAKS"
        else
            echo "❌ CON LEAKS"
        fi
    else
        echo "⚠️  No ejecutado"
    fi
done

for test in C3 C4; do
    echo -n "   Test $test (Threading): "
    if [ -f "$LOGS_DIR/valgrind_${test}.log.filtered" ]; then
        if grep -q "ERROR SUMMARY: 0 errors" "$LOGS_DIR/valgrind_${test}.log.filtered"; then
            echo "✅ SIN ERRORES REALES"
        else
            echo "❌ CON ERRORES REALES"
        fi
    else
        echo "⚠️  Revisar log"
    fi
done

echo ""
echo "💥 STRESS TESTS:"
for test in D1 D3; do
    echo -n "   Test $test (Memory): "
    if [ -f "$LOGS_DIR/valgrind_${test}.log" ]; then
        if grep -q "All heap blocks were freed" "$LOGS_DIR/valgrind_${test}.log"; then
            echo "✅ SIN LEAKS"
        else
            echo "❌ CON LEAKS"
        fi
    else
        echo "⚠️  No ejecutado"
    fi
done

echo ""
echo "🧠 TESTS DE MEMORIA:"
for test in E1 E2 E3; do
    echo -n "   Test $test: "
    if [ -f "$LOGS_DIR/valgrind_${test}.log" ]; then
        if grep -q "All heap blocks were freed" "$LOGS_DIR/valgrind_${test}.log"; then
            echo "✅ SIN LEAKS"
        else
            echo "❌ CON LEAKS"
        fi
    else
        echo "⚠️  No ejecutado"
    fi
done

echo ""
echo "💡 COMANDOS ÚTILES:"
echo "   📄 Ver logs de argumentos: ls logs/valgrind_A*.log"
echo "   📄 Ver logs de edge cases: ls logs/valgrind_B*.log"
echo "   📄 Ver logs normales: ls logs/valgrind_C*.log"
echo "   📄 Ver logs de stress: ls logs/valgrind_D*.log"
echo "   📄 Ver logs de memoria: ls logs/valgrind_E*.log"
echo "   📄 Ver solo errores: grep -r \"ERROR SUMMARY\" logs/ | grep -v \"0 errors\""
echo "   📄 Ver solo leaks: grep -r \"definitely lost\" logs/"
echo "   🧹 Limpiar logs: rm logs/valgrind_*.log*"
echo ""

# Estadísticas finales
total_tests=$(ls "$LOGS_DIR"/valgrind_*.log 2>/dev/null | wc -l)
clean_memory=$(grep -l "All heap blocks were freed" "$LOGS_DIR"/valgrind_*.log 2>/dev/null | wc -l)
echo "📊 ESTADÍSTICAS:"
echo "   🔄 Tests ejecutados: $total_tests"
echo "   ✅ Tests sin memory leaks: $clean_memory"
if [ "$total_tests" -gt 0 ]; then
    echo "   📈 Porcentaje limpio: $(( clean_memory * 100 / total_tests ))%"
fi
echo ""

echo "ℹ️  ESTRUCTURA:"
echo "   📁 valgrind_tester/valgrind_test.sh  # ← Script"
echo "   📁 valgrind_tester/logs/             # ← Logs"
echo "   📁 ../philo                          # ← Ejecutable"
echo ""
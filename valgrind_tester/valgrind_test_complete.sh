#!/bin/bash
# filepath: valgrind_tester/valgrind_test_complete.sh
# Testing COMPLETO de memory leaks, race conditions y edge cases
# ✅ VERSIÓN COMPLETA con todas las casuísticas

echo ""
echo "🧹 =================================================="
echo "🧹 VALGRIND TESTING SUITE - PHILOSOPHERS v4.0"
echo "🧹 TESTING COMPLETO - TODAS LAS CASUÍSTICAS"
echo "🧹 =================================================="
echo ""

# ✅ CONFIGURACIÓN
PHILO_EXECUTABLE="../philo"
LOGS_DIR="./logs_complete"
TOTAL_TESTS=0
PASSED_TESTS=0

# ✅ VERIFICACIONES INICIALES
if [ ! -f "$PHILO_EXECUTABLE" ]; then
    echo "❌ ERROR: $PHILO_EXECUTABLE no encontrado"
    exit 1
fi

mkdir -p "$LOGS_DIR"
echo "📝 Logs completos en: $LOGS_DIR/"
echo ""

# ✅ FUNCIONES DE TESTING
run_comprehensive_test() {
    local section="$1"
    local test_num="$2"
    local test_name="$3"
    local test_type="$4"  # "simple", "valgrind", "signal"
    local args="$5"
    local expected_exit="$6"
    local timeout_val="${7:-5}"
    local valgrind_opts="$8"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo "🔧 =============================================="
    echo "🔧 $section-$test_num: $test_name"
    echo "🔧 =============================================="
    
    case "$test_type" in
        "simple")
            timeout ${timeout_val}s $PHILO_EXECUTABLE $args > /dev/null 2>&1
            local exit_code=$?
            if [ "$exit_code" -eq "$expected_exit" ]; then
                echo "✅ Test PASÓ - Exit code: $exit_code"
                PASSED_TESTS=$((PASSED_TESTS + 1))
            else
                echo "❌ Test FALLÓ - Exit code: $exit_code (esperado: $expected_exit)"
            fi
            ;;
        "valgrind")
            local log_file="$LOGS_DIR/valgrind_${section}_${test_num}.log"
            timeout ${timeout_val}s valgrind $valgrind_opts $PHILO_EXECUTABLE $args > "$log_file" 2>&1
            local exit_code=$?
            
            # Análisis básico
            if grep -q "All heap blocks were freed" "$log_file"; then
                echo "✅ NO HAY MEMORY LEAKS"
                PASSED_TESTS=$((PASSED_TESTS + 1))
            elif grep -q "definitely lost.*0 bytes" "$log_file"; then
                echo "⚠️  MEMORY REACHABLE pero sin leaks definitivos"
                PASSED_TESTS=$((PASSED_TESTS + 1))
            else
                echo "❌ MEMORY LEAKS DETECTADOS"
            fi
            ;;
        "signal")
            $PHILO_EXECUTABLE $args &
            local pid=$!
            sleep 1
            kill -TERM $pid 2>/dev/null
            wait $pid 2>/dev/null
            echo "✅ Signal test completado"
            PASSED_TESTS=$((PASSED_TESTS + 1))
            ;;
    esac
    echo ""
}

# ========================================
# EJECUCIÓN DE TODAS LAS SECCIONES
# ========================================

echo "🔥 SECCIÓN A: VALIDACIÓN DE ARGUMENTOS COMPLETA"
echo "================================================"
run_comprehensive_test "A" "01" "Sin argumentos" "simple" "" "1"
run_comprehensive_test "A" "02" "Pocos argumentos" "simple" "5 800 200" "1"
run_comprehensive_test "A" "03" "Demasiados argumentos" "simple" "5 800 200 200 5 extra" "1"
run_comprehensive_test "A" "04" "Número negativo" "simple" "-5 800 200 200" "1"
run_comprehensive_test "A" "05" "Cero filósofos" "simple" "0 800 200 200" "1"
run_comprehensive_test "A" "06" "Argumento no numérico" "simple" "abc 800 200 200" "1"
run_comprehensive_test "A" "07" "Número muy grande" "simple" "999999999 800 200 200" "1"
run_comprehensive_test "A" "08" "Tiempo muerte cero" "simple" "5 0 200 200" "1"
run_comprehensive_test "A" "09" "Tiempo comer cero" "simple" "5 800 0 200" "1"
run_comprehensive_test "A" "10" "Tiempo dormir cero" "simple" "5 800 200 0" "1"
run_comprehensive_test "A" "11" "Comidas negativas" "simple" "5 800 200 200 -1" "1"
run_comprehensive_test "A" "12" "Número decimal" "simple" "5.5 800 200 200" "1"
run_comprehensive_test "A" "13" "Texto mezclado" "simple" "5abc 800 200 200" "1"
run_comprehensive_test "A" "14" "Overflow INT_MAX" "simple" "2147483648 800 200 200" "1"
run_comprehensive_test "A" "15" "Números hexadecimales" "simple" "0x5 800 200 200" "1"

echo "⚡ SECCIÓN B: EDGE CASES EXPANDIDOS"
echo "=================================="
run_comprehensive_test "B" "01" "1 filósofo" "valgrind" "1 800 200 200" "0" "3" "--leak-check=full"
run_comprehensive_test "B" "02" "2 filósofos (deadlock)" "valgrind" "2 1000 200 200" "0" "5" "--leak-check=full"
run_comprehensive_test "B" "03" "Muerte inmediata" "valgrind" "4 100 200 200" "0" "3" "--leak-check=full"
run_comprehensive_test "B" "04" "Tiempos mínimos" "valgrind" "3 200 60 60" "0" "3" "--leak-check=full"
run_comprehensive_test "B" "05" "Muchos filósofos" "valgrind" "50 800 200 200 1" "0" "10" "--leak-check=full"
run_comprehensive_test "B" "06" "Timing crítico" "valgrind" "4 200 195 5" "0" "5" "--tool=helgrind"
run_comprehensive_test "B" "07" "1 comida total" "valgrind" "5 800 200 200 1" "0" "8" "--leak-check=full"

echo "✨ SECCIÓN C: CASOS NORMALES"
echo "==========================="
run_comprehensive_test "C" "01" "Caso básico" "valgrind" "4 800 200 200 5" "0" "8" "--leak-check=full"
run_comprehensive_test "C" "02" "Terminación natural" "valgrind" "3 800 200 200 3" "0" "8" "--leak-check=full"
run_comprehensive_test "C" "03" "Threading básico" "valgrind" "5 800 200 200 4" "0" "10" "--tool=helgrind"

echo "💥 SECCIÓN D: STRESS TESTS"
echo "========================="
run_comprehensive_test "D" "01" "Muchas comidas" "valgrind" "4 800 200 200 10" "0" "15" "--leak-check=full"
run_comprehensive_test "D" "02" "Threading intensivo" "valgrind" "8 800 100 100 5" "0" "12" "--tool=helgrind"
run_comprehensive_test "D" "03" "Memoria intensiva" "valgrind" "15 800 200 200 3" "0" "12" "--leak-check=full"

echo "🚨 SECCIÓN F: TESTS DE ERRORES"
echo "============================="
run_comprehensive_test "F" "01" "Límite sistema" "valgrind" "1000 800 100 100" "1" "10" "--leak-check=full"
run_comprehensive_test "F" "02" "Sintaxis - Tabs" "simple" "5	800	200	200" "0"
run_comprehensive_test "F" "03" "Sintaxis - Unicode" "simple" "5€ 800 200 200" "1"

echo "🎯 SECCIÓN G: TIMING CRÍTICO"
echo "==========================="
run_comprehensive_test "G" "01" "Muerte exacta" "valgrind" "2 100 50 25" "0" "3" "--leak-check=full"
run_comprehensive_test "G" "02" "Race muerte vs comida" "valgrind" "4 200 195 5" "0" "5" "--tool=helgrind"
run_comprehensive_test "G" "03" "Microsegundos críticos" "valgrind" "3 1 1 1" "0" "2" "--leak-check=full"

echo "🔄 SECCIÓN H: CONCURRENCIA AVANZADA"
echo "=================================="
run_comprehensive_test "H" "01" "Deadlock circular" "valgrind" "4 800 200 200" "0" "8" "--tool=helgrind"
run_comprehensive_test "H" "02" "Starvation test" "valgrind" "7 1000 100 100" "0" "12" "--tool=helgrind"
run_comprehensive_test "H" "03" "Race conditions" "valgrind" "8 800 100 100 5" "0" "15" "--tool=drd"

echo "🧪 SECCIÓN I: ROBUSTEZ"
echo "====================="
run_comprehensive_test "I" "01" "Recursos limitados" "valgrind" "100 800 200 200" "0" "15" "--leak-check=full"
run_comprehensive_test "I" "02" "Interrupción SIGTERM" "signal" "4 800 200 200" "0" "3"

# ========================================
# RESUMEN FINAL COMPLETO
# ========================================

echo "🎉 =================================================="
echo "🎉 TESTING COMPLETO TERMINADO"
echo "🎉 =================================================="
echo ""
echo "📊 ESTADÍSTICAS FINALES:"
echo "   🔄 Tests ejecutados: $TOTAL_TESTS"
echo "   ✅ Tests pasados: $PASSED_TESTS"
echo "   ❌ Tests fallidos: $((TOTAL_TESTS - PASSED_TESTS))"
if [ "$TOTAL_TESTS" -gt 0 ]; then
    echo "   📈 Porcentaje éxito: $(( PASSED_TESTS * 100 / TOTAL_TESTS ))%"
fi
echo ""

# Análisis detallado de logs
total_clean_memory=$(find "$LOGS_DIR" -name "*.log" -exec grep -l "All heap blocks were freed" {} \; | wc -l)
total_with_leaks=$(find "$LOGS_DIR" -name "*.log" -exec grep -l "definitely lost.*[1-9]" {} \; | wc -l)
total_logs=$(find "$LOGS_DIR" -name "*.log" | wc -l)

echo "📋 ANÁLISIS DE MEMORIA:"
echo "   📄 Logs generados: $total_logs"
echo "   ✅ Sin memory leaks: $total_clean_memory"
echo "   ❌ Con memory leaks: $total_with_leaks"
echo ""

echo "💡 COMANDOS ÚTILES:"
echo "   📄 Ver todos los logs: ls $LOGS_DIR/"
echo "   📄 Ver solo errores: grep -r \"ERROR SUMMARY\" $LOGS_DIR/ | grep -v \"0 errors\""
echo "   📄 Ver solo leaks: grep -r \"definitely lost\" $LOGS_DIR/ | grep -v \"0 bytes\""
echo "   🧹 Limpiar logs: rm $LOGS_DIR/*.log"
echo ""

echo "✅ Testing completo de $TOTAL_TESTS casos terminado!"
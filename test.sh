#!/bin/bash
# test.sh - Suite completa de testing para philosophers (sin valgrind)
# Basado en valgrind_test_complete.sh - TODAS LAS CASUÍSTICAS

echo ""
echo " ==============================================="
echo "  TESTING PHILOSOPHERS v2.0"
echo " ==============================================="
echo ""

# CONFIGURACIÓN
TOTAL_TESTS=0
PASSED_TESTS=0

# Función para ejecutar test con formato expandido
# Función para ejecutar test con formato expandido
run_test() {
    local section="$1"
    local test_num="$2"
    local test_name="$3"
    local command="$4"
    local timeout_val="$5"
    local expected_exit="$6"
    local expected_desc="$7"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo " =============================================="
    echo "  $section-$test_num: $test_name"
    echo " =============================================="
    echo "🔧 Comando: 		$command"
    echo "⏰ Timeout: 		${timeout_val}s"
    echo "🎯 Exit esperado: 	$expected_exit"
    echo "✅ Esperado: 		$expected_desc"
    echo "----------------------------------------------"
    
    timeout ${timeout_val}s $command > /dev/null 2>&1
    local exit_code=$?
    
    # ✅ CORRECCIÓN: Análisis del resultado mejorado
    if [ $exit_code -eq 124 ]; then
        # Timeout ocurrido
        if [ "$expected_exit" = "timeout" ]; then
            echo "✅ OK - Timeout esperado (comportamiento correcto)"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo "⏰ Test terminado por timeout (no esperado)"
        fi
    elif [ "$expected_exit" = "timeout" ]; then
        # Se esperaba timeout pero terminó normalmente
        echo "❌ FALLO - El programa terminó cuando debería seguir ejecutándose"
    elif [ $exit_code -eq $expected_exit ]; then
        # Exit code correcto
        echo "✅ OK - Exit code: $exit_code"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        # Exit code incorrecto
        echo "❌ FALLO - Exit code: $exit_code (esperado: $expected_exit)"
    fi    
    
    echo "----------------------------------------------"
    echo ""
}

# Verificar que philo existe
if [ ! -f "./philo" ]; then
    echo "❌ ERROR: ./philo no encontrado"
    echo "💡 Solución: Ejecuta 'make' primero"
    echo ""
    exit 1
fi

echo "✅ Ejecutable ./philo encontrado"
echo ""

# ========================================
# SECCIÓN A: VALIDACIÓN DE ARGUMENTOS COMPLETA
# ========================================

echo "🔥 SECCIÓN A: VALIDACIÓN DE ARGUMENTOS COMPLETA"
echo "================================================"

run_test "A" "01" "Sin argumentos" \
    "./philo" "2" "1" "Error: argumentos insuficientes"

run_test "A" "02" "Pocos argumentos" \
    "./philo 5 800 200" "2" "1" "Error: argumentos insuficientes"

run_test "A" "03" "Demasiados argumentos" \
    "./philo 5 800 200 200 5 extra" "2" "1" "Error: demasiados argumentos"

run_test "A" "04" "Número negativo" \
    "./philo -5 800 200 200" "2" "1" "Error: números negativos"

run_test "A" "05" "Cero filósofos" \
    "./philo 0 800 200 200" "2" "1" "Error: cero filósofos inválido"

run_test "A" "06" "Argumento no numérico" \
    "./philo abc 800 200 200" "2" "1" "Error: argumentos no numéricos"

run_test "A" "07" "Número muy grande" \
    "./philo 999999999 800 200 200" "2" "1" "Error: número excesivo"

run_test "A" "08" "Tiempo muerte cero" \
    "./philo 5 0 200 200" "2" "1" "Error: tiempo muerte cero"

run_test "A" "09" "Tiempo comer cero" \
    "./philo 5 800 0 200" "2" "1" "Error: tiempo comer cero"

run_test "A" "10" "Tiempo dormir cero" \
    "./philo 5 800 200 0" "2" "1" "Error: tiempo dormir cero"

run_test "A" "11" "Comidas negativas" \
    "./philo 5 800 200 200 -1" "2" "1" "Error: comidas negativas"

run_test "A" "12" "Número decimal" \
    "./philo 5.5 800 200 200" "2" "1" "Error: números decimales"

run_test "A" "13" "Texto mezclado" \
    "./philo 5abc 800 200 200" "2" "1" "Error: texto mezclado con números"

run_test "A" "14" "Overflow INT_MAX" \
    "./philo 2147483648 800 200 200" "2" "1" "Error: overflow entero"

run_test "A" "15" "Números hexadecimales" \
    "./philo 0x5 800 200 200" "2" "1" "Error: formato hexadecimal"

# ========================================
# SECCIÓN B: EDGE CASES EXPANDIDOS
# ========================================

echo "⚡ SECCIÓN B: EDGE CASES EXPANDIDOS"
echo "=================================="

run_test "B" "01" "1 filósofo debe morir" \
    "./philo 1 800 200 200" "3" "0" "1 filósofo toma fork y muere"

run_test "B" "02" "2 filósofos alternancia" \
    "./philo 2 1000 200 200" "5" "timeout" "Alternancia perfecta sin muerte"

run_test "B" "03" "Muerte inmediata - timing imposible" \
    "./philo 4 100 200 200" "3" "0" "die_time < eat_time → muerte inmediata"

run_test "B" "04" "Tiempos mínimos viables" \
    "./philo 3 200 60 60" "3" "timeout" "Tiempos mínimos pero funcionales"

run_test "B" "05" "Muchos filósofos con límite" \
    "./philo 50 800 200 200 1" "10" "0" "50 filósofos, cada uno come 1 vez"

run_test "B" "06" "Timing crítico - muerte probable" \
    "./philo 4 200 195 5" "5" "0" "Timing muy ajustado, alta probabilidad muerte"

run_test "B" "07" "Una comida total por filósofo" \
    "./philo 5 800 200 200 1" "8" "0" "Cada filósofo come exactamente 1 vez"

# ========================================
# SECCIÓN C: CASOS NORMALES ESTABLES
# ========================================

echo "✨ SECCIÓN C: CASOS NORMALES ESTABLES"
echo "====================================="

run_test "C" "01" "Caso básico estable" \
    "./philo 4 800 200 200 5" "8" "0" "4 filósofos, 5 comidas cada uno"

run_test "C" "02" "Terminación natural" \
    "./philo 3 800 200 200 3" "8" "0" "3 filósofos, 3 comidas, terminación limpia"

run_test "C" "03" "Threading básico sin muerte" \
    "./philo 5 800 200 200 4" "10" "0" "5 filósofos, 4 comidas, sin muertes"

run_test "C" "04" "Equilibrio perfecto" \
    "./philo 4 600 200 200" "5" "timeout" "Timing perfectamente balanceado"

run_test "C" "05" "Número impar de filósofos" \
    "./philo 7 800 200 200 2" "12" "0" "7 filósofos (impar), 2 comidas cada uno"

# ========================================
# SECCIÓN D: STRESS TESTS
# ========================================

echo "💥 SECCIÓN D: STRESS TESTS"
echo "========================="

run_test "D" "01" "Muchas comidas por filósofo" \
    "./philo 4 800 200 200 10" "15" "0" "Cada filósofo come 10 veces"

run_test "D" "02" "Threading intensivo" \
    "./philo 8 800 100 100 5" "12" "0" "8 threads, timing rápido"

run_test "D" "03" "Memoria intensiva" \
    "./philo 15 800 200 200 3" "12" "0" "15 filósofos simultáneos"

run_test "D" "04" "Estrés extremo" \
    "./philo 20 1000 50 50 5" "15" "0" "20 filósofos, timing agresivo"

run_test "D" "05" "Límite sistema" \
    "./philo 100 800 200 200 1" "10" "0" "100 filósofos, 1 comida cada uno"

# ========================================
# SECCIÓN E: TESTS DE MUERTE GARANTIZADA
# ========================================

echo "💀 SECCIÓN E: TESTS DE MUERTE GARANTIZADA"
echo "========================================="

run_test "E" "01" "Muerte garantizada - timing corto" \
    "./philo 4 150 200 100" "2" "0" "die_time=150ms << ciclo=300ms"

run_test "E" "02" "Imposible completar comida" \
    "./philo 4 100 200 50" "2" "0" "die_time < eat_time → muerte segura"

run_test "E" "03" "Conflicto con 3 filósofos" \
    "./philo 3 180 150 100" "2" "0" "3 filósofos compiten → muerte probable"

run_test "E" "04" "Timing extremadamente ajustado" \
    "./philo 4 50 100 50" "2" "0" "Solo 50ms para vivir"

run_test "E" "05" "Muerte exacta en límite" \
    "./philo 2 100 50 25" "3" "0" "Timing calculado para muerte exacta"

run_test "E" "06" "Microsegundos críticos" \
    "./philo 3 1 1 1" "2" "0" "Tiempos mínimos absolutos"

# ========================================
# SECCIÓN F: CASOS ESPECIALES Y SINTAXIS
# ========================================

echo "🚨 SECCIÓN F: CASOS ESPECIALES Y SINTAXIS"
echo "========================================="

run_test "F" "01" "Espacios múltiples" \
    "./philo  5   800   200   200" "5" "timeout" "Espacios múltiples en argumentos"

run_test "F" "02" "Tabs como separadores" \
    "./philo 5	800	200	200" "5" "timeout" "Tabs entre argumentos"

run_test "F" "03" "Números con ceros iniciales" \
    "./philo 05 0800 0200 0200" "5" "timeout" "Números con padding de ceros"

run_test "F" "04" "Solo comer (sleep mínimo)" \
    "./philo 4 800 500 1" "5" "timeout" "Tiempo de dormir mínimo"

run_test "F" "05" "Solo dormir (eat mínimo)" \
    "./philo 4 800 1 500" "5" "timeout" "Tiempo de comer mínimo"

# ========================================
# SECCIÓN G: TIMING CRÍTICO AVANZADO
# ========================================

echo "🎯 SECCIÓN G: TIMING CRÍTICO AVANZADO"
echo "====================================="

run_test "G" "01" "Race muerte vs comida" \
    "./philo 4 200 195 5" "5" "0" "Race condition muerte/comida"

run_test "G" "02" "Precisión de milisegundos" \
    "./philo 2 400 200 200 5" "5" "0" "Timing exacto de 200ms"

run_test "G" "03" "Timing balanceado crítico" \
    "./philo 5 310 150 150" "3" "0" "Timing muy ajustado pero viable"

run_test "G" "04" "Alternancia perfecta" \
    "./philo 2 500 200 200" "8" "timeout" "2 filósofos, alternancia perfecta"

run_test "G" "05" "Timing extremo supervivencia" \
    "./philo 4 310 100 100 5" "10" "0" "Timing crítico con límite comidas"

# ========================================
# SECCIÓN H: ROBUSTEZ Y CONCURRENCIA
# ========================================

echo "🔄 SECCIÓN H: ROBUSTEZ Y CONCURRENCIA"
echo "====================================="

run_test "H" "01" "Deadlock potencial" \
    "./philo 4 800 200 200" "8" "timeout" "4 filósofos, detectar deadlocks"

run_test "H" "02" "Starvation test" \
    "./philo 7 1000 100 100" "12" "timeout" "7 filósofos, test de inanición"

run_test "H" "03" "Concurrencia máxima" \
    "./philo 8 800 100 100 5" "15" "0" "8 threads concurrentes"

run_test "H" "04" "Recursos limitados" \
    "./philo 100 800 200 200" "15" "timeout" "100 filósofos, límite recursos"

run_test "H" "05" "Threading masivo" \
    "./philo 50 1000 100 100 2" "20" "0" "50 threads, 2 comidas cada uno"

# ========================================
# SECCIÓN I: LÍMITES Y EDGE CASES EXTREMOS
# ========================================

echo "🧪 SECCIÓN I: LÍMITES Y EDGE CASES EXTREMOS"
echo "==========================================="

run_test "I" "01" "Un solo ciclo completo" \
    "./philo 10 800 200 200 1" "15" "0" "10 filósofos, 1 comida cada uno"

run_test "I" "02" "Muchas comidas pequeñas" \
    "./philo 3 800 50 50 20" "25" "0" "3 filósofos, 20 comidas rápidas"

run_test "I" "03" "Tiempo vida largo" \
    "./philo 4 5000 1000 1000" "8" "timeout" "Tiempos muy largos"

run_test "I" "04" "Equilibrio exacto" \
    "./philo 5 500 200 200 3" "12" "0" "Timing exactamente calculado"

run_test "I" "05" "Máximo stress sostenible" \
    "./philo 12 800 150 150 5" "20" "0" "12 filósofos, timing moderado"

# ========================================
# RESUMEN FINAL COMPLETO
# ========================================

echo " =================================================="
echo "  TESTING COMPLETO TERMINADO"
echo " =================================================="
echo ""
echo "📊 ESTADÍSTICAS FINALES:"
echo "   🔄 Tests ejecutados: $TOTAL_TESTS"
echo "   ✅ Tests pasados: $PASSED_TESTS"
echo "   ❌ Tests fallidos: $((TOTAL_TESTS - PASSED_TESTS))"
if [ "$TOTAL_TESTS" -gt 0 ]; then
    echo "   📈 Porcentaje éxito: $(( PASSED_TESTS * 100 / TOTAL_TESTS ))%"
fi
echo ""

echo "📋 RESUMEN POR SECCIONES:"
echo "   🔥 Sección A: Validación argumentos (15 tests)"
echo "   ⚡ Sección B: Edge cases (7 tests)"
echo "   ✨ Sección C: Casos normales (5 tests)"
echo "   💥 Sección D: Stress tests (5 tests)"
echo "   💀 Sección E: Muerte garantizada (6 tests)"
echo "   🚨 Sección F: Casos especiales (5 tests)"
echo "   🎯 Sección G: Timing crítico (5 tests)"
echo "   🔄 Sección H: Concurrencia (5 tests)"
echo "   🧪 Sección I: Límites extremos (5 tests)"
echo "📊 TOTAL: $TOTAL_TESTS casos de test"
echo ""

echo "🔧 INSTRUCCIONES:"
echo "   - Tests con timeout normal indican funcionamiento correcto"
echo ""
echo "✅ Testing $TOTAL_TESTS casos terminado!"
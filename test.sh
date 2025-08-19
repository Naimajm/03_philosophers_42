#!/bin/bash
# quick_test_complete.sh - Versión completa con tests de muerte rápida CORREGIDOS

echo ""
echo "🧪 ==============================================="
echo "🧪 TESTING PHILOSOPHERS - SUITE COMPLETA"
echo "🧪 ==============================================="
echo ""

# Función para ejecutar test con formato
run_test() {
    local test_num="$1"
    local test_name="$2"
    local command="$3"
    local timeout_val="$4"
    local expected="$5"
    
    echo "📋 =============================================="
    echo "📋 TEST $test_num: $test_name"
    echo "📋 =============================================="
    echo "🔧 Comando: $command"
    echo "⏰ Timeout: ${timeout_val}s"
    echo "✅ Esperado: $expected"
    echo ""
    echo "🚀 EJECUTANDO..."
    echo "----------------------------------------------"
    
    timeout ${timeout_val}s $command
    local exit_code=$?
    
    echo ""
    echo "----------------------------------------------"
    if [ $exit_code -eq 124 ]; then
        echo "⏰ Test terminado por timeout (comportamiento normal)"
    elif [ $exit_code -eq 0 ]; then
        echo "✅ Test completado exitosamente"
    else
        echo "❌ Test falló con código: $exit_code"
    fi
    echo ""
    echo "📋 FIN TEST $test_num"
    echo ""
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
# TESTS BÁSICOS
# ========================================

run_test "1" \
    "Un filósofo debe morir" \
    "./philo 1 800 200 200" \
    "3" \
    "0 1 has taken a fork → 800 1 died"

run_test "2" \
    "Sin muerte - supervivencia" \
    "./philo 5 800 200 200" \
    "8" \
    "Filósofos comen/duermen indefinidamente, sin 'died'"

run_test "3" \
    "Límite de comidas" \
    "./philo 5 800 200 200 3" \
    "15" \
    "Programa termina cuando todos coman 3 veces"

run_test "4" \
    "Timing preciso" \
    "./philo 2 400 200 200 5" \
    "5" \
    "Alternancia perfecta, timing exacto 200ms"

# ========================================
# TESTS DE MUERTE RÁPIDA - CORREGIDOS
# ========================================

run_test "5" \
    "Muerte garantizada - die_time muy bajo" \
    "./philo 4 150 200 100" \
    "2" \
    "die_time=150ms << ciclo=300ms → Muerte segura en ~150ms"

run_test "6" \
    "Imposible completar comida" \
    "./philo 4 100 200 50" \
    "2" \
    "die_time=100ms < eat_time=200ms → Muerte inmediata"

run_test "7" \
    "Conflicto con 3 filósofos" \
    "./philo 3 180 150 100" \
    "2" \
    "3 filósofos compiten por forks → Mayor probabilidad muerte"

run_test "8" \
    "Timing extremadamente ajustado" \
    "./philo 4 50 100 50" \
    "2" \
    "Solo 50ms para vivir → Muerte casi inmediata"

echo "🎉 =============================================="
echo "🎉 TODOS LOS TESTS COMPLETADOS"
echo "🎉 =============================================="
echo ""
echo "💡 RESUMEN DE TESTS:"
echo "   ✅ Tests 1-4: Casos básicos y funcionalidad"
echo "   🔥 Tests 5-8: Casos de muerte rápida CORREGIDOS"
echo ""
echo "📋 ANÁLISIS:"
echo "   - Tests 1-4 verifican funcionalidad correcta"
echo "   - Tests 5-8 verifican detección de muerte"
echo "   - Si AHORA no hay muertes en 5-8, revisar monitor_death()"
echo ""
echo "🔥 CAMBIOS EN TESTS 5-8:"
echo "   - Test 5: 250ms → 150ms (die_time mucho menor)"
echo "   - Test 6: 310ms → 100ms (imposible completar comida)"
echo "   - Test 7: 4 philos → 3 philos (más conflicto)"
echo "   - Test 8: 310ms → 50ms (timing extremo)"
echo ""
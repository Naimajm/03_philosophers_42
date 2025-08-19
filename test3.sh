

#!/bin/bash
# quick_test_complete.sh - Versión completa con tests de muerte rápida

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
# TESTS DE MUERTE RÁPIDA
# ========================================

run_test "5" \
    "Reducir die_time" \
    "./philo 4 250 200 100" \
    "2" \
    "die_time < eat_time + sleep_time → Muerte rápida"

run_test "6" \
    "Aumentar eat_time" \
    "./philo 4 310 250 100" \
    "2" \
    "eat_time + sleep_time > die_time → Muerte inevitable"

run_test "7" \
    "Imposible completar ciclo" \
    "./philo 4 200 250 100" \
    "2" \
    "die_time < eat_time → Muerte inmediata"

run_test "8" \
    "Timing muy ajustado" \
    "./philo 4 310 250 100" \
    "2" \
    "Margen mínimo → Alta probabilidad de muerte"

echo "🎉 =============================================="
echo "🎉 TODOS LOS TESTS COMPLETADOS"
echo "🎉 =============================================="
echo ""
echo "💡 RESUMEN DE TESTS:"
echo "   ✅ Tests 1-4: Casos básicos y funcionalidad"
echo "   ⚡ Tests 5-8: Casos de muerte rápida"
echo ""
echo "📋 ANÁLISIS:"
echo "   - Tests 1-4 verifican funcionalidad correcta"
echo "   - Tests 5-8 verifican detección de muerte"
echo "   - Si no hay muertes en 5-8, revisar monitor_death()"
echo ""


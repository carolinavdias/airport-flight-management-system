#!/bin/bash

echo "=========================================="
echo "  TESTE LOCAL - Simula AutoTester"
echo "=========================================="
echo ""

# TESTE 1: Datasets no repo?
echo "1. Verificar datasets no Git..."
DATASETS=$(git ls-files | grep -E "\.csv$|dataset/" | grep -v resultados)
if [ -z "$DATASETS" ]; then
    echo "✓ OK - Sem datasets"
else
    echo "✗ ERRO - Datasets encontrados:"
    echo "$DATASETS"
fi
echo ""

# TESTE 2: Compilação
echo "2. Compilar..."
make clean > /dev/null 2>&1
if make 2>&1 | grep -q "sucesso"; then
    echo "✓ OK - Compilou"
else
    echo "✗ ERRO - Falhou compilação"
    exit 1
fi
echo ""

# TESTE 3: Tempo de execução
echo "3. Testar execução (60s timeout)..."
rm -rf resultados/
START=$(date +%s)
timeout 60s ./programa-principal dataset/ inputs_fase1.txt > /dev/null 2>&1
EXIT_CODE=$?
END=$(date +%s)
TEMPO=$((END - START))

if [ $EXIT_CODE -eq 124 ]; then
    echo "✗ TIMEOUT! Mais de 60s"
elif [ $EXIT_CODE -ne 0 ]; then
    echo "✗ CRASH (código: $EXIT_CODE)"
else
    echo "✓ OK - Terminou em ${TEMPO}s"
fi
echo ""

# TESTE 4: Outputs criados?
echo "4. Verificar outputs..."
NUM=$(ls resultados/command*_output.txt 2>/dev/null | wc -l)
echo "✓ $NUM ficheiros criados"
echo ""

# TESTE 5: Ver exemplos
echo "5. Exemplos de outputs:"
echo ""
echo "Q1 (command1):"
head -2 resultados/command1_output.txt 2>/dev/null || echo "  (não encontrado)"
echo ""
echo "Q2 (command2):"
head -2 resultados/command2_output.txt 2>/dev/null || echo "  (não encontrado)"
echo ""
echo "Q3 (command3):"
head -2 resultados/command3_output.txt 2>/dev/null || echo "  (não encontrado)"
echo ""

# TESTE 6: Memory leaks (OPCIONAL - demora)
read -p "Testar memory leaks com valgrind? (demora 2min) [s/N]: " -n 1 -r
echo
if [[ $REPLY =~ ^[Ss]$ ]]; then
    echo "6. Valgrind..."
    LEAKS=$(valgrind --leak-check=summary ./programa-principal dataset/ inputs_fase1.txt 2>&1 | grep "definitely lost")
    echo "$LEAKS"
else
    echo "6. Valgrind pulado"
fi

echo ""
echo "=========================================="
echo "  FIM DOS TESTES"
echo "=========================================="
echo ""
echo "Se tudo passou → PODEM SUBMETER!"

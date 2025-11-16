#!/bin/bash

# ============================================
# SCRIPT DE MEDIÇÃO DE TEMPO POR QUERY
# ============================================

DATASET="dataset-fase-1"
INPUT="inputs_fase1.txt"
REPETITIONS=3  # Número de vezes que cada query é executada

echo "=========================================="
echo "  MEDIÇÃO DE TEMPO POR QUERY"
echo "=========================================="
echo ""
echo "Dataset: $DATASET"
echo "Input: $INPUT"
echo "Repetições por query: $REPETITIONS"
echo ""

# Verificar se ficheiros existem
if [ ! -f "$INPUT" ]; then
    echo "Erro: $INPUT não encontrado!"
    exit 1
fi

if [ ! -d "$DATASET" ]; then
    echo "Erro: $DATASET não encontrado!"
    exit 1
fi

# Arrays para guardar tempos
declare -a Q1_TIMES
declare -a Q2_TIMES
declare -a Q3_TIMES

echo "Medindo tempos... (isto pode demorar alguns minutos)"
echo ""

# Ler ficheiro linha a linha
COMMAND_NUM=1
while IFS= read -r line || [ -n "$line" ]; do
    # Extrair tipo de query (primeiro número da linha)
    QTYPE=$(echo "$line" | awk '{print $1}')
    
    # Criar input temporário com só esta query
    echo "$line" > /tmp/single_query.txt
    
    # Executar múltiplas vezes e tirar média
    TOTAL_TIME=0
    for ((i=1; i<=REPETITIONS; i++)); do
        rm -rf resultados/ 2>/dev/null
        mkdir -p resultados
        
        # Medir tempo em nanosegundos
        START=$(date +%s%N)
        ./programa-principal "$DATASET" /tmp/single_query.txt > /dev/null 2>&1
        END=$(date +%s%N)
        
        ELAPSED=$(( (END - START) / 1000000 ))  # Converter para ms
        TOTAL_TIME=$((TOTAL_TIME + ELAPSED))
    done
    
    # Calcular média
    AVG_TIME=$((TOTAL_TIME / REPETITIONS))
    
    # Guardar no array correspondente
    case $QTYPE in
        1) Q1_TIMES+=($AVG_TIME) ;;
        2) Q2_TIMES+=($AVG_TIME) ;;
        3) Q3_TIMES+=($AVG_TIME) ;;
    esac
    
    printf "Command %3d (Q%d): %6d ms (média de %d execuções)\n" \
           $COMMAND_NUM $QTYPE $AVG_TIME $REPETITIONS
    
    ((COMMAND_NUM++))
done < "$INPUT"

# Função para calcular estatísticas
calc_stats() {
    local -n arr=$1
    local sum=0
    local min=999999
    local max=0
    local count=${#arr[@]}
    
    if [ $count -eq 0 ]; then
        echo "Nenhuma query deste tipo"
        return
    fi
    
    for t in "${arr[@]}"; do
        sum=$((sum + t))
        [ $t -lt $min ] && min=$t
        [ $t -gt $max ] && max=$t
    done
    
    local avg=$((sum / count))
    
    echo "  Total de queries: $count"
    echo "  Tempo médio: $avg ms"
    echo "  Tempo mínimo: $min ms"
    echo "  Tempo máximo: $max ms"
    echo "  Tempo total: $sum ms"
}

echo ""
echo "=========================================="
echo "  ESTATÍSTICAS POR TIPO DE QUERY"
echo "=========================================="
echo ""

if [ ${#Q1_TIMES[@]} -gt 0 ]; then
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "Q1 - Busca de Aeroporto"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    calc_stats Q1_TIMES
    echo ""
fi

if [ ${#Q2_TIMES[@]} -gt 0 ]; then
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "Q2 - Top N Aeronaves"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    calc_stats Q2_TIMES
    echo ""
fi

if [ ${#Q3_TIMES[@]} -gt 0 ]; then
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "Q3 - Aeroporto com Mais Partidas"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    calc_stats Q3_TIMES
    echo ""
fi

# Resumo geral
TOTAL_Q1=0
TOTAL_Q2=0
TOTAL_Q3=0

for t in "${Q1_TIMES[@]}"; do TOTAL_Q1=$((TOTAL_Q1 + t)); done
for t in "${Q2_TIMES[@]}"; do TOTAL_Q2=$((TOTAL_Q2 + t)); done
for t in "${Q3_TIMES[@]}"; do TOTAL_Q3=$((TOTAL_Q3 + t)); done

GRAND_TOTAL=$((TOTAL_Q1 + TOTAL_Q2 + TOTAL_Q3))

echo "=========================================="
echo "  RESUMO GERAL"
echo "=========================================="
echo "Tempo total Q1: $(echo "scale=3; $TOTAL_Q1/1000" | bc) s"
echo "Tempo total Q2: $(echo "scale=3; $TOTAL_Q2/1000" | bc) s"
echo "Tempo total Q3: $(echo "scale=3; $TOTAL_Q3/1000" | bc) s"
echo "TEMPO TOTAL: $(echo "scale=3; $GRAND_TOTAL/1000" | bc) s"
echo "=========================================="

# Limpar
rm -f /tmp/single_query.txt

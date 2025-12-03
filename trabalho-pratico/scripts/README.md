# Scripts Auxiliares - Análise de Performance

Este diretório contém scripts desenvolvidos para análise de desempenho do projeto.

## medir_tempos_completo.sh

Mede o tempo de execução individual de cada query do ficheiro de input.

**Uso:**
```bash
chmod +x medir_tempos_completo.sh
./medir_tempos_completo.sh
```

**Funcionalidades:**
- Executa cada query 3 vezes e calcula a média
- Apresenta estatísticas por tipo de query (Q1, Q2, Q3)
- Tempo mínimo, máximo e médio
- Precisão de milissegundos

**Tempo estimado:** ~10-15 minutos

## info_completa.sh

Recolhe informação completa do sistema e testa desempenho.

**Uso:**
```bash
chmod +x info_completa.sh
./info_completa.sh
```

**Output:** Cria ficheiro `relatorio_performance.txt` com:
- Especificações de hardware (CPU, RAM, SO)
- Testes de desempenho (5 execuções)
- Análise de memória
- Verificação de memory leaks (valgrind)

**Tempo estimado:** ~5 minutos

## Nota

Estes scripts foram utilizados para recolher dados experimentais 
incluídos na Secção 4 (Análise de Desempenho) do relatório.
EOF

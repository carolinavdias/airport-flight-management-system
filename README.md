<div align="center">

<h1>✈️ 𝘼𝙞𝙧𝙥𝙤𝙧𝙩 & 𝙁𝙡𝙞𝙜𝙝𝙩 𝙈𝙖𝙣𝙖𝙜𝙚𝙢𝙚𝙣𝙩 𝙎𝙮𝙨𝙩𝙚𝙢</h1>
<h3>Data Analysis Engine for Flight Operations</h3>

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![GLib](https://img.shields.io/badge/GLib-4A90D9?style=for-the-badge)
![Valgrind](https://img.shields.io/badge/Valgrind-grey?style=for-the-badge)

*Academic project developed for **Laboratórios de Informática III** @ University of Minho*  
*Final grade: **17/20** ⭐*

</div>

---

## 🛫 About

A data management system built in **C** for analysing real-world flight operations data. The system loads and validates large CSV datasets containing airports, flights, aircraft, passengers and reservations, storing them in optimised in-memory data structures to answer a set of analytical queries efficiently.

The project was developed across two phases, with a strong focus on **modular architecture**, **encapsulation via opaque pointers**, **memory safety** and **performance optimisation** using advanced data structures such as hash tables.

---

## ⚡ Queries

| Query | Description |
|---|---|
| `Q1 <code>` | Airport summary - name, city, country, type, arrival and departure counts |
| `Q2 <N> [manufacturer]` | Top N aircraft with most flights, optionally filtered by manufacturer |
| `Q3 <start> <end>` | Airport with most departures in a date range |
| `Q4 [start end]` | Passenger who spent most time in the weekly top 10 spenders |
| `Q5 <N>` | Top N airlines by average delay per flight |
| `Q6 <nationality>` | Most common destination airport for passengers of a given nationality |

---

## 🚀 Features

- **CSV parsing & validation** - syntactic and logical validation of all input datasets with error reporting
- **Modular architecture** - strict separation between entities, managers, parsers, queries and I/O
- **Encapsulation** - opaque pointers throughout to enforce module isolation
- **Performance** - hash tables and optimised data structures for fast query execution
- **Three execution modes** - batch, interactive and testing
- **Test program** - validates query correctness, measures execution time and reports memory usage
- **Doxygen documentation** - all public interfaces documented
- **Memory safety** - validated with Valgrind, zero memory leaks

---

## 🚀 How to Run

```bash
# Compile
make

# Batch mode — process a list of queries from a file
./programa-principal <dataset_folder/> <input.txt>

# Interactive mode — query the system via terminal menu
./programa-interativo

# Test mode — validate results and measure performance
./programa-testes <dataset_folder/> <input.txt> <expected_results_folder/>
```

---

## 🗂️ Project Structure

```
trabalho-pratico/
├── include/          # Header files with Doxygen documentation
├── src/              # Implementation files
├── resultados/       # Query outputs and error files (generated at runtime)
├── relatorio-fase1.pdf
├── relatorio-fase2.pdf
└── Makefile
```

---

## 🛠️ Tech Stack

`C` · `Hash Tables` · `Opaque Pointers` · `Modular Architecture` · `Valgrind` · `GDB` · `Doxygen` · `GLib2` · `ncurses`

---

## 👩‍💻 Authors

**Carolina Dias** - [@carolinavdias](https://github.com/carolinavdias)  
**Ana Patrícia Machado** - [@a111661](https://github.com/a111661)  
**Alexandra Neves** - [@xana06](https://github.com/xana06)

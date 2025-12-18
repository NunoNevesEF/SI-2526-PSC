----------		Commands		----------	

wsl  
sudo apt update
sudo apt install libcsv-dev
sudo apt install valgrind
sudo apt install build-essential	// Make & gcc

gcc -g ex1.c -o ex1
make
./ex1_filter caminho-anterior/SI-2526-PSC/files/frutas.csv filter_table_testing
./ex1_loadAndSave caminho-anterior/SI-2526-PSC/files/frutas.csv load_and_save_testing


----------		TodoList		----------

Implement correctly cleanTable in comandos.c in ex3 and ex4
Implement ex4 with a shared object

Code Review :

Check table_load_csv and table_save_csv, specially the libcsv operations
Check ways to optimize and simplify code, specially in table.c
Check makefile

----------		DueDate 		----------

Data limite de entrega: 22 de dezembro de 2025

----------      Links     ----------

https://psc-apontamentos.readthedocs.io/pt/latest/values_expressions/values.html
https://github.com/NunoNevesEF/Project-C-Assembly
https://www.dropbox.com/scl/fo/4uk88ebbbrujaacdg7xuf/ABDH8n02Jcbc4IQWMABFZgU?rlkey=zzvcpqdie5g2a62vpv8lw0ep7&e=1&st=op3e3pe2&dl=0

----------		Developers 		----------

- 
-
- Nuno Neves nº48308

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
CODE_REVIEW UNTIL 22/12 :

- Fix the main.c [execComand] function in order to search in the commandArray, remove the if's
- Fix commandos.c [cleanTable] function in order to sucessfully clean memory allocated 
- Check table_save_csv, currente implementation is using fputc (using the csv lib with write_csv would be BiS)
- Check ways to optimize and simplify code, specially in table.c due to libcsv functions
- Check if it is required to pass parameters into the libfile.so ( would be easy to achieve this if the user pass the row they wanna delete in the command )

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

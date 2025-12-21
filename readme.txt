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

 gcc -shared -fPIC -o libfile.so libfile.c

load frutas.csv
show A1:E6

command libfile.so handle_remove_first
command libfile.so test




----------		TodoList		----------

CODE_REVIEW UNTIL 22/12 :

- [High] Fix table_save_csv, current implementation is using fputc (using the csv lib with write_csv would be BiS)
- [Low] Add protection to table.c show_sub_table, (iterate over the current stored table to check if the user input is valid, before iterating over the table)

int initialCol = initialColumn - 'A';
    int lastCol = lastColumn - 'A';
    int iRow = initialRow - '1';
    int lRow = lastRow - '1';

    // Add protection here

    for (int c = initialCol; c <= lastCol; c++)
    {
        printf("%s\t", current_table->columns[c]);
    }
    printf("\n");

- [Low] Try to pass parameters into the libfile.so ( would be easy to achieve this if the user pass the row they wanna delete in the command ). This is an extra, from the given functionalities this should not be required.

----------		DueDate 		----------

Data limite de entrega: 22 de dezembro de 2025 23:59h

----------      Links     ----------

https://psc-apontamentos.readthedocs.io/pt/latest/values_expressions/values.html
https://github.com/NunoNevesEF/Project-C-Assembly
https://www.dropbox.com/scl/fo/4uk88ebbbrujaacdg7xuf/ABDH8n02Jcbc4IQWMABFZgU?rlkey=zzvcpqdie5g2a62vpv8lw0ep7&e=1&st=op3e3pe2&dl=0

----------		Developers 		----------

- 
- André Esteves nº48321
- Nuno Neves nº48308

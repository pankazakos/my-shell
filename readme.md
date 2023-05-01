# Mysh: My simple custom shell

## Compile and run
To compile use
```
make
```
And run the shell with
```
./mysh
```

## Description
Mysh is a simple shell for Unix operating systems that allows the user to run any executable located in current directory or in PATH folders. It also provides: 
- i/o redirection of stdout and stdin to files
- signal handling for Ctrl+C and Ctrl+Z to stop or suspend a process
- pipes
- aliases
- history
- background processes.

## Parser
The syntax of mysh is similar to BASH with very little differences. Each time user gives an input the parser creates an array of tokens which include the tokens for all commands to be executed. Commands are separated by ';' or '|'. The expected form of each command is: \
`exec args < fileInput args > fileOutput args`
- Arguments can be found before, after or among files to which the redirection is done.
- Whitespace does not matter in mysh.

## Aliases
Create a new alias with `createalias` keyword. If your alias consists of multiple values separated by space you do not need to add `'"'` characters. To delete an alias use `destroyalias`.

## History
History is temporarily stored (in each session) and can only be viewed. You can view the last 20 lines of input to mysh by entering `h` or view a specific line of history with `h-i`.

## Tests
```
in-mysh-now:> echo -e "5 \n4 \n1 \n3 \n2" > num.txt
in-mysh-now:> sort<num.txt>sorted.txt; ls|grep sorted.txt ;cat sorted.txt
sorted.txt
1 
2
3 
4 
5 
in-mysh-now:> echo 6 new        >>sorted.txt         ; cat<sorted.txt | grep new
6 new
in-mysh-now:> createalias ll "ls -la -g"
in-mysh-now:> ll
.
.
.
in-mysh-now:> destroyalias ll
in-mysh-now:> ll
ll is not a command
in-mysh-now:> sleep 30 &        ; ls    | grep Makefile
Makefile
in-mysh-now:> echo first line > new.txt ; ls | grep Makefile | cat >> new.txt ; cat new.txt ; myHistory 2
sort<num.txt>sorted.txt; ls|grep sorted.txt ;cat sorted.txt
first line
Makefile
sorted.txt
1 
2
3 
4 
5 


// other tests
wget https://ftp.ebi.ac.uk/pub/databases/genenames/hgnc/tsv/locus_types/gene_with_protein_product.txt
cat gene_with_protein_product.txt
cut -f2 gene_with_protein_product.txt > gene_with_protein_product_names_only_v1.txt
cat gene_with_protein_product_names_only_v1.txt
cat gene_with_protein_product.txt | cut -f2 > gene_with_protein_product_names_only_v2.txt
diff gene_with_protein_product_names_only_v1.txt gene_with_protein_product_names_only_v2.txt
(return nothing)

shuf gene_with_protein_product_names_only_v1.txt > gene_with_protein_product_names_only_v1_shuffled.txt
sort < gene_with_protein_product_names_only_v1_shuffled.txt > gene_with_protein_product_names_only_v1_sorted.txt
head -100 gene_with_protein_product_names_only_v1.txt >h100.txt
tail -50 gene_with_protein_product_names_only_v1.txt >t50.txt
cat h100.txt t50.txt >ht150_v1.txt
wc -l ht150_v1.txt
(return 150)

cat h100.txt> ht150_v2.txt
cat t50.txt >> ht150_v2.txt
diff ht150_v1.txt ht150_v2.txt
(return nothing)

ls gene*
ls h*1?0.txt
```
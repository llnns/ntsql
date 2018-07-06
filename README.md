# ntSQL: RDF Document converter to SQL

**ntSQL** is a simple tool to automatically infer and extract from an RDF document file in nt format a SQL script to build an equivalent relational database.

### Authors:

* Flávio R. Bayer
* Lucas L. Nesi
* Rebeca Schroeder

### Requirements
* Bison
* Flex

### Build

Using makefile:

```sh
make
```

### Execution

The program has only one argument, the nt file location. Example:

```sh
./ntsql file.nt
```

### Generated files

* output.sql : SQL script to create the relational database.
* relational_model.txt : The inferred relational model.

### Descriptive Paper:

In Portuguese: http://www.lbd.dcc.ufmg.br/colecoes/erbd/2015/011.pdf

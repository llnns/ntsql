#ifndef aux_h
#define aux_h

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <cctype>
#include <set>
#include <map>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <sstream>

#define DB_TYPE "http://www.w3.org/1999/02/22-rdf-syntax-ns#type"
#define N_N_CHAR "X"
#define TIPO_INT 1
#define TIPO_DOUBLE 2
#define TIPO_VARCHAR 3

using namespace std;

typedef struct{
	int cardinalidade;
	int db_maior_tam;
	int db_tipo;
	string nome;
	string tabela_nova;
	string FK;
}Campos;


typedef struct{
	string nome;
	int auto_increment;
	map<string,Campos> variaveis;
}Tabelas;


typedef struct{
	string nome;
	string valor;
	int IRI;
	string tipo;
}Variaveis;

typedef struct{
	int id;
	string nome;
	map<string,Variaveis> variaveis;
	queue<Variaveis> aprocessar;
	string tipo;
}Sujeito;

extern map<string,Tabelas> TodasTabelas;
extern map<string,Sujeito> TodosSujeitos;

struct line{
	string sujeito;
	string predicado;
	string objeto;
	string tipo;
	string tmp;
};

int yyerror (const char *str);
extern int yylex();
extern int yylineno;

void inserir(string sujeito,string predicado,string objeto,string tipo);
string Ultimo_nome(string a,string final);

#endif

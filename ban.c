#include <cstdio>
#include <cstring>
#include <cctype>
#include <iostream>
#include <set>
#include <map>
#include <stdlib.h>
#include <string.h>
#include <queue>

#include "lib.h"

extern FILE *yyin;
int yyparse (void);

using namespace std;

map<string,Tabelas> TodasTabelas;
map<string,Sujeito> TodosSujeitos;

template <typename T>
string NumberToString(T Number) {
    ostringstream ss;
    ss << Number;
    return ss.str();
}

int main(int argc, char **argv)
{

	if(argc<2){
			printf("Usage: ntsql <RDF File>\n");
      exit(1);
	}

	yyin = fopen(argv[1],"r");

	if(yyin==NULL){
			printf("File not found.\n");
			exit(1);
	}

	string tmp;

	printf("Reading File: %s\n",argv[1]);
	yyparse();

	printf("Inferring Tables\n");
	for (map<string,Sujeito>::iterator it = TodosSujeitos.begin(); it != TodosSujeitos.end(); it++) {
		if(it->second.id>0){
			while (!it->second.aprocessar.empty()){
				Variaveis *yt = &(it->second.aprocessar.front());
				if(TodosSujeitos.find(yt->valor)!=TodosSujeitos.end()){
					TodasTabelas[it->second.tipo].variaveis[yt->nome].FK = TodosSujeitos[yt->valor].tipo;
				}
				if(TodasTabelas[it->second.tipo].variaveis[yt->nome].cardinalidade>1){
						string nova_tabela = Ultimo_nome(it->second.tipo,tmp) + N_N_CHAR + Ultimo_nome(yt->nome,tmp);
						string nova_tabela2 = nova_tabela;
						nova_tabela2 += NumberToString(it->second.id);
						nova_tabela2 += N_N_CHAR;
						nova_tabela2 += yt->valor;

						if(TodasTabelas[it->second.tipo].variaveis[yt->nome].tabela_nova==""){
							TodasTabelas[nova_tabela].nome = nova_tabela;
							TodasTabelas[nova_tabela].auto_increment=0;
							TodasTabelas[nova_tabela].variaveis[it->second.tipo].nome = it->second.tipo;
							TodasTabelas[nova_tabela].variaveis[it->second.tipo].cardinalidade = 1;
							TodasTabelas[nova_tabela].variaveis[it->second.tipo].db_tipo = 1;
							TodasTabelas[nova_tabela].variaveis[it->second.tipo].FK = it->second.tipo;

							TodasTabelas[nova_tabela].variaveis[yt->nome].nome = TodasTabelas[it->second.tipo].variaveis[yt->nome].nome;
							TodasTabelas[nova_tabela].variaveis[yt->nome].cardinalidade = 1;
							TodasTabelas[nova_tabela].variaveis[yt->nome].db_tipo = TodasTabelas[it->second.tipo].variaveis[yt->nome].db_tipo;
							TodasTabelas[nova_tabela].variaveis[yt->nome].db_maior_tam = TodasTabelas[it->second.tipo].variaveis[yt->nome].db_maior_tam;
							TodasTabelas[nova_tabela].variaveis[yt->nome].FK = TodasTabelas[it->second.tipo].variaveis[yt->nome].FK;

							TodasTabelas[it->second.tipo].variaveis[yt->nome].tabela_nova = nova_tabela;
						}
						TodosSujeitos[nova_tabela2].id = -1;
						TodosSujeitos[nova_tabela2].tipo = nova_tabela;
						TodosSujeitos[nova_tabela2].nome = nova_tabela;
						TodosSujeitos[nova_tabela2].variaveis[it->second.tipo].nome = it->second.tipo;
						TodosSujeitos[nova_tabela2].variaveis[it->second.tipo].valor = it->second.nome;
						TodosSujeitos[nova_tabela2].variaveis[it->second.tipo].tipo = it->second.tipo;

						TodosSujeitos[nova_tabela2].variaveis[yt->nome].nome = yt->nome;
						TodosSujeitos[nova_tabela2].variaveis[yt->nome].valor = yt->valor;
						TodosSujeitos[nova_tabela2].variaveis[yt->nome].tipo = yt->tipo;
				}else{
					it->second.variaveis[yt->nome] = *yt;
				}
				it->second.aprocessar.pop();
			}

		}
	}

	printf("Generating SQL Creat Tables calls and relational model file\n");
	FILE *fc = fopen("output.sql", "w");
	FILE *ff = fopen("relational_model.txt", "w");
	for (map<string,Tabelas>::iterator it = TodasTabelas.begin(); it != TodasTabelas.end(); it++) {
		fprintf(fc, "CREATE TABLE `%s` (", Ultimo_nome(it->first,tmp).c_str());
		fprintf(ff, "%s (", Ultimo_nome(it->first,tmp).c_str());
		if(it->second.auto_increment>0){
			fprintf(fc, "`id` int(11) NOT NULL,PRIMARY KEY (`id`),");
			fprintf(ff, "#id,");
		}
		for (map<string,Campos>::iterator yt = it->second.variaveis.begin(); yt != it->second.variaveis.end(); yt++) {
			if(yt->second.cardinalidade==1){
				if (yt->second.FK != "") {
					fprintf(fc, "`%s` int DEFAULT NULL,", Ultimo_nome(yt->first,tmp).c_str());
					fprintf(ff, "&%s:%s,", Ultimo_nome(yt->first,tmp).c_str(),Ultimo_nome(yt->second.FK,tmp).c_str());
				} else {
					fprintf(ff, "%s,", Ultimo_nome(yt->first,tmp).c_str());
					if(yt->second.db_tipo==TIPO_INT){
						fprintf(fc, "`%s` int DEFAULT NULL,", Ultimo_nome(yt->first,tmp).c_str());
					}else if(yt->second.db_tipo==TIPO_DOUBLE){
						fprintf(fc, "`%s` double DEFAULT NULL,", Ultimo_nome(yt->first,tmp).c_str());
					}else{
						fprintf(fc, "`%s` varchar(%d) DEFAULT NULL,", Ultimo_nome(yt->first,tmp).c_str(),yt->second.db_maior_tam+10);
					}
				}
			}
		}
		fseek(fc, -1, SEEK_CUR);
		fseek(ff, -1, SEEK_CUR);
        fprintf(fc, ");\n");
		fprintf(ff, ")\n");
	}
	fclose(ff);
	printf("Generating SQL Insert calls\n");
	for (map<string,Sujeito>::iterator it = TodosSujeitos.begin(); it != TodosSujeitos.end(); it++) {
		fprintf(fc,"INSERT INTO %s (", Ultimo_nome(it->second.tipo,tmp).c_str());
		if(it->second.id>0){
			fprintf(fc,"id,");
		}
		for (map<string,Variaveis>::iterator yt = it->second.variaveis.begin(); yt != it->second.variaveis.end(); yt++) {
			fprintf(fc,"%s,", Ultimo_nome(yt->second.nome,tmp).c_str());
		}
		fseek(fc,-1,SEEK_CUR);
		fprintf(fc,") VALUES (");
		if(it->second.id>0){
			fprintf(fc,"%d,",it->second.id);
		}
		for (map<string,Variaveis>::iterator gt = it->second.variaveis.begin(); gt != it->second.variaveis.end(); gt++) {
			if(TodasTabelas[it->second.tipo].variaveis[gt->second.nome].FK==""){
				fprintf(fc,"'%s',",Ultimo_nome(gt->second.valor,tmp).c_str());
			}else{
				fprintf(fc,"%d,",TodosSujeitos[gt->second.valor].id);
			}
		}
		fseek(fc,-1,SEEK_CUR);
		fprintf(fc,");\n");



	}
	printf("Generating SQL Key calls\n");
	int nextCONSTRAINT = 0;
    for (map<string,Tabelas>::iterator it = TodasTabelas.begin(); it != TodasTabelas.end(); it++) {
        nextCONSTRAINT = 0;
        for (map<string,Campos>::iterator yt = it->second.variaveis.begin(); yt != it->second.variaveis.end(); yt++) {
			if(yt->second.cardinalidade==1){
				if (yt->second.FK != "") {
					if (nextCONSTRAINT == 0) {
						fprintf(fc, "ALTER TABLE `%s`", Ultimo_nome(it->first,tmp).c_str());
					}
					fprintf(fc, "ADD CONSTRAINT `%s_ibfk_%d` FOREIGN KEY (`%s`) REFERENCES `%s` (`id`),", Ultimo_nome(it->first,tmp).c_str(), nextCONSTRAINT,Ultimo_nome(yt->first,tmp).c_str(), Ultimo_nome(yt->second.FK,tmp).c_str());
					nextCONSTRAINT++;
				}
            }
        }
        if (nextCONSTRAINT) {
			fseek(fc,-1,SEEK_CUR);
            fprintf(fc, ";\n");
        }
    }
    fclose(fc);
	printf("Done! SQL File: output.sql\n");

	return 0;
}

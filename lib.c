#include "lib.h"

bool operator<(const Sujeito& a, const Sujeito& b) {
    return a.nome < b.nome;
}

bool operator<(const Variaveis& a, const Variaveis& b) {
    return a.nome < b.nome;
}

bool operator<(const Tabelas& a, const Tabelas& b) {
    return a.nome < b.nome;
}

bool operator<(const Campos& a, const Campos& b) {
    return a.nome < b.nome;
}

bool is_int(string a){
		return a.find_first_not_of("0123456789") == std::string::npos;
}

bool is_double(string a){
	if(a.find_first_not_of("0123456789.") == std::string::npos){
		return a.find_first_of(".")==a.find_last_of(".");
	}
	return false;
}

string Ultimo_nome(string a,string final){
		final = a;
		size_t para = final.find_last_of("/");
        if (para == string::npos) {
            para = -1;
        }
        size_t ini = final.find_last_of("#");
        if (ini == string::npos || ini<para) {
            ini = para;
        }

        final = final.substr(ini + 1, final.size() - (ini-1));
		return final;
}

void inserir(string sujeito,string predicado,string objeto,string tipo){
	static string ultimo_sujeito;
	static map<string,Campos> ultimo_valores;

	if(ultimo_sujeito!=sujeito){
		if(ultimo_sujeito!=""){

			for (map<string,Campos>::iterator it = ultimo_valores.begin(); it != ultimo_valores.end(); it++) {
				if(TodasTabelas[TodosSujeitos[ultimo_sujeito].tipo].variaveis[it->first].cardinalidade<ultimo_valores[it->first].cardinalidade){
					TodasTabelas[TodosSujeitos[ultimo_sujeito].tipo].variaveis[it->first].cardinalidade=ultimo_valores[it->first].cardinalidade;
				}
				if(TodasTabelas[TodosSujeitos[ultimo_sujeito].tipo].variaveis[it->first].db_maior_tam<ultimo_valores[it->first].db_maior_tam){
					TodasTabelas[TodosSujeitos[ultimo_sujeito].tipo].variaveis[it->first].db_maior_tam=ultimo_valores[it->first].db_maior_tam;
				}
				if(TodasTabelas[TodosSujeitos[ultimo_sujeito].tipo].variaveis[it->first].db_tipo<ultimo_valores[it->first].db_tipo){
					TodasTabelas[TodosSujeitos[ultimo_sujeito].tipo].variaveis[it->first].db_tipo=ultimo_valores[it->first].db_tipo;
				}
			}
			TodosSujeitos[ultimo_sujeito].id = ++TodasTabelas[TodosSujeitos[ultimo_sujeito].tipo].auto_increment;
		}
		ultimo_sujeito=sujeito;
		ultimo_valores.clear();
	}

	if(predicado==string(DB_TYPE) && TodosSujeitos[sujeito].tipo.empty()){
		TodasTabelas[objeto].nome = objeto;
		TodosSujeitos[sujeito].nome=sujeito;
		TodosSujeitos[sujeito].tipo=objeto;

	}else if(sujeito==""){

	}else{
		Variaveis a;
		a.nome = predicado;
		a.valor = objeto;
		a.tipo = tipo;
		TodosSujeitos[sujeito].aprocessar.push(a);
		ultimo_valores[predicado].cardinalidade++;
		if(ultimo_valores[predicado].db_maior_tam<objeto.size()){
			ultimo_valores[predicado].db_maior_tam=objeto.size();
		}

		if(is_int(objeto)){
			if(ultimo_valores[predicado].db_tipo<TIPO_INT)ultimo_valores[predicado].db_tipo=TIPO_INT;
		}else{
			if(is_double(objeto)){
				if(ultimo_valores[predicado].db_tipo<TIPO_DOUBLE)ultimo_valores[predicado].db_tipo=TIPO_DOUBLE;
			}else{
				ultimo_valores[predicado].db_tipo=TIPO_VARCHAR;
			}
		}
	}
}

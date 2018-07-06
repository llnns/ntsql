%{

#include "lib.h"

#define YYSTYPE struct line

%}

%token TLANGTAG TEOL TIRIREF TCOMENTARIO THEX TUCHAR TBLANK_NODE_LABEL TECHAR ERRO TPONTO  TSOBRECARGA TSTRING_LITERAL_QUOTE

%%

ntriplesDoc : triplas {inserir("","","","");}
			;

triplas : triplas pretriple
		| pretriple
		;

pretriple : triple
		  | TCOMENTARIO
		  | TEOL
		  ;

triple : subject predicate object TPONTO {inserir($1.sujeito,$2.predicado,$3.objeto,$3.tipo);}
	   ;

subject : iriref {$$.sujeito=$1.tmp;}
		| TBLANK_NODE_LABEL {$$.sujeito=$1.tmp;}
		;

predicate : iriref {$$.predicado=$1.tmp;}
		;

object 	: iriref {$$.objeto=$1.tmp;}
		| TBLANK_NODE_LABEL {$$.objeto=$1.tmp;}
		| literal {$$.objeto=$1.objeto.substr(1,$1.objeto.size()-2);$$.tipo=$1.tipo;}
		;

literal : TSTRING_LITERAL_QUOTE {$$.objeto=$1.tmp;}
		| TSTRING_LITERAL_QUOTE TSOBRECARGA iriref {$$.objeto=$1.tmp;$$.tipo=$3.tmp;}
		| TSTRING_LITERAL_QUOTE TLANGTAG {$$.objeto=$1.tmp;}
		;

iriref : TIRIREF {$$.tmp=$1.tmp.substr(1,$1.tmp.size()-2);}
		;


%%

#include "lex.yy.c"

int yyerror (const char *str)
{
	printf("\nSyntatic Error!\n");
	printf("Error: %s\n", yytext);
	printf("Line:%d\n",yylineno);
	printf("Trying to continue...\n");
	return 0;

}

int yywrap()
{
	return 1;
}

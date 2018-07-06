%option yylineno
%option nounput
%option noinput


LANGTAG @[a-zA-Z]+(-[a-zA-Z0-9]+)*
EOL [\n\r]+
IRIREF \<[^\x00-\x20\<\>\"{}|^`\\]*\>
BLANK_NODE_LABEL _:[a-zA-Z0-9]*
ECHAR \\[tbnrf\"\'\\]
COMENTARIO #([^\n\r])*
STRING_LITERAL_QUOTE \"([^\x22\x5C\xA\xD]|{ECHAR})*\"
delim [ \t]
ws	{delim}+



%%
{ws} {}
{LANGTAG} {return TLANGTAG;}
{EOL} {return TEOL;}
{IRIREF} {yylval.tmp=yytext; return TIRIREF;}
{BLANK_NODE_LABEL} {yylval.tmp=yytext; return TBLANK_NODE_LABEL;}
{ECHAR} {return TECHAR;}
{COMENTARIO} {return TCOMENTARIO;}
{STRING_LITERAL_QUOTE} {yylval.tmp=yytext; return TSTRING_LITERAL_QUOTE;}
"." {return TPONTO;}
"^^" {return TSOBRECARGA;}

. {return ERRO;}

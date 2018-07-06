all: ntsql

ntsql : lex.yy.c ban.tab.c ban.c lib.c
	g++ ban.c ban.tab.c lib.c -o ntsql

lex.yy.c :
	flex ban.l

ban.tab.c :
	bison ban.y

clean:
	rm ntsql lex.yy.c ban.tab.c

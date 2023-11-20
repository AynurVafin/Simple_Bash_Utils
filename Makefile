Cat=src/cat/cat.c
Cat_o=src/cat/s_cat 
Flags=-Wall -Werror -Wextra
Grep=src/grep/grep.c
Grep_o=src/grep/s_grep

all: s_cat s_grep
	
s_grep:
	gcc $(Flags) $(Grep) -o $(Grep_o)

s_cat: 
	gcc $(Flags) $(Cat) -o $(Cat_o)

rebuild: clean all

clean:
	rm -rf $(Cat_o)
	rm -rf $(Grep_o)


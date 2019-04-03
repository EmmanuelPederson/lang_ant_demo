
lang_ant_runner: main.o lang_ant.o pos_hash.o dict_lang_rule.o
	gcc -O3 -o bin/lang_ant_runner build/main.o build/lang_ant.o build/pos_hash.o build/dict_lang_rule.o -lm

main.o: src/main.c
	gcc -c -o build/main.o src/main.c

lang_ant.o: src/lang_ant.c
	gcc -c -o build/lang_ant.o src/lang_ant.c

dict_lang_rule.o: src/dict_lang_rule.c
	gcc -c -o build/dict_lang_rule.o src/dict_lang_rule.c

test_runner: pos_hash.o 
	gcc -o bin/test_runner -Isrc tests/tests.c build/pos_hash.o

pos_hash.o: src/pos_hash.c
	gcc -c -o build/pos_hash.o src/pos_hash.c

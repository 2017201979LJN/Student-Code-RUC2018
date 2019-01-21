with-structure: avl-tree.o avl-tree-internel.o bitset.o bitstream.o serialize.o deserialize.o int256.o huffman.o with-structure.o
	gcc -O2 -Wall avl-tree.o avl-tree-internel.o bitset.o bitstream.o serialize.o deserialize.o int256.o huffman.o with-structure.o -o with-structure
insert-sequence: bitset.o bitstream.o huffman.o int256.o insert-sequence.o
	gcc -O2 -Wall bitset.o bitstream.o huffman.o int256.o insert-sequence.o -o insert-sequence
debug: avl-tree.o avl-tree-internel.o bitset.o bitstream.o serialize.o deserialize.o int256.o huffman.o debug.o
	gcc -O2 -Wall avl-tree.o avl-tree-internel.o bitset.o bitstream.o serialize.o deserialize.o int256.o huffman.o debug.o -o debug
avl-tree.o: avl-tree/avl-tree.c
	gcc -O2 -Wall -c avl-tree/avl-tree.c -o avl-tree.o
avl-tree-internel.o: avl-tree/avl-tree-internel.c
	gcc -O2 -Wall -c avl-tree/avl-tree-internel.c -o avl-tree-internel.o
bitset.o: bitset/bitset.c
	gcc -O2 -Wall -c bitset/bitset.c -o bitset.o
bitstream.o: bitset/bitstream.c
	gcc -O2 -Wall -c bitset/bitstream.c -o bitstream.o
serialize.o: serialization/serialize.c
	gcc -O2 -Wall -c serialization/serialize.c -o serialize.o
deserialize.o: serialization/deserialize.c
	gcc -O2 -Wall -c serialization/deserialize.c -o deserialize.o
lz77.o: lz77/lz77.c
	gcc -O2 -Wall -c lz77/lz77.c -o lz77.o
int256.o: int256/int256.c
	gcc -O2 -Wall -c int256/int256.c -o int256.o
huffman.o: huffman/huffman.c
	gcc -O2 -Wall -c huffman/huffman.c -o huffman.o
with-structure.o: with-structure.c
	gcc -O2 -Wall -c with-structure.c -o with-structure.o
insert-sequence.o: insert-sequence.c
	gcc -O2 -Wall -c insert-sequence.c -o insert-sequence.o
debug.o: debug.c
	gcc -O2 -Wall -c debug.c -o debug.o
insert-sequence-data-generator: insert-sequence-data-generator.c
	gcc -O2 -Wall insert-sequence-data-generator.c -o insert-sequence-data-generator
clean:
	rm -f *.o debug insert-sequence insert-sequence-data-generator with-structure

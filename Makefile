CC := gcc
FLAGS := -Wall -Wextra
TEST_OUT := geohash_test
RM := rm -f

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

geohash.o:

test:	test-geohash.c geohash.o
	$(CC) $(FLAGS) test-geohash.c geohash.o -o $(TEST_OUT)
	./$(TEST_OUT)

clean:
	$(RM) *.o $(TEST_OUT)

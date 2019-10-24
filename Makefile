CC = gcc
FLAGS = -std=c99 -O0 -Wall -Werror -g -pedantic 

rgrep:
	$(CC) $(FLAGS) rgrep.c -o rgrep

clean:
	rm -rf rgrep *.dSYM

check: clean rgrep
	test "`echo "mcdonalds" | ./rgrep 'l'`" = "mcdonalds"
	test "`echo "cse31isaaaawsome" | ./rgrep 'a+'`" = "cse31isaaaawsome"
	test "`echo "i\nhope\nipass\nthisproject" | ./rgrep 'hope'`" = "hope"
	test "`echo "ham\nburger" | ./rgrep '.u.'`" = "burger"
	test "`echo "grrrrrrrrrr" | ./rgrep 'grrrrrrrrrr'`" = "grrrrrrrrrr"
	test "`echo "apples\ngray" | ./rgrep 'p+'`" = "apples"
	test "`echo "A+project" | ./rgrep '.\+'`" = "A+project"
	test "`echo "a\nb\nc\nd" | ./rgrep 'c?'`" = "c"
	test "`echo "microphone\nmirophone" | ./rgrep 'micro'`" = "microphone"
	test "`echo "yummy" | ./rgrep 'm+y'`" = "yummy"
	test "`echo "abcdefghijklmnop" | ./rgrep '......'`" = "abcdefghijklmnop"
	test "`echo "C++\nor\nC" | ./rgrep 'o?r'`" = "or"
	test "`echo "CCCCCCC\nC+\nC++" | ./rgrep '.\+\+'`" = "C++"
	test "`echo "question?" | ./rgrep 'on\?'`" = "question?"
	test "`echo "woooooo_CSE31.jpg" | ./rgrep 'w.+_...31\.jpg'`" = "woooooo_CSE31.jpg"

	@echo "Passed sanity check."

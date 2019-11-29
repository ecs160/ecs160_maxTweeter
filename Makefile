all: maxTweeter

maxTweeter: maxTweeter.c
	afl-clang -g -o maxTweeter maxTweeter.c

fuzz: maxTweeter
	afl-fuzz -i afl/in -o afl/out -- ./maxTweeter @@


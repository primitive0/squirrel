build-catch:
	g++ -Wall -Wextra -std=c++23 -c -o build/catch.o src/thirdparty/catch_amalgamated.cpp
	ar rcs build/libcatch.a build/catch.o

test:
	g++ -Wall -Wextra -std=c++23 -L./build -lcatch -o build/sq-tester src/utf8_test.cpp
	./build/sq-tester

make:
	mkdir -p build
	g++ -Wall -Wextra -std=c++23 -o build/sqd src/main.cpp

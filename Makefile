all:
	g++ parse2hpp.cpp -o parse2hpp
	g++ parse2json.cpp -o parse2json
debug:
	g++ -g parse2hpp.cpp -o parse2hpp
	g++ -g parse2json.cpp -o parse2json

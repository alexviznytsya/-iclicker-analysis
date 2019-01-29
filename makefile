build:
	g++ iclicker_analysis.cpp -o iclicker_analysis -std=c++11 -Wall
clean:
	rm iclicker_analysis

run:
	clear
	./iclicker_analysis
Output.exe: Output.cpp
	g++ -o Output.exe Output.cpp

run: Output.exe 
	python Output.py
	Output.exe
	python main.py
	
clean: 
	rm Output.exe
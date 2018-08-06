
compile:
	g++ Assign4.cpp Assign4helper.h ;
	touch compile
execute:compile
	./a.out  $(input1)  $(input2)
	touch execute
clean:
	\rm -rf *.out  compile execute *.log



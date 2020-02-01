cc=g++
cppversion=-std=c++17
cflage=-Wall -g
fflage=-l sqlite3 -pthread
project=../bin/httpServer

src=$(shell ls src/*.cpp) \
	$(shell ls src/database/*.cpp) \
	$(shell ls ../lib/src/*.cpp)

tmp0=$(subst src,obj,$(src))
tmp=$(subst database/,,$(tmp0))
obj=$(subst .cpp,.o,$(tmp))


server: $(obj)
	$(cc) $(cppversion) $(cflage) $(obj) -o $(project) $(fflage)
	$(project)


obj/%.o: src/%.cpp
	$(cc) $(cppversion) $(cflage) -c $< -o $@ $(fflage)

obj/%.o: src/database/%.cpp
	$(cc) $(cppversion) $(cflage) -c $< -o $@ $(fflage)

../lib/obj/%.o: ../lib/src/%.cpp
	$(cc) $(cppversion) $(cflage) -c $< -o $@ $(fflage)


clean:
	rm -rf obj/*.o
	rm -rf ../lib/obj/*.o
	rm -rf $(project)


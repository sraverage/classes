# Nome do compilador e flags
CXX = g++
CXXFLAGS = -O3

# Arquivos
TARGET = main
OBJS = fx.o main.o

# Regra principal
all: $(TARGET)

# Regra para gerar o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compila fx.cpp para fx.o
fx.o: fx.cpp fx.hpp exprtk.hpp
	$(CXX) $(CXXFLAGS) -c fx.cpp

# Compila main.cpp para main.o
main.o: main.cpp fx.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Limpa arquivos intermediários
clean:
	rm -f *.o $(TARGET)

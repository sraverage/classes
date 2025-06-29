#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "lista.hpp" // Inclua o cabeçalho da sua classe


#define private public
#include "lista.hpp"
#undef private

TEST_CASE("Nome do Caso de Teste (Ex: Testes da MinhaClasse)") {
    // Crie instâncias da sua classe
    SuaClasse obj;

    SUBCASE("Subcaso 1: Testando um método específico") {
        // Use as asserções do doctest
        CHECK(obj.algumMetodo(parametro) == valorEsperado);
        CHECK_FALSE(obj.outroMetodo());
    }

    SUBCASE("Subcaso 2: Testando outro cenário") {
        // Mais testes...
        REQUIRE(obj.metodoCritico() != nullptr);
    }
}

// Adicione mais TEST_CASEs conforme necessário para diferentes funcionalidades
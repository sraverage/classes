#ifndef PONTO_HPP
#define PONTO_HPP

#include <cmath>    // Para a função sqrt
#include <array>    // Para usar std::array
#include <numeric>  // Para usar std::inner_product (opcional, para uma solução mais elegante)

template<int dim>
class ponto {
public:
    std::array<double, dim> coords;

    // Construtor padrão que inicializa todas as coordenadas com zero
    ponto() {
        coords.fill(0.0);
    }
    
    // Construtor que inicializa todas as coordenadas com um valor específico
    ponto(double val) {
        coords.fill(val);
    }


    template<typename... Args, typename = std::enable_if_t<sizeof...(Args) == dim>>
    ponto(Args... args) : coords{{static_cast<double>(args)...}} {}
    
    
    // Funções para pegar as coordenadas (boa prática)
    double get_coord(int i) const {
        return coords.at(i);
    }
    
    double& get_coord(int i) {
        return coords.at(i);
    }

    
    double get_distance(const ponto<dim>& outro_ponto) const noexcept {
        double dist_squared = 0.0;
        for (int i = 0; i < dim; ++i) {
            dist_squared += pow(coords[i] - outro_ponto.coords[i], 2);
        }
        return sqrt(dist_squared);
    }

};

#endif
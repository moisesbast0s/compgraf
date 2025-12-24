#include "maploader.h"
#include <fstream>
#include <iostream>

// ... (Mantenha o método load existente aqui) ...
// Se não tiver o load aqui, avise que eu mando, mas vou focar na novidade:

bool MapLoader::load(const char* filename) {
    // ... sua implementação atual de leitura de arquivo ...
    // Vou assumir que ela preenche mapData, width e height corretamente.
    // Abaixo está um esqueleto caso precise, senão mantenha o seu.
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    std::string line;
    mapData.clear();
    while (std::getline(file, line)) {
        mapData.push_back(line);
    }
    if (!mapData.empty()) {
        height = mapData.size();
        width = mapData[0].size();
    }
    return true;
}

// A IMPLEMENTAÇÃO NOVA
char MapLoader::getBlock(int x, int z) const {
    // 1. Verificação de Limites (Bounds Checking)
    // Se a coordenada estiver fora do mapa, retornamos '1' (Parede) para bloquear.
    if (z < 0 || z >= height || x < 0 || x >= width) {
        return '1'; 
    }

    // 2. Acesso Direto: mapData[linha/z][coluna/x]
    // Proteção extra: verifica se a linha tem o tamanho certo
    if (x >= mapData[z].size()) {
        return '1';
    }

    return mapData[z][x];
}
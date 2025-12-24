# 🛠️ Atualização do Sistema de Colisão

Este documento detalha as alterações realizadas na engine para corrigir os bugs de "paredes invisíveis" e "atravessamento de muros"

Ao contrário de uma correção rápida (apenas no input), optamos por uma **Refatoração Estrutural** para alinhar perfeitamente o Visual (OpenGL) com a Lógica (Matemática).

---

## 📋 Resumo das Alterações

| Arquivo Modificado | O que mudou? | Motivo Técnico |
| :--- | :--- | :--- |
| **`drawlevel.cpp`** | **CRÍTICO:** Remoção da centralização do mapa (`LevelMetrics`). | O mapa agora é desenhado a partir da origem `(0,0)`, alinhando Visual e Grid. |
| **`utils/maploader`** | Adição da função `getBlock()`. | Proteção contra *Segmentation Fault* (Crash) se o jogador sair do mapa. |
| **`input.cpp`** | Nova lógica de colisão com compensação de Offset. | Compensa o fato do `glutSolidCube` ser desenhado pelo centro. |
| **`main.cpp`** | Remoção de `static` e ajuste de Spawn. | Permite acesso global ao mapa e corrige a posição inicial do jogador. |

---

## 🔍 Detalhes Arquivo por Arquivo

### 1. `utils/drawlevel.cpp` (A Raiz do Problema)
O código antigo "empurrava" o mapa para trás para que o centro da sala ficasse na coordenada `(0,0)`. Isso criava um **desalinhamento** entre onde víamos a parede e onde a matemática calculava a colisão.

* **❌ Removido:** Cálculo complexo de `m.tileCenter` e uso de `LevelMetrics` para offset de desenho.
* **✅ Adicionado:** Desenho simples e direto. O índice `0` do vetor vira a coordenada `0.0` no mundo 3D.
    ```cpp
    // Nova Lógica Simples
    float wx = x * TILE;
    float wz = z * TILE;
    ```

### 2. `utils/maploader.h` e `.cpp` (Segurança)
Acessar o vetor do mapa diretamente (`data[z][x]`) é perigoso. Se a física calculasse uma coordenada negativa (ex: `-1`), o jogo fechava sozinho.

* **✅ Adicionado:** Método `getBlock(x, z)`.
    * **Função:** Verifica se a coordenada está dentro dos limites. Se estiver fora, retorna `'1'` (Parede).
    * **Benefício:** Cria uma borda infinita de proteção. O jogo nunca crasha, mesmo se o jogador cair do mundo.

### 3. `input.cpp` (A Física)
Agora que o mapa começa no zero, a matemática ficou mais limpa, mas precisou de ajustes para o volume do jogador.

* **✅ Offset Matemático:** Adicionado `BLOCO_TAMANHO / 2.0f`.
    * *Motivo:* O OpenGL desenha o cubo a partir do centro. O offset alinha a matemática com o "umbigo" do cubo visual.
* **✅ Raio do Jogador:** Aumentado para `1.0f`.s
    * *Motivo:* Evita que a câmera atravesse a textura da parede antes da colisão ser detectada (o problema de "ver através do muro").


### 4. `main.cpp` (Configuração)
* **❌ Removido:** Palavra-chave `static` na declaração do `gMap`.
    * *Motivo:* `static` tornava o mapa invisível para o `input.cpp`.
* **✅ Ajuste de Spawn:** Mudança de `camX=0` para `camX=6.0f`.
    * *Motivo:* Como mudamos o desenho (ponto 1), a coordenada `0,0` virou o centro da primeira parede. Movemos o spawn para o centro do primeiro bloco vazio `(1,1)`.


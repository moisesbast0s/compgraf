#version 120

uniform float uTime;      // Tempo
uniform float uDistancia; // Distância do sensor

void main()
{
    // --- PARTE 1: A Cor Base (O que já tínhamos) ---
    float onda = (sin(uTime * 3.0) + 1.0) / 2.0;
    vec3 corVerde = vec3(0.0, 1.0, 0.0); // Verde Matrix
    vec3 corRoxa  = vec3(0.8, 0.0, 1.0); // Roxo Neon
    vec3 corAlerta = vec3(1.0, 0.0, 0.0); // Vermelho Perigo

    // Mistura normal
    vec3 corBase = mix(corVerde, corRoxa, onda);

    // Sensor de proximidade (se chegar perto, fica vermelho)
    float fatorAlerta = 1.0 - clamp((uDistancia - 3.0) / 7.0, 0.0, 1.0);
    vec3 corFinal = mix(corBase, corAlerta, fatorAlerta);


    // --- PARTE 2: O Efeito Holograma (Scanlines) ---
    
    // gl_FragCoord.y = A altura do pixel na sua tela (0 a 1080, por exemplo)
    // Nós usamos o SENO nessa altura para criar listras horizontais.
    // Multiplicamos uTime para as listras "subirem".
    float scanline = sin(gl_FragCoord.y * 0.1 - uTime * 10.0);

    // Se o seno for negativo, a gente "apaga" o pixel (escurece)
    // Isso cria o efeito de listras pretas intercaladas
    if (scanline < 0.0) 
    {
        // Escurece a cor em 50% nas faixas pretas
        corFinal = corFinal * 0.5; 
    }

    // --- PARTE 3: Brilho Extra ---
    // Adiciona um brilho pulsante nas listras claras
    float brilho = max(0.0, scanline); 
    corFinal = corFinal + (vec3(0.2, 0.2, 0.2) * brilho);


    // Saída final
    gl_FragColor = vec4(corFinal, 1.0);
}
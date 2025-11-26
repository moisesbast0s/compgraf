#version 120

void main()
{
    // Transforma o vértice do mundo 3D para a tela
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
varying vec4 lightPos;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    lightPos = gl_ModelViewMatrix * vec4(0.0, 0.0, 0.0, 1.0);
}

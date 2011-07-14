varying vec4 lightPos;
varying vec2 texPos;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    texPos = vec2(gl_Position / gl_Position.w) / 2 + vec2(0.5);
    lightPos = gl_ModelViewMatrix * vec4(0.0);
}

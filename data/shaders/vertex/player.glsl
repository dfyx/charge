varying vec3 normal;
varying vec3 texCoord;
varying vec4 worldPos;

void main()
{
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        worldPos = gl_ModelViewMatrix * gl_Vertex;

        gl_TexCoord[0] = gl_MultiTexCoord0;

        normal = gl_NormalMatrix * gl_Normal;
}

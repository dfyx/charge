varying vec3 normal;
varying vec3 texCoord;

void main()
{
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        vec3 vertex = vec3(gl_ModelViewMatrix * gl_Vertex);

        gl_TexCoord[0] = gl_MultiTexCoord0;

        normal = gl_NormalMatrix * gl_Normal;
}

uniform sampler2D diffuseBuffer, specularBuffer, positionBuffer, normalBuffer;
uniform vec4 ambientColor;

void main()
{
    gl_FragColor = texture2D(diffuseBuffer, gl_TexCoord[0].st) * ambientColor;
}

uniform sampler2D diffuseBuffer, specularBuffer, positionBuffer, normalBuffer;
uniform vec4 lightColor;
varying vec4 lightPos;
varying vec2 texPos;

void main()
{
    gl_FragColor = texture2D(diffuseBuffer, texPos) * lightColor;
}

uniform sampler2D diffuseBuffer, specularBuffer, positionBuffer, normalBuffer;
uniform vec4 lightColor;
varying vec4 lightPos;
varying vec2 texPos;

void main()
{
    vec4 diffuse = texture2D(diffuseBuffer, texPos);
    vec4 specular = texture2D(specularBuffer, texPos);
    vec4 position = texture2D(positionBuffer, texPos);
    vec4 normal = texture2D(normalBuffer, texPos);

    vec4 diff = position - lightPos;
    float distSquared = dot(diff, diff);
    gl_FragColor = specular * lightColor * lightColor.a / distSquared;
}

varying vec3 normal;
varying vec3 texCoord;
uniform sampler2D diffuseTexture, specularTexture;

void main()
{
    gl_FragData[0] = texture2D(diffuseTexture, gl_TexCoord[0].st);
    gl_FragData[1] = texture2D(specularTexture, gl_TexCoord[0].st);
    gl_FragData[2] = gl_FragCoord;
    gl_FragData[3] = vec4(normalize(normal), 1.0);
}

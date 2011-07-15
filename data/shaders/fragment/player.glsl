varying vec3 normal;
varying vec3 texCoord;
varying vec4 worldPos;
uniform sampler2D diffuseTexture, specularTexture, identifierTexture;
uniform vec3 playerColor;

void main()
{
    vec4 diffuse = texture2D(diffuseTexture, gl_TexCoord[0].st);
    vec4 identifier = texture2D(identifierTexture, gl_TexCoord[0].st);
    gl_FragData[0] = diffuse * (1.0 - identifier.a) + identifier * vec4(playerColor, 1.0) * identifier.a;
    gl_FragData[1] = texture2D(specularTexture, gl_TexCoord[0].st);
    gl_FragData[2] = worldPos;
    gl_FragData[3] = vec4(normalize(normal), 1.0);
}

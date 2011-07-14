uniform sampler2D diffuseBuffer, specularBuffer, positionBuffer, normalBuffer;
uniform vec2 screenSize;
uniform vec4 lightColor;
varying vec4 lightPos;

vec4 phongColor(in vec3 normal, in vec3 lightDir, in vec3 eyeDir,
    in vec4 ambientColor, in vec4 diffuseColor, in vec4 specularColor,
    in float shininess)
{
    // diffuse
    vec4 diffuse = vec4(0.0);
    float NdotL = dot(normal, lightDir);
    if(NdotL > 0.0)
    {
            diffuse = diffuseColor * NdotL;
    }

    // specular
    vec4 specular = vec4(0.0);
    vec3 R = normal * 2.0 * NdotL - lightDir;
    float RdotV = dot(R, eyeDir);
    if(RdotV > 0.0)
    {
            specular = specularColor * pow(RdotV, shininess);
    }

    vec4 result = ambientColor + diffuse + specular;
    result.a = ambientColor.a;
    return result;
}

void main()
{
    vec2 texPos = gl_FragCoord.xy / screenSize;
    vec4 diffuse = texture2D(diffuseBuffer, texPos);
    vec4 specular = texture2D(specularBuffer, texPos);
    vec4 position = texture2D(positionBuffer, texPos);
    vec3 normal = vec3(texture2D(normalBuffer, texPos));

    vec3 lightDir = normalize(vec3(lightPos - position));
    vec3 eyeDir = normalize(vec3(-position));

    vec4 diff = position - lightPos;
    float distSquared = dot(diff, diff);
    gl_FragColor = phongColor(normal, lightDir, eyeDir,
        vec4(0.0), diffuse, specular, 100.0) * lightColor * lightColor.a / distSquared;
}

#version 410
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPos; 

uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 lightPos; 


void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec4 objectColor = texture(ourTexture, TexCoord);
    vec3 result = diffuse * vec3( objectColor );
    FragColor = vec4(result, 1.0);
}
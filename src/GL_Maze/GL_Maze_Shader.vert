// Shader-ul de varfuri  
#version 400

layout (location = 0) in vec4 in_Position; // pozitia este atribut standard
layout (location = 1) in vec3 in_Color; // culoarea este atribut instantiat
layout (location = 2) in mat4 modelMatrix; // matricea de transformare este atribut instantiat
layout (location = 3) in vec3 in_Normal;

out vec4 gl_Position; 
out vec4 ex_Color;
 
 uniform mat4 matrUmbra;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform int codCol;
 
void main(void)
  {

	    gl_Position = projectionMatrix*viewMatrix*modelMatrix*in_Position;
        vec3 Normal=mat3(projectionMatrix*viewMatrix*modelMatrix)*in_Normal; 
        vec3 inLightPos= vec3(projectionMatrix*viewMatrix*modelMatrix* vec4(lightPos, 1.0f));
        vec3 inViewPos=vec3(projectionMatrix*viewMatrix*modelMatrix*vec4(viewPos, 1.0f));
        vec3 FragPos = vec3(gl_Position);

        // Ambient
        float ambientStrength = 0.5f;
        vec3 ambient = ambientStrength * lightColor;
  	
        // Diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(inLightPos - FragPos);
        // vec3 lightDir = normalize(-inLightPos); // pentru sursa directionala
        float diff = max(dot(norm, lightDir), 0.0);
        // vec3 diffuse = diff * lightColor;
        vec3 diffuse = pow(diff,0.2) * lightColor; // varianta de atenuare
    
        // Specular
        float specularStrength = 0.7f;
        vec3 viewDir = normalize(inViewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);
        vec3 specular = specularStrength * spec * lightColor;  
        
        vec3 result = (ambient + diffuse ) * in_Color;
	    ex_Color = vec4(result, 1.0f);
 } 
 
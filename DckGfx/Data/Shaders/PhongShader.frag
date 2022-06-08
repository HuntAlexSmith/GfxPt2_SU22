#version 450 core

// How many lights we will allow at one time
const int maxLights = 10;

// Input variables from the vertex shader
in vec3 myColor;
in vec4 worldPos;
in vec4 worldNorm;

// Uniform for tinting the object a certain color
uniform vec3 tint;

// Uniform for ignoring lighting (debug stuff)
uniform int ignoreNorm;

// Uniform for the camera eye position
uniform vec4 eyePos;

// Uniform for ambient color
uniform vec3 ambientColor;

// Uniform for how many lights there are
uniform int lightCount;

// Uniforms for light values
uniform vec4 lightPos[maxLights];
uniform vec3 lightColor[maxLights];

// Uniforms for diffuse and specular calculations (material)
uniform vec3 diffuseCoeff;
uniform vec3 specularCoeff;
uniform float specularExp;

// Output for the actual color
out vec4 fragColor;

void main() {
    // If we are ignoring the normals, we can just output the color
    if(ignoreNorm == 1 || lightCount == 0)
    {
        fragColor = vec4(myColor+tint, 1);
    }

    // Otherwise, we can do phong lighting calculations
    else
    {
        // Get normalized world normal
        vec4 nWorldNorm = normalize(worldNorm);

        // Start sum for final pixel color
        vec3 finalColor = diffuseCoeff * ambientColor;
        
        for(int i = 0; i < lightCount; ++i)
        {
            // Get light vector
            vec4 lightVec = normalize(lightPos[i] - worldPos);

            // Get dot product of normal and light
            float normDotLight = dot(nWorldNorm, lightVec);

            // If value is >0, light is doing something
            if(normDotLight > 0)
            {
                // Add calculation of light color to final color
                finalColor += diffuseCoeff * (normDotLight * lightColor[i]);

                // Get viewing vector
                vec4 viewingVec = normalize(eyePos - worldPos);

                // Find vector of perfect specular reflection
                vec4 perfSpec = normalize(((2*normDotLight)*nWorldNorm)-lightVec);

                // Get dot product of perfect specular and viewing vectors
                float perfDotView = dot(perfSpec, viewingVec);

                // If value is >0, light is doing something
                if(perfDotView > 0)
                {
                    finalColor += specularCoeff * (pow(perfDotView, specularExp) * lightColor[i]);
                }
            }
        }

        // Output the final color
        fragColor = vec4(finalColor, 1);
    }
}
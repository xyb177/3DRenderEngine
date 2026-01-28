#version 330 core

in vec2 pass_textureCoordinates;
in vec3 surfaceNormal;
in vec3 toLightVector[4];
in vec3 toCameraVector;
in float visibility;
in vec4 shadowCoords;
in float shadowFade;

out vec4 out_Color;

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;
uniform sampler2D shadowMap;

uniform vec3 lightColour[4];
uniform vec3 attenuation[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColour;

const int pcfCount = 2;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
float lightFactor;
void main(void){
	vec4 processedShadowCoords = shadowCoords;
	if (processedShadowCoords.z > 1.0) {
        processedShadowCoords.z = 1.0;
    } else if (processedShadowCoords.z < -1.0) {
        processedShadowCoords.z = -1.0;
    }
    
    float mapSize = 4096.0;
    float texelSize = 1.0 / mapSize;
    float total = 0.0;
    float lightFactor;

    for (int x = -pcfCount; x <= pcfCount; x++) {
        for (int y = -pcfCount; y <= pcfCount; y++) {
            vec2 sampleCoord = processedShadowCoords.xy + vec2(x, y) * texelSize;
            if (sampleCoord.x < 0.0 || sampleCoord.x > 1.0 || 
                sampleCoord.y < 0.0 || sampleCoord.y > 1.0) {
                continue;
            }
            
            float objectNearestLight = texture(shadowMap, sampleCoord).r;
            if (processedShadowCoords.z > objectNearestLight) {
                total += 1.0;
            }
        }
    }
    
    total /= totalTexels;
    lightFactor = 1.0 - (total * processedShadowCoords.w);
    

	vec4 blendMapColour = texture(blendMap,pass_textureCoordinates);
	float backTextureAmount = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);
	vec2 titledCoords = pass_textureCoordinates * 20.0;
	vec4 backgroundTextureColour = texture(backgroundTexture,titledCoords) * backTextureAmount;
	vec4 rTextureColour = texture(rTexture,titledCoords) * blendMapColour.r;
	vec4 gTextureColour = texture(gTexture,titledCoords) * blendMapColour.g;
	vec4 bTextureColour = texture(bTexture,titledCoords) * blendMapColour.b;

	vec4 totalColour = backgroundTextureColour + rTextureColour + gTextureColour + bTextureColour;

    vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);

	for( int i=0;i<4;i++)
	{	float distance = length(toLightVector[i]);
		float attFactor = attenuation[i].x + (attenuation[i].y*distance) + (attenuation[i].z*distance*distance);
		vec3 unitLightVector = normalize(toLightVector[i]);
		float nDotl = dot(unitNormal,unitLightVector);
		float brightness = max(nDotl,0.0);
    	vec3 lightDirection = -unitLightVector;
    	vec3 reflectedLightDirection = reflect(lightDirection,unitNormal);
    	float specularFactor = dot(reflectedLightDirection , unitVectorToCamera);
		specularFactor = max(specularFactor,0.0);
		float dampedFactor = pow(specularFactor,shineDamper);
		totalDiffuse  =  totalDiffuse + brightness * lightColour[i] / attFactor;
		totalSpecular =  totalSpecular + dampedFactor * reflectivity * lightColour[i] /attFactor;
	}
	totalDiffuse = max(totalDiffuse * lightFactor,0.4) ;

    out_Color =  vec4(totalDiffuse,1.0) * totalColour + vec4(totalSpecular,1.0);
	out_Color = mix(vec4(skyColour,1.0),out_Color,visibility);
}

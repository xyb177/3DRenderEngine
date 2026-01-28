#include "TerrainShader.h"

const std::string TerrainShader::VERTEX_FILE = "src/glsl/TerrainVertexShader.glsl";
const std::string TerrainShader::FRAGMENT_FILE = "src/glsl/TerrainFragmentShader.glsl";


TerrainShader::TerrainShader()
                 : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) { initUniforms(); }



void TerrainShader::getAllUniformLocations() {
    location_transformationMatrix = getUniformLocation("transformationMatrix");
    location_projectionMatrix = getUniformLocation("projectionMatrix");
    location_viewMatrix = getUniformLocation("viewMatrix");
    location_shineDamper = getUniformLocation("shineDamper");
    location_reflectivity = getUniformLocation("reflectivity");
    location_skyColour = getUniformLocation("skyColour");
    location_backgroundTexture = getUniformLocation("backgroundTexture");
    location_rTexture = getUniformLocation("rTexture");
    location_gTexture = getUniformLocation("gTexture");
    location_bTexture = getUniformLocation("bTexture");
    location_blendMap = getUniformLocation("blendMap");
    location_plane = getUniformLocation("plane");
    location_toShadowMapSpace = getUniformLocation("toShadowMapSpace");
    location_shadowMap = getUniformLocation("shadowMap");

    location_lightPosition.resize(MAX_LIGHTS);
    location_lightColour.resize(MAX_LIGHTS);
    location_attenuation.resize(MAX_LIGHTS);
    for ( int i=0;i<MAX_LIGHTS;i++)
    {
        location_lightPosition[i] = getUniformLocation("lightPosition[" + std::to_string(i) + "]");
        location_lightColour[i] = getUniformLocation("lightColour[" + std::to_string(i) + "]");
        location_attenuation[i] = getUniformLocation("attenuation[" + std::to_string(i) + "]");
    }
    std::cout << "View Matrix Location: " << location_viewMatrix << std::endl;
    std::cout << "Projection Matrix Location: " << location_projectionMatrix << std::endl;
    std::cout << "Transformation Matrix Location: " << location_transformationMatrix << std::endl;
    std::cout << "  Shine Damper: " << location_shineDamper << std::endl;
    std::cout << "  Reflectivity: " << location_reflectivity << std::endl;
    std::cout << "  rTexture " << location_rTexture << std::endl;
    std::cout << "  gTexture " << location_gTexture << std::endl;
    std::cout << "  bTexture " << location_bTexture << std::endl;
    std::cout << "  blendMap " << location_blendMap << std::endl;

    if (location_projectionMatrix == -1 || 
        location_transformationMatrix == -1 || 
        location_viewMatrix == -1) {
        std::cerr << "Error: Failed to find essential matrix uniform locations!" << std::endl;
    }
     
}

void TerrainShader::loadShineVariables(float damper, float reflectivity) {
    loadFloat(location_shineDamper, damper);
    loadFloat(location_reflectivity, reflectivity);
}

void TerrainShader::loadTransformationMatrix(const glm::mat4& matrix) {
    loadMatrix(location_transformationMatrix, matrix);
}

void TerrainShader::loadLight(const std::vector<Light> &light) {
   for( int i=0;i< MAX_LIGHTS;i++)
    {
        if (i<light.size()){
            loadVector(location_lightPosition[i],light[i].getPosition());
            loadVector(location_lightColour[i],light[i].getColour());
            loadVector(location_attenuation[i], light[i].getAttenuation());
        } else{
            loadVector(location_lightPosition[i],glm::vec3(0,0,0));
            loadVector(location_lightColour[i],glm::vec3(0,0,0));
            loadVector(location_attenuation[i], glm::vec3(1,0,0));
        }  
    }
}

void TerrainShader::loadViewMatrix(const Camera& camera) {
    glm::mat4 viewMatrix = Maths::createViewMatrix(camera);
    loadMatrix(location_viewMatrix, viewMatrix);
}

void TerrainShader::loadProjectionMatrix(const glm::mat4& projection) {
    loadMatrix(location_projectionMatrix, projection);
}

void TerrainShader::loadSkyColour(float r, float g, float b)
{
    loadVector(location_skyColour,glm::vec3(r,g,b));
}

void TerrainShader::connectTextureUnits()
{
    loadInt(location_backgroundTexture,0);
    loadInt(location_rTexture,1);
    loadInt(location_gTexture,2);
    loadInt(location_bTexture,3);
    loadInt(location_blendMap,4);
    loadInt(location_shadowMap,5);
}

void TerrainShader::loadClipPlane(glm::vec4 plane)
{
    loadVector(location_plane,plane);
}

void TerrainShader::loadToShadowSpaceMatrix(glm::mat4 matrix)
{
    loadMatrix(location_toShadowMapSpace, matrix);
}

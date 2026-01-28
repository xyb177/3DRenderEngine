#include "StaticShader.h"
#include "Maths.h"

const std::string StaticShader::VERTEX_FILE = "src/glsl/vertexShader.glsl";
const std::string StaticShader::FRAGMENT_FILE = "src/glsl/fragmentShader.glsl";

StaticShader::StaticShader()
    : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) { initUniforms(); }

void StaticShader::getAllUniformLocations()
{
    location_transformationMatrix = getUniformLocation("transformationMatrix");
    location_projectionMatrix = getUniformLocation("projectionMatrix");
    location_viewMatrix = getUniformLocation("viewMatrix");
    location_cameraPosition = getUniformLocation("cameraPosition");
    location_shineDamper = getUniformLocation("shineDamper");
    location_reflectivity = getUniformLocation("reflectivity");
    location_useFakeLighting = getUniformLocation("useFakeLighting");
    location_skyColour = getUniformLocation("skyColour");
    location_numberOfRows = getUniformLocation("numberOfRows");
    location_offset = getUniformLocation("offset");
    location_plane = getUniformLocation("plane");
    location_lightPosition.resize(MAX_LIGHTS);
    location_lightColour.resize(MAX_LIGHTS);
    location_attenuation.resize(MAX_LIGHTS);
    for ( int i=0;i<MAX_LIGHTS;i++)
    {
        location_lightPosition[i] = getUniformLocation("lightPosition[" + std::to_string(i) + "]");
        location_lightColour[i] = getUniformLocation("lightColour[" + std::to_string(i) + "]");
        location_attenuation[i] = getUniformLocation("attenuation[" + std::to_string(i) + "]");
    }

    location_specularMap = getUniformLocation("specularMap");
    location_useSpecularMap = getUniformLocation("useSpecularMap");
    location_modelTexture = getUniformLocation("modelTexture");
    location_environmentMap = getUniformLocation("environmentMap");
    location_useEnvironmentMap = getUniformLocation("useEnvironmentMap");
    location_reflectionStrength = getUniformLocation("reflectionStrength");

    std::cout << "View Matrix Location: " << location_viewMatrix << std::endl;
    std::cout << "Projection Matrix Location: " << location_projectionMatrix << std::endl;
    std::cout << "Transformation Matrix Location: " << location_transformationMatrix << std::endl;
    std::cout << "  Light Position: " << location_lightPosition[0] << std::endl;
    std::cout << "  Light Colour: " << location_lightColour[0] << std::endl;
    std::cout << "  Shine Damper: " << location_shineDamper << std::endl;
    std::cout << "  Reflectivity: " << location_reflectivity << std::endl;
    std::cout << "  specularMap: " << location_specularMap << std::endl;
    std::cout << "  useSpecularMap: " << location_useSpecularMap << std::endl;

    if (location_projectionMatrix == -1 || 
        location_transformationMatrix == -1 || 
        location_viewMatrix == -1) {
        std::cerr << "Error: Failed to find essential matrix uniform locations!" << std::endl;
    }
    
}

void StaticShader::loadTransformationMatrix(const glm::mat4 &matrix)
{
    loadMatrix(location_transformationMatrix, matrix);
}

void StaticShader::loadViewMatrix(const Camera& camera) {
    glm::mat4 viewMatrix = Maths::createViewMatrix(camera);
    loadMatrix(location_viewMatrix, viewMatrix);
    loadVector(location_cameraPosition, camera.getPosition());
}

void StaticShader::loadProjectionMatrix(const glm::mat4& projection) { 
    loadMatrix(location_projectionMatrix, projection);
}

void StaticShader::loadLight(const std::vector<Light> &light)
{   for( int i=0;i< MAX_LIGHTS;i++)
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

void StaticShader::loadShineVariables(float damper, float reflectivity)
{
    loadFloat(location_shineDamper,damper);
    loadFloat(location_reflectivity,reflectivity);
}

void StaticShader::loadFakeLightingVariable(bool useFake)
{
    loadBoolean(location_useFakeLighting, useFake);
}

void StaticShader::loadSkyColour(float r, float g, float b)
{
    loadVector(location_skyColour,glm::vec3(r,g,b));
}

void StaticShader::loadNumberOfRow(int n)
{
    loadFloat(location_numberOfRows,n);
}

void StaticShader::loadOffset(float x, float y)
{
    load2DVector(location_offset,glm::vec2(x,y));
}

void StaticShader::loadClipPlane(glm::vec4 plane)
{
    loadVector(location_plane,plane);
}

void StaticShader::connectTextureUnits()
{
    loadInt(location_modelTexture, 0);
    loadInt(location_specularMap, 1);
    loadInt(location_environmentMap,2);
}

void StaticShader::loadUseSpecularMap(bool useMap)
{
    loadBoolean(location_useSpecularMap, useMap);
}


void StaticShader::loadUseEnvironmentMap(bool useMap, float strength)
{
    loadBoolean(location_useEnvironmentMap, useMap);
    loadFloat(location_reflectionStrength, strength);
}

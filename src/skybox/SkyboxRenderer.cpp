#include "SkyboxRenderer.h"

const std::vector<float> SkyBoxRenderer::vertices = {
        -SIZE,  SIZE, -SIZE,
	    -SIZE, -SIZE, -SIZE,
	    SIZE, -SIZE, -SIZE,
	     SIZE, -SIZE, -SIZE,
	     SIZE,  SIZE, -SIZE,
	    -SIZE,  SIZE, -SIZE,

	    -SIZE, -SIZE,  SIZE,
	    -SIZE, -SIZE, -SIZE,
	    -SIZE,  SIZE, -SIZE,
	    -SIZE,  SIZE, -SIZE,
	    -SIZE,  SIZE,  SIZE,
	    -SIZE, -SIZE,  SIZE,

	     SIZE, -SIZE, -SIZE,
	     SIZE, -SIZE,  SIZE,
	     SIZE,  SIZE,  SIZE,
	     SIZE,  SIZE,  SIZE,
	     SIZE,  SIZE, -SIZE,
	     SIZE, -SIZE, -SIZE,

	    -SIZE, -SIZE,  SIZE,
	    -SIZE,  SIZE,  SIZE,
	     SIZE,  SIZE,  SIZE,
	     SIZE,  SIZE,  SIZE,
	     SIZE, -SIZE,  SIZE,
	    -SIZE, -SIZE,  SIZE,

	    -SIZE,  SIZE, -SIZE,
	     SIZE,  SIZE, -SIZE,
	     SIZE,  SIZE,  SIZE,
	     SIZE,  SIZE,  SIZE,
	    -SIZE,  SIZE,  SIZE,
	    -SIZE,  SIZE, -SIZE,

	    -SIZE, -SIZE, -SIZE,
	    -SIZE, -SIZE,  SIZE,
	     SIZE, -SIZE, -SIZE,
	     SIZE, -SIZE, -SIZE,
	    -SIZE, -SIZE,  SIZE,
	     SIZE, -SIZE,  SIZE
    };

SkyBoxRenderer::SkyBoxRenderer(Loader &loader, const glm::mat4 &projectionMatrix)
                :cube(loader.loadToVAO(vertices,3)),texture(loader.loadCubeMap(TEXTURE_FILES)),nightTexture(loader.loadCubeMap(NIGHT_TEXTURE_FILES))
{
    shader.start();
	shader.connectTextureUnits();
    shader.loadProjectionMatrix(projectionMatrix);
    shader.stop();
}

void SkyBoxRenderer::render(const Camera &camera, float r, float g, float b)
{
    shader.start();
    shader.loadViewMatrix(camera);
	shader.loadFogColour(r,g,b);
    glBindVertexArray(cube.getVaoID());
    glEnableVertexAttribArray(0);
    bindTextures();
    glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount());
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    shader.stop();
    
}

void SkyBoxRenderer::bindTextures()
{	time += DisplayManager::getInstance().getFrameTimeSeconds()* 2400;
	time = std::fmod(time, 24000.0f);

	int texture1;
	int texture2;
	float blendFactor;		
	if(time >= 0 && time < 5000){
		texture1 = nightTexture;
		texture2 = nightTexture;
		blendFactor = (time - 0)/(5000 - 0);
	}else if(time >= 5000 && time < 8000){
		texture1 = nightTexture;
		texture2 = texture;
		blendFactor = (time - 5000)/(8000 - 5000);
	}else if(time >= 8000 && time < 21000){
		texture1 = texture;
		texture2 = texture;
		blendFactor = (time - 8000)/(21000 - 8000);
	}else{
		texture1 = texture;
		texture2 = nightTexture;
		blendFactor = (time - 21000)/(24000 - 21000);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP,texture1);
	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP,texture2);
	shader.loadBlendFactor(blendFactor);
}


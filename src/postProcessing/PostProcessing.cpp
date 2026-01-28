#include "PostProcessing.h"
PostProcessing* PostProcessing::instance = nullptr;
const std::vector<float> PostProcessing::POSITIONS = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };

PostProcessing& PostProcessing::getInstance() {
    if (instance == nullptr) {
        instance = new PostProcessing();
    }
    return *instance;
}

void PostProcessing::init(Loader& loader) {
    // 加载全屏四边形到VAO
    quad = loader.loadToVAO(POSITIONS, 2);
    int width, height;
    GLFWwindow* window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &width, &height); 
    hBlur = new HorizontalBlur(width/5, height/5);
    vBlur = new VerticalBlur(width/5,height/5);
    brightFilter = new BrightFilter(width/2, height/2);
    combineFilter = new CombineFilter();
}

void PostProcessing::doPostProcessing(GLuint colourTexture) {
    start();
    
    brightFilter->render(colourTexture);
    hBlur->render(brightFilter->getOutputTexture());
    vBlur->render(hBlur->getOutputTexture());
    combineFilter->render(colourTexture,vBlur->getOutputTexture());
    end();
}

void PostProcessing::cleanUp() {
    contrastChanger.cleanUp();
    hBlur->cleanUp();
    vBlur->cleanUp();
}

void PostProcessing::start() {
    // 绑定VAO和启用顶点属性
    glBindVertexArray(quad.getVaoID());
    glEnableVertexAttribArray(0);
    
    // 禁用深度测试（后期处理通常不需要深度信息）
    glDisable(GL_DEPTH_TEST);
}

void PostProcessing::end() {
    // 恢复深度测试
    glEnable(GL_DEPTH_TEST);
    
    // 禁用顶点属性和解绑VAO
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}
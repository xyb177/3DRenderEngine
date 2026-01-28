#include "FontRenderer.h"

FontRenderer::FontRenderer()
{
}
void FontRenderer::render(std::unordered_map<FontType*, std::vector<GUIText*>> texts)
{
    prepare();
    for (auto& [font, textList]:texts)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,font->getTextureAtlas());
        for (auto& text : textList)
        {
            renderText(*text);
        }
    }
}

void FontRenderer::prepare(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    shader.start();

}

void FontRenderer::renderText(const GUIText& text)
{
    glBindVertexArray(text.getMesh());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    shader.loadColour(text.getColour());
    shader.loadTranslation(text.getPosition());
    glDrawArrays(GL_TRIANGLES,0,text.getVertexCount());
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}

void FontRenderer::endRendering(){
    shader.stop();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void FontRenderer::cleanUp()
{
    shader.cleanUp();
}
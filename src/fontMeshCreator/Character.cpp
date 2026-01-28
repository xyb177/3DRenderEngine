#include "Character.h"

Character::Character(int id, 
                   double xTextureCoord, 
                   double yTextureCoord,
                   double xTexSize,
                   double yTexSize,
                   double xOffset,
                   double yOffset,
                   double sizeX,
                   double sizeY,
                   double xAdvance)
    : id(id),
      xTextureCoord(xTextureCoord),
      yTextureCoord(yTextureCoord),
      xOffset(xOffset),
      yOffset(yOffset),
      sizeX(sizeX),
      sizeY(sizeY),
      xMaxTextureCoord(xTexSize + xTextureCoord),
      yMaxTextureCoord(yTexSize + yTextureCoord),
      xAdvance(xAdvance) {}

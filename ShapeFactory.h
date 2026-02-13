#ifndef GRAPHICSENGINE_SHAPEFACTORY_H
#define GRAPHICSENGINE_SHAPEFACTORY_H

class Mesh;

class ShapeFactory {
public:
    ShapeFactory();
    Mesh CreateQuad();
    Mesh CreateCube();
};

#endif //GRAPHICSENGINE_SHAPEFACTORY_H

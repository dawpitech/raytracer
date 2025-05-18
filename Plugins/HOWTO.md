# Raytracer plugins 101

To implement a plugin for this raytracer 3000 you need to implement
the RACIST standard (Raytracer Advanced Complementary Interfaces for Solids and Transforms)

A plugin can either be a Primitive, or a Material
Every plugin need to register corresponding primitive or material name.
Every plugin need to implement a libconfig++ parser to parse its components
and return a valid Object or Material.

For a basic implementation to follow as a guide, you can check the source
code of the Sphere Primitive, it's well documented to serve as a guide.

Regarding the AABB Algorithm, when implementing a primitive you also need
to return a corresponding 3D virtual box that will encapsulate the whole
object, if a part of the object is not in this box, it could not be rendered.

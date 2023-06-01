# VectorFontStorm
C++ vector font rendering library for OpenGL 3D text drawing, by VoxelStorm.

This library was developed for the 3D text prominently used in [sphereFACE](http://sphereface.com).

It produces optimal triangulated 3D polygon models of text from arbitrary fonts, using delaunay triangulation in 2D, and extrusion in 3D.

This is a legacy project that is not currently maintained, having been split out from a larger game codebase, but is provided in the hope it will be useful to someone.  The code has not been reviewed for many years and is not of the best quality.  Pull requests welcome.

## Dependencies
- Freetype2
- OpenGL
- GLFW (probably not actually required - just the includes need to be cleaned up)
- GLEW (probably not actually required - just the includes need to be cleaned up)
- [poly2tri](https://github.com/jhasse/poly2tri) (included) - 2D constrained Delaunay triangulation library
- [timestorm](https://github.com/VoxelStorm-Ltd/timestorm) - for diagnostics only, should be made optional

## Usage

Full documentation TODO.

## Example results
The following screenshots from sphereFACE demonstrate the output.

![image](https://github.com/VoxelStorm-Ltd/vectorfontstorm/assets/649131/9d4555ab-9fc6-4727-b045-6dff455bc4a0)
![image](https://github.com/VoxelStorm-Ltd/vectorfontstorm/assets/649131/74134456-f33b-4379-a43b-1a80732fc374)

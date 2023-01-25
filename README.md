# Tanked

A 3rd-person shoot 'em up video game, set in a futuristic, procedurally generated maze-like world. Everything was written from the ground up, with a custom game engine targeting OpenGL 3.3. The project was intended for a university competition, in which it was deemed as the best one and was subsequently showed at the university's expo (the only project from the School of Computing). The game was a group effort (a group of 2-3), as we needed help with matters unrelated to programming (like 3D assets, sounds). My biggest contributions were all graphics effects, renderer, AI system and pathfinding, big portion of the game engine, some gameplay mechanics. I've also written design and technical documentation that I used to present the finished product to a comity (comprised from university professors and developers from game industry).

![tanked-1](https://i.imgur.com/UWnD1Qf.png)

## Features:
 - A* pathfinding
 - Audio (FMOD wrapper)
 - Bloom / emissive effects with Gaussian blur
 - Collision solver (circles, points, rays, AAB, OBB)
 - Deferred renderer
 - Frustum culling (based on view-frustum)
 - GUI (component based UI system)
 - Modular design (different systems are decoupled from each other)
 - Multi-threading (collision solver and scene graph update)
 - Particle system (multiple effects, gravity, collision with floor)
 - Physically based rendering (Cook-Torrance BRDF)
 - Procedural levels
 - Scene graph structure used by all game objects
 
# Graphics:

**Bloom effect** - a pseudo HDR effect is used that writes over-brightened pixel's color (before clamping it to 0-1 range) to the emissive buffer that is later bled out to adjacent pixels by Gaussian blur. This eliminates over-brightened artifacts while still keeping the effect of ultra bright pixels.

**Deferred renderer** - uses a g-buffer to decouple the lighting calculations from geometry rendering. Shading is carried out on visible pixels only, thus eliminating unnecessary calculations and handling any depth complexity. Allows easy integration of new effects and as everything is handled as a screen-space post-processing effect, meaning fast and independent of the scene complexity.

**Gaussian blur** - a customizable blur pass is implemented that is used for emissive textures and bloom effect. The Gaussian kernel is pre-computed to save from unnecessary calculations and the blur pass is divided into vertical and horizontal passes to minimize expensive texture lookups.

**GUI** - component based UI system. Text rendering allows exchangeable fonts. HUD drawing is implemented as a collection of multiple UI elements.

**Lighting** - all lighting calculations are being done as a screen-space effect, thus allowing a very high number of dynamic lights in the scene.

**Parallax mapping** - the parallax displacement effect is heavily used in the game so objects appear to be 3D without rendering extra geometry. Almost all the assets contain height maps used for displacement. The most noticeable effect is on the walls in the maze.

**Physically** based rendering - the renderer was later overhauled to support PBR, using BRDFs comprised of lambertian model for diffuse reflection and Cook-Torrance model for specular reflections (Beckman for microfacet distribution; Schlick's approximation for Fresnel effect; Schlick/GGX for geometry attenuation).

Engine:

**Audio** - uses FMOD for audio library. All sounds are handled via wrappers, that encapsulates low level functionality. Wrappers are stored in unordered maps (average case: constant). Supports streaming for background music.

**Frustum culling** - optimization by removing graphics objects outside of the view-projection matrix (view frustum) from the rendering process.

**Data handling** - visitor pattern (double dispatch) is used for object rendering. A snapshot of the current scene is created each frame, thus decoupling rendering data from other components (beneficial for multithreading).

**Modular design** - all the elements within the engine are separate and modular, allowing swapping certain parts in and out, or disabling various nodes altogether. Every object is created and governed using factories.

**Multi-threading** - the multi-threading was utilized very late in the project to accelerate the tasks that take up the most CPU time. Multi-threading was accomplished with SDL threads and was implemented in collision solver and game object updates within the scene graph.

**Scene** - the objects within the scene are contained in a scene-graph. With parent/children pairing allowing for easy object transform manipulation in the 3D space.

# Gameplay:

**Enemy AI** - all enemies in the game are directed by simple AI rules to always seek the player. Once they are in a direct line of sight, they charge the player directly facing them. During the combat in an open area, the enemies have a delay on the players last position, allowing the player strafe and dodge, making the combat easier and more fun.

**Pathfinding** - the enemies in the game (robots) uses A-STAR pathfinding. The scene is divided into a grid, and the pathfinding is only calculated for movements between the nodes in the grid, thus the enemies do not need to recalculate the path until they exit their occupied node. If the enemy has a direct sight of the player, it foregoes the pathfinding and travels directly to its destination. The movement cost is calculated based on the Manhattan model for distance between adjacent and diagonal nodes.

**Procedural levels** - the levels in the game are procedurally generated in a maze-like fashion. The maze generator creates rooms at the predetermined positions (for enemy generators and player's starting position) and traces the paths to guarantee that all the rooms are reachable. The map itself is modularly constructed from wall and floor nodes.

**Tank upgrades** - the weapons of the player's tank are designed to be modular and certain parts can be swapped out and upgraded (although the current game state only has one set of weapons).

# Physics:

**Collisions** - custom collision solver that handles simple interactions between circles, AAB, OBB, points and rays. As an optimization the scene is divided into quad trees that are used to generate the collision pairs.

**Particle system** - a generic particle system is implemented with multiple effects (sparks, waves, radial and box explosions, hover effect for the tank). It uses GL_POINTS with varying colors, has gravity calculations and collisions with the floor. All calculations are done on the CPU, before the buffers are sent to the GPU for rendering.


## Screenshots:

![tanked-2](https://i.imgur.com/m2ZptrO.png)
![tanked-3](https://i.imgur.com/cfu6o0j.png)
![tanked-4](https://i.imgur.com/TfW2EAT.png)


## Dependencies
Libraries used in this project:  

- [freeglut](https://freeglut.sourceforge.net/)
- [GLEW](https://glew.sourceforge.net/)
- [GLFW](https://www.glfw.org/)

- [OpenGL Mathematics (GLM)](https://github.com/g-truc/glm)
- [GLFW](https://www.glfw.org/)
- [GLFW](https://www.glfw.org/)

- [Open-Asset-Importer-Lib (ASSIMP)](https://github.com/assimp/assimp)
- [FreeImage](https://freeimage.sourceforge.io/)
- [SDL](https://www.libsdl.org/)
    
Their individual licenses can be found in **LICENSE.md** file.


## License

**The software is licensed under the [MIT](https://choosealicense.com/licenses/mit/) License:**

Copyright © 2013 Paulius Akulavicius

Permission is hereby granted, free of charge, 
to any person obtaining a copy of this software 
and associated documentation files (the “Software”), 
to deal in the Software without restriction, 
including without limitation the rights to use, 
copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice 
shall be included in all copies or substantial portions 
of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
# [WIP] Technical Report
This is an improved version of the weak, rushed report I turned in alongside [this commit](https://github.com/collebrusco/minecraft/tree/CS-384G-release) for the CS 384G final project at UT.


This project aimed to build an improved, faster Minecraft renderer and engine in C++.         
## Background
Minecraft is a notoriously poor performing game, particularly older Java versions. Sure, some of this is bad Mojang code and Java, but I blame that much less than the simple fact that building an infinite procedural and editable voxel engine is a legitimate challenge to do performantly. How do you represent or render the world?
#### Chunking
Minecraft famously divides the world into chunks. This will be the smallest unit of world generated, loaded or unloaded at once. By maintaining a growing set of these chunks, we can represent an infinite procedural modifyable world in memory. A mesh can also then be built for each chunk, updated if it changes, and the world can be drawn by making a draw call per chunk, stitching them all together.    
#### a single 2D minecraft chunk
![chunkpic](https://media.forgecdn.net/attachments/421/591/2022-01-09_19.png)           
#### Chunking Strategies
There are a few ways to chunk the world. Minecraft uses 2D chunks of size 16x16. 2D here means that the world is divided along 2 dimensions: a chunk spans in this case 16x16 blocks laterally, but contains everything from bedrock to the height limit. Visualize this as a large vertical chunk of the world.               
#### 
The world could also use 3D chunks, where the world is divided along every dimension, and every chunk is an NxNxN cube of blocks. This lets you have an infinite world height and depth, which is a neat feature but complicates the engine.        
#### Rendering Chunks
With the world divided into nice bite sized pieces, the renderer can build a mesh for each piece and render them one at a time. This is what minecraft does. This does mean the chunk mesh has to be updated each time a block changes.               
You'll know the following if you've ever used an x-ray cheat, but minecraft builds the mesh for a given chunk out of only the block faces that are exposed and visible to the air. If it didn't do this, the renderer would have to store and process millions of extra polygons that all end up invisible.


## Our Implementation
### Chunking Strategy
We use 2D chunks to keep the engine a little simpler. Somewhat arbitrarily, the size is the same as in minecraft as well: 16x16.    
### Chunk Rendering



The engine is built using my graphics library that I’ve been developing for a couple of years (flgl). It’s built on OpenGL + GLFW. I’ll be the first to admit that I spent too much time engineering the engine such that I ran out of time to utilize it to build the game. However, the foundation is strong and the renderer is very smooth.
As far as I know, Minecraft does not use instanced rendering. Each chunk mesh is unique, so they can’t. We instance faces. There is one face VAO on the GPU, and the only thing re-buffered when a chunk updates is the instance buffer. It turns out that we can represent a face of a chunk with one 32-bit integer.
Chunks are 16 x 256 x 16. There are 6 possible orientations, and 4096 textures. So each face can be fully specified by:
● 4bitsforX
● 4bitsforZ
● 8bitsforY
● 3 bits for orientation
● 12 bits for texture
This is 31 bits total. Thus, we can rebuffer chunks at a cost of 4 bytes per face! If we were rebuffering the whole mesh, each face would require 4 verts each with 8 floats (pos, uv, norm) plus 6 element indices. This is 152 bytes total. This is a 38x bandwidth improvement!



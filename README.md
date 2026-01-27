This Readme is as much a work in progress as the Engine itself
# Pixie Engine
Pixie Engie started as a project for my Engine and Tools specialization course.
The goal is to have a simple engine that will support the creation of a basic spline game (such as classic Star Fox) by the end of February 2026.

Right now Pixie Engine is only compatible for Windows platforms that support openGL 4.5+.
this repository has some external library dependancies that are not in the uploaded includes.
TODO: write clear instalation instructions and move as many include files as possible into the repository.

At the moment the Solution is set up so that there is a core Engine project, and then a SandBox template that holds editor code. I believe I am going to end up transfering to a three project set up where the game project is separated out from the editor and engine, but can use them as libraries.   

Pixie Engine is currently single threaded, though I hope to transition it to having audio, physics / object updating, and rendering on different threads.

## About the Editor
- Can only have one scene open at a time
- play and stop buttons in scene viewport are planned to allow simulating gameplay in editor.

Keyboard commands:
* ctrl + s         saves the current scene in place or opens the save as dialogue if the scene has never been saved before.
* ctrl + shift + s opens the save as dialogue to save the current scene
* ctrl + o         opens the open file dialogue and will allow you to load previously saved scenes (file ending is .pixie)
* tab sets the currently possessed camera into fly mode. which allows you to navigate the scene with WASD, the mouse for rotation, left ctrl moves you in your down vector, space bar moves you in your up vector.
* ctrl + f         moves the currently possessed camera to an offset location from the currently selected object and turns the camera to look at that object.

### About Scenes
The secondary toolbar has buttons to toggle lit or unlit shading, wireframe mode, and a readout of which camera the viewport is controlling.
#### Active Camera settings
* Speed: higher values makes the camera move faster in fly mode
* Look Sensitivity: higher values make smaller mouse movements rotate further
* Zoom: kind of broken right now, closer to 0 zooms in, further from 0 zooms out. fixing this is on my todo list
* reset zoom button resets the zoom to 1.0. On perspective cameras the visual effect of zoom can still be achieved at zoom level 1 by manipulating the FOV (field of view). Currently the editor camera settings are not fully exposed, and its FOV is set to 45 degrees.
- Unrotated cameras look into the -Z axis with +X to the right and +Y is up.
- there is currently no click to select objects in the viewport, but that is pretty high on the todo list.

#### The Hiearchy panel lists the objects in the scene 
- clicking and dragging an object in the list 
 * dropping it on empty space makes the object have no parent objects.
 * dropping it on another object sets it as a child of that object
- clicking on a name in the list selects it, and a selected object can have its details and components edited in the Details View panel. 

#### Loaded Assets panel
allows viewing of assets that have been loaded using the AssetLoader system. 
- Texture / Image resources can have their mipmap levels and filtering set here.
- shaders can be hot reloaded here
- the raw text of text files can be viewed here

#### Console panel and Logging
- Console panel is a viewer of all text sent through the Logger wrapper class.
- you can hide or show log levels as you wish, clear the history, and search the log entries for keywords.

Logging is set up through a third party system called spdlog.
  * I have set up two loggers which are used through the static template functions: Logger::Core and Logger::Game.
  * Logger::Core is intended for use in the Pixie engine and editor only files, and Logger::Game is intended for use in game specific code. by having these commands go to separate loggers, it allows the in editor viewer (Console panel) to filter out game logs from engine logs

These logs are also dumped to a rotating set of 3 files in SandBox/logs/. each file is permitted to reach 5mb in size before starting a new file.

## Other Engine Systems
#### Physics System
- Initial physics system is going to be a simple collision detection system
- If there is time more in depth physics will be added.

#### Audio System
- Audio is considered a nice to have at this time. More details will be added when time to implement audio is made.
## Third Party Systems Utilized
glfw, glad, spdlog, imgui, imguizmo, assimp, stb, entt

##   
*Pixie Engine is named for the late Pixie, my cat of the last 7 years. She made it to around 18 years old, and brought so much joy and warmth to our lives.*

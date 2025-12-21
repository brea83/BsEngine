#pragma once
// For use by Pixie applications

#include "Source/Core.h"
#include "Source/EngineContext.h"
#include "Source/GlfwWrapper.h"


#include "Layers/Layer.h"
#include "Layers/ImGuiLayer.h"
// ToDo: make and add debug / logger


#include "Input/Input.h"
#include "Events/KeyCodes.h"
#include "Events/MouseCodes.h"

#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/GameObject.h"
#include "Scene/Components/Component.h"
#include "Scene/Components/CameraComponent.h"
#include "Scene/Components/CameraController.h"
#include "Scene/Components/MeshComponent.h"

// ---Renderer------------------------
#include "Graphics/Renderers/Renderer.h"
#include "Graphics/Renderers/ForwardRenderer.h"

#include "Graphics/Shaders/Shader.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/Texture.h"
#include "Graphics/Primitives/Mesh.h"

#include "Graphics/Camera.h"
// -----------------------------------
// // ToDo: Threading
// ToDo: Physics and Collisions
// ToDo: Splines 
// ToDo: Audio System
// ToDo: Animation system
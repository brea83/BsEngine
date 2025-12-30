#include "BsPrecompileHeader.h"
#include "ForwardRenderPass.h"
#include "Graphics/Shaders/Shader.h"
#include "Scene/Components/MeshComponent.h"
#include "Scene/Scene.h"
#include "Scene/Components/Component.h"
#include "Scene/Components/Transform.h"
#include "Scene/Components/CameraComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Texture.h"
#include "Resources/AssetLoader.h"
#include "EngineContext.h"
#include "Scene/GameObject.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Pixie
{
	ForwardRenderPass::ForwardRenderPass()
	{
		m_FallbackMaterial.BaseMapPath = "../Assets/Textures/teal.png";
		m_FallbackMaterial.BaseMap = AssetLoader::LoadTexture(m_FallbackMaterial.BaseMapPath);

		m_Shader = AssetLoader::LoadShader("../Assets/Shaders/VertexShader.glsl", "../Assets/Shaders/FragmentMultiLightLecture.glsl");
		
	}

	ForwardRenderPass::~ForwardRenderPass()
	{ }

	void ForwardRenderPass::Execute(Scene& sceneToRender)
	{
		m_Shader->Use();

		GameObject cameraEntity = sceneToRender.GetActiveCameraGameObject();
		if (!cameraEntity)
		{
			// no valid camera to render
			m_Shader->EndUse();
			return;
		}
		// set up rendering camera info
		TransformComponent& transform = cameraEntity.GetComponent<TransformComponent>();
		glm::vec3 camPosition = transform.GetPosition();

		glm::mat4 viewMatrix{1.0f};
		Camera* mainCam = sceneToRender.GetActiveCamera(viewMatrix);
		if (mainCam == nullptr)
		{
			std::cout << "No Camera in the scene is set to active" << std::endl;
			return;
		}
		glm::mat4 projectionMatrix = mainCam->ProjectionMatrix();

		m_Shader->SetUniformMat4("view", viewMatrix);
		m_Shader->SetUniformMat4("projection", projectionMatrix);
		m_Shader->SetUniformVec3("cameraPosition", camPosition);

		// get scene registry for lighting and renderables
		entt::registry& registry = sceneToRender.GetRegistry();

		// set up light data
		SendLightsToShader(registry);

		// render meshes
		auto group = registry.group<MeshComponent>(entt::get<TransformComponent>);

		for (auto entity : group)
		{
			TransformComponent& transform = group.get<TransformComponent>(entity);
			m_Shader->SetUniformMat4("transform", transform.GetObjectToWorldMatrix());

			MeshComponent& mesh = group.get<MeshComponent>(entity);

			if (!mesh.HasTexture())
			{
				m_FallbackMaterial.BaseMap->Bind(0);
				m_Shader->SetUniformInt("Texture1", 0);
			}

			mesh.Render(*m_Shader);
		}

		m_Shader->EndUse();
	}
	void ForwardRenderPass::SendLightsToShader(entt::registry& registry)
	{
		auto group = registry.group<LightComponent>(entt::get<TransformComponent>);

		if (!group)
		{
			std::cout << "No light components found" << std::endl;
			return;
		}

		m_Shader->SetUniformBool("BUseLights", true);

		int lightTypes[MAX_LIGHTS];
		float lightColors[MAX_LIGHTS * 3];
		float lightPositions[MAX_LIGHTS * 3];
		float lightDirections[MAX_LIGHTS * 3];
		float lightAttenuations[MAX_LIGHTS * 3];
		float innerRadiusCos[MAX_LIGHTS];
		float outerRadiusCos[MAX_LIGHTS];

		int activeLights = 0;
		for (auto entity : group)
		{
			LightComponent& light = group.get<LightComponent>(entity);
			TransformComponent& lightTransform = group.get<TransformComponent>(entity);
			if (!light.Enabled) continue;

			lightTypes[activeLights] = light.Type;

			lightColors[activeLights * 4 + 0] = light.Color.r;
			lightColors[activeLights * 4 + 1] = light.Color.g;
			lightColors[activeLights * 4 + 2] = light.Color.b;

			lightPositions[activeLights * 4 + 0] = lightTransform.GetPosition().x;
			lightPositions[activeLights * 4 + 1] = lightTransform.GetPosition().y;
			lightPositions[activeLights * 4 + 2] = lightTransform.GetPosition().z;

			lightDirections[activeLights * 4 + 0] = light.Direction.x;
			lightDirections[activeLights * 4 + 1] = light.Direction.y;
			lightDirections[activeLights * 4 + 2] = light.Direction.z;

			lightAttenuations[activeLights * 4 + 0] = light.Attenuation.x;
			lightAttenuations[activeLights * 4 + 1] = light.Attenuation.y;
			lightAttenuations[activeLights * 4 + 2] = light.Attenuation.z;

			innerRadiusCos[activeLights] = glm::cos(glm::radians(light.InnerRadius));
			outerRadiusCos[activeLights] = glm::cos(glm::radians(light.OuterRadius));

			activeLights++;
		}
		
		//glUniform1i(glGetUniformLocation(m_Shader->ShaderProgram, "activeLightsCount"), activeLights);
		glUniform1i(glGetUniformLocation(m_Shader->ShaderProgram, "activeLights"), activeLights);
		glUniform3fv(glGetUniformLocation(m_Shader->ShaderProgram, "lightPosition"), activeLights, lightPositions);
		glUniform3fv(glGetUniformLocation(m_Shader->ShaderProgram, "lightDirection"), activeLights, lightDirections);
		glUniform3fv(glGetUniformLocation(m_Shader->ShaderProgram, "lightColor"), activeLights, lightColors);
		glUniform3fv(glGetUniformLocation(m_Shader->ShaderProgram, "lightAttenuation"), activeLights, lightAttenuations);
		glUniform1fv(glGetUniformLocation(m_Shader->ShaderProgram, "innerRadius"), activeLights, innerRadiusCos);
		glUniform1fv(glGetUniformLocation(m_Shader->ShaderProgram, "outerRadius"), activeLights, outerRadiusCos);
		glUniform1iv(glGetUniformLocation(m_Shader->ShaderProgram, "lightTypes"), activeLights, lightTypes);
	}
}
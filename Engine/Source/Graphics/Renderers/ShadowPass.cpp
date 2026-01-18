#include "BsPrecompileHeader.h"
#include "ShadowPass.h"
#include "EngineContext.h"
#include "Resources/AssetLoader.h"
#include "ForwardRenderer.h"
#include "Scene/GameObject.h"
#include "Scene/Components/Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//ToDo: implement shadow map cascades

namespace Pixie
{
	ShadowPass::ShadowPass(int cascadesCount)
		: m_CascadesCount(cascadesCount)
	{
		FrameBufferTextureSpecification depthTextureSpec = FrameBufferTextureSpecification(FrameBufferTextureFormat::Depth24, cascadesCount + 1);
		//FrameBufferTextureSpecification colorTextureSpec = FrameBufferTextureSpecification(FrameBufferTextureFormat::RGBA8);
		FrameBufferSpecification frameBufferData;
		frameBufferData.Width = 1024;
		frameBufferData.Height = 1024;
		frameBufferData.Attachments.Attachments.push_back(depthTextureSpec);
		//frameBufferData.Attachments.Attachments.push_back(colorTextureSpec);

		m_FrameBuffer = FrameBuffer::Create(frameBufferData);
		glEnable(GL_DEPTH_TEST);

		m_Shader = AssetLoader::LoadShader("../Assets/Shaders/CascadesDepthVertex.glsl", "../Assets/Shaders/SimpleDepthFragment.glsl", "../Assets/Shaders/CascadesDepthGeometry.glsl");
	}

	ShadowPass::~ShadowPass()
	{}

	void ShadowPass::Execute(Scene& sceneToRender, uint32_t prevPassDepthID, uint32_t prevPassColorID)
	{
		// start rendering
		m_Shader->Use();
		m_FrameBuffer->Bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_FrameBuffer->GetFirstColorAttachmentID());
		// render meshes
		entt::registry& registry = sceneToRender.GetRegistry();
		auto group = registry.group<MeshComponent>(entt::get<TransformComponent>);

		glCullFace(GL_FRONT);
		for (auto entity : group)
		{
			TransformComponent& transform = group.get<TransformComponent>(entity);
			m_Shader->SetUniformMat4("transform", transform.GetObjectToWorldMatrix());

			MeshComponent& mesh = group.get<MeshComponent>(entity);

			mesh.RenderWithoutMaterial(*m_Shader);
		}
		glCullFace(GL_BACK);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_Shader->EndUse();
	}

	uint32_t ShadowPass::GetFrameBufferID()
	{
		return m_FrameBuffer->GetFrameBufferID();
	}

	uint32_t ShadowPass::GetColorAttatchmentID()
	{
		return m_FrameBuffer->GetFirstColorAttachmentID();
	}
	uint32_t ShadowPass::GetDepthAttatchmentID()
	{
		return m_FrameBuffer->GetDepthAttatchmentID();
	}
}
#pragma once

#include "Layer.h"
#include <glm/glm.hpp>
#include "Events/Event.h"

namespace Pixie
{
	//may need key, mouse, and application events
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override {}
		virtual void OnImGuiRender() override {}
		virtual void OnEvent(Event& event) override {}

		void Begin();
		void End();

		glm::vec2 GetViewportSize() const { return m_ViewportPanelSize; }
	protected:
		float m_Time{ 0.0f };

		glm::vec2 m_ViewportPanelSize{ 0.0f };
		glm::vec2 m_ViewportBounds[2];
		bool m_bViewportResized{ true };
	};
}
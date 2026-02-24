#include "CombatComponents.h"
#include "ImGui/ImGuiPanel.h"
#include "Scene/GameObject.h"

// Attack Component --------------
void Attack::Draw(Pixie::GameObject& selected)
{
	if (!selected.HasCompoenent<Attack>())
		return;
	Attack& component = selected.GetComponent<Attack>();
	ImGui::Text("Damage");
	ImGui::SameLine();
	ImGui::DragInt("##maxHealth", &component.Damage, 1.0f);
}

void Attack::Remove(Pixie::GameObject& selected)
{
	selected.RemoveComponent<Attack>();
}

// Point Scoring component ------------

void ScoresPoints::Draw(Pixie::GameObject& selected)
{
	if (!selected.HasCompoenent<ScoresPoints>())
		return;
	ScoresPoints& component = selected.GetComponent<ScoresPoints>();
	ImGui::Text("Damage");
	ImGui::SameLine();
	ImGui::DragInt("##maxHealth", &component.Value, 1.0f);
}

void ScoresPoints::Remove(Pixie::GameObject& selected)
{
	selected.RemoveComponent<ScoresPoints>();
}

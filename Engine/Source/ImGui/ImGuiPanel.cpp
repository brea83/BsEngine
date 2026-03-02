#include "BsPrecompileHeader.h"
#include "ImGuiPanel.h"
#include "PlatformUtils.h"
#include "Resources/AssetLoader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Pixie
{
	bool ImGuiPanel::DrawVec3Control(const std::string& label, glm::vec3& values, SliderParams params, float columnWidth)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		bool bValueChanged = false;

		if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Resizable))
		{
			float fontSize = ImGui::GetFontSize();
			ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, fontSize * columnWidth);
			ImGui::TableSetupColumn("Values"/*, ImGuiTableColumnFlags_WidthFixed,*/);
			ImGui::TableNextRow();
			// the label
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(label.c_str());

			// the values
			ImGui::TableSetColumnIndex(1);
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25f);// fontSize * 4.0f);

			ImGui::PushStyleVarX(ImGuiStyleVar_ItemSpacing, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.25f, 0.25f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.0f, 0.05f, 1.0f });
			if (ImGui::Button("X"))
			{
				values.x = params.ResetValue;
				bValueChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("##floatX", &values.x, params.Speed, params.Min, params.Max, params.Format.c_str(), params.Flags))
			{
				bValueChanged = true;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();

			ImGui::SameLine();
			ImGui::PushStyleVarX(ImGuiStyleVar_ItemSpacing, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.3f, 0.55f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.9f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.6f, 0.05f, 1.0f });
			if (ImGui::Button("Y"))
			{
				values.y = params.ResetValue;
				bValueChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("##floatY", &values.y, params.Speed, params.Min, params.Max, params.Format.c_str(), params.Flags))
			{
				bValueChanged = true;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();

			ImGui::SameLine();
			ImGui::PushStyleVarX(ImGuiStyleVar_ItemSpacing, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.25f, 0.25f, 0.7f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 1.0f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.0f, 0.6f, 1.0f });
			if (ImGui::Button("Z"))
			{
				values.z = params.ResetValue;
				bValueChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("##floatZ", &values.z, params.Speed, params.Min, params.Max, params.Format.c_str(), params.Flags))
			{
				bValueChanged = true;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();

			ImGui::PopItemWidth();

			ImGui::EndTable();
		}

		ImGui::PopStyleVar();
		return bValueChanged;
	}

	bool ImGuiPanel::DrawVec2Control(const std::string& label, glm::vec2& values, SliderParams params, float columnWidth)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		bool bValueChanged = false;

		if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Resizable))
		{
			float fontSize = ImGui::GetFontSize();
			ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, columnWidth);
			ImGui::TableSetupColumn("Values"/*, ImGuiTableColumnFlags_WidthFixed,*/);
			ImGui::TableNextRow();
			// the label
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(label.c_str());

			// the values
			ImGui::TableSetColumnIndex(1);
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25f);// fontSize * 4.0f);

			ImGui::PushStyleVarX(ImGuiStyleVar_ItemSpacing, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.25f, 0.25f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.0f, 0.05f, 1.0f });
			if (ImGui::Button("X"))
			{
				values.x = params.ResetValue;
				bValueChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("##floatX", &values.x, params.Speed, params.Min, params.Max, params.Format.c_str(), params.Flags))
			{
				bValueChanged = true;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();

			ImGui::SameLine();
			ImGui::PushStyleVarX(ImGuiStyleVar_ItemSpacing, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.3f, 0.55f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.9f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.6f, 0.05f, 1.0f });
			if (ImGui::Button("Y"))
			{
				values.y = params.ResetValue;
				bValueChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("##floatY", &values.y, params.Speed, params.Min, params.Max, params.Format.c_str(), params.Flags))
			{
				bValueChanged = true;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();

			ImGui::PopItemWidth();

			ImGui::EndTable();
		}

		ImGui::PopStyleVar();
		return bValueChanged;
	}

	bool ImGuiPanel::DrawFloatControl(const std::string& label, float& value, SliderParams params, float columnWidth)
	{
		bool bValueChanged = false;

		if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Resizable))
		{
			float fontSize = ImGui::GetFontSize();
			ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, fontSize * columnWidth);
			ImGui::TableSetupColumn("Values"/*, ImGuiTableColumnFlags_WidthFixed,*/);
			ImGui::TableNextRow();
			// the label
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(label.c_str());

			// the values
			ImGui::TableSetColumnIndex(1);
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25f);

			ImGui::PushStyleVarX(ImGuiStyleVar_ItemSpacing, 0.0f);
			if (ImGui::DragFloat("##float", &value, params.Speed, params.Min, params.Max, params.Format.c_str(), params.Flags))
			{
				bValueChanged = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				value = params.ResetValue;
				bValueChanged = true;
			}

			ImGui::PopStyleVar();


			ImGui::PopItemWidth();

			ImGui::EndTable();
		}

		return bValueChanged;
	}

	bool ImGuiPanel::DrawStringProperty(const std::string& label, std::string& value, std::string& editingValue, float columnWidth)
	{
		bool bValueSubmitted = false;
		ImGui::PushID((label + value).c_str());
		ImGui::BeginGroup();

		if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Resizable/* | ImGuiTableFlags_RowBg*/))
		{
			float fontSize = ImGui::GetFontSize();
			ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, fontSize * columnWidth);
			ImGui::TableSetupColumn("Values", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			// the label
			ImGui::TableSetColumnIndex(0);
			ImGui::PushItemWidth(fontSize * columnWidth);
			ImGui::Text(label.c_str());
			ImGui::PopItemWidth();

			// the values
			ImGui::TableSetColumnIndex(1);
			//ImGui::PushItemWidth(fontSize * value.size());
			// do stuff

			ImGui::InputTextWithHint("##EditableString", value.c_str(), &editingValue);
			if (ImGui::IsItemDeactivatedAfterEdit())
			{
				Logger::Core(LOG_DEBUG, "DetailsViewPanel::DrawSTringProperty() pressed enter on text input");
				if (value != editingValue)
				{
					if (editingValue != "" && editingValue != " ")
					{
						value = editingValue;
						bValueSubmitted = true;
					}
				}

			}

			if (!ImGui::IsItemActivated())
			{
				editingValue = value;
			}

			//ImGui::PopItemWidth();

			ImGui::EndTable();
		}

		ImGui::EndGroup(); // end of overal encapsulating group for the whole property
		ImGui::PopID();
		return bValueSubmitted;
	}

	bool ImGuiPanel::FileProperty(const std::string& label, std::string& value, const char* filter, std::string deleteButtonText, float propertyWidth)
	{
		ImGui::PushID((label + value).c_str());
		ImGui::BeginGroup();

		std::filesystem::path fileName = value;
		if (!value.empty() && fileName.has_filename())
		{
			fileName = fileName.filename();
		}
		
		std::string buttonText = "...";
		float deleteWidth = deleteButtonText.empty() ? 0.0f : GetTextSizePadded(deleteButtonText.c_str()).x + ImGui::GetStyle().ItemSpacing.x;
		float buttonWidth = GetTextSizePadded(buttonText.c_str()).x + ImGui::GetStyle().ItemSpacing.x;
		float tableWidth = propertyWidth == 0.0f ? ImGui::GetContentRegionAvail().x - (buttonWidth + deleteWidth) : propertyWidth - (buttonWidth + deleteWidth);
		float columnWidth = tableWidth * 0.25;
		if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Resizable, ImVec2(tableWidth, 0.0f)))
		{
			//float fontSize = ImGui::GetFontSize();
			ImGui::TableSetupColumn("Labels" , ImGuiTableColumnFlags_WidthFixed, columnWidth);
			ImGui::TableSetupColumn("Values");// , ImGuiTableColumnFlags_WidthStretch);
			//ImGui::TableSetupColumn("EditButton", ImGuiTableColumnFlags_WidthStretch/*, -FLT_MIN*/);

			ImGui::TableNextRow();
			// the label
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(label.c_str());

			// the values
			ImGui::TableSetColumnIndex(1);
			ImGui::Text((fileName.string().c_str()));

			ImGui::EndTable();

			// the button to turn  the value field into an edit field
			ImGui::SameLine();
			//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (buttonWidth + deleteWidth));

			ImGui::PushID(label.c_str());
			if (ImGui::Button(buttonText.c_str()))
			{
				//bValueChanged = (value != editingValue);
				std::string filePath = FileDialogs::OpenFile(filter);

				if (!filePath.empty())
				{
					value = filePath;
					ImGui::PopID();
					ImGui::EndGroup();// outermost group for whole file property drawer thingy
					ImGui::PopID();
					return true;
				}
			}
			ImGui::PopID();
			//ImGui::PopItemWidth();
		}

		ImGui::EndGroup();// outermost group for whole file property drawer thingy
		ImGui::PopID();

		return false;
	}
	ImVec2 ImGuiPanel::GetTextSizePadded(const std::string& label)
	{
		return ImVec2 { ImGui::CalcTextSize(label.c_str()).x + (ImGui::GetStyle().FramePadding.x * 2.0f),
			ImGui::CalcTextSize(label.c_str()).y + (ImGui::GetStyle().FramePadding.y * 2.0f) };
	}

	void ImGuiPanel::CenteredText(const std::string& text)
	{
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(text.c_str());
	}

	bool ImGuiPanel::CheckBox(const std::string& label, bool* value, bool bDiamondStyle)
	{
		if (bDiamondStyle)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
		}
		ImGui::Text(label.c_str());
		ImGui::SameLine();
		if (ImGui::Checkbox(("##" + label + "checkbox").c_str(), value))
		{
			if (bDiamondStyle)
			{
				ImGui::PopStyleVar();
			}
			return true;
		}

		if(bDiamondStyle)
		{
			ImGui::PopStyleVar();
		}
		return false;
	}
}
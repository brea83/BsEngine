#include "BsPrecompileHeader.h"
#include "ImGuiPanel.h"
#include "PlatformUtils.h"
#include "Resources/AssetLoader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Pixie
{
	bool ImGuiPanel::DrawVec3Control(const std::string& label, glm::vec3& values, SliderParams params, float columnWidth)
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

		return bValueChanged;
	}

	bool ImGuiPanel::DrawVec2Control(const std::string& label, glm::vec2& values, SliderParams params, float columnWidth)
	{
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

		return bValueSubmitted;
	}

	bool ImGuiPanel::FileProperty(const std::string& label, std::string& value, const char* filter, float columnWidth)
	{
		std::filesystem::path fileName = value;
		if (!value.empty() && fileName.has_filename())
		{
			fileName = fileName.filename();
		}


		if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Resizable/* | ImGuiTableFlags_RowBg*/))
		{
			float fontSize = ImGui::GetFontSize();
			ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, fontSize * columnWidth);
			ImGui::TableSetupColumn("Values", ImGuiTableColumnFlags_WidthStretch);
			//ImGui::TableSetupColumn("EditButton", ImGuiTableColumnFlags_WidthStretch/*, -FLT_MIN*/);

			ImGui::TableNextRow();
			// the label
			ImGui::TableSetColumnIndex(0);

			ImGui::PushItemWidth(fontSize * columnWidth);
			ImGui::Text(label.c_str());
			ImGui::PopItemWidth();

			// the values
			ImGui::TableSetColumnIndex(1);
			ImGui::PushItemWidth(fontSize * value.size());
			// do stuff

			ImGui::Text((fileName.string().c_str()));

			ImGui::PopItemWidth();

			// the button to turn  the value field into an edit field

			ImGui::EndTable();

			ImGui::SameLine();
			std::string buttonText = "...";
			std::string deleteText = "X";
			float deleteWidth = ImGui::CalcTextSize(deleteText.c_str()).x + (ImGui::GetStyle().FramePadding.x * 2.f);
			float buttonWidth = ImGui::CalcTextSize(buttonText.c_str()).x + (ImGui::GetStyle().FramePadding.x * 2.f);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (buttonWidth + deleteWidth));

			ImGui::PushID(label.c_str());
			if (ImGui::Button(buttonText.c_str()))
			{
				//bValueChanged = (value != editingValue);
				std::string filePath = FileDialogs::OpenFile(filter);

				if (!filePath.empty())
				{
					value = filePath;
					ImGui::PopID();
					return true;
				}

			}
			ImGui::PopID();
			//ImGui::PopItemWidth();
		}

		return false;
	}
}
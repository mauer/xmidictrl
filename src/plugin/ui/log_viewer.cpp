//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
//
//   XMidiCtrl is free software: you can redistribute it and/or modify it under the terms of the
//   GNU Affero General Public License as published by the Free Software Foundation, either version 3
//   of the License, or (at your option) any later version.
//
//   XMidiCtrl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
//   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Affero General Public License for more details.
//
//   You should have received a copy of the GNU Affero General Public License along with XMidiCtrl.
//   If not, see <https://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------------------------------------------------

#include "log_viewer.h"

// Font Awesome
#include <IconsFontAwesome6.h>

// fmt
#include "fmt/format.h"

// XMidiCtrl
#include "plugin.h"
#include "types.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
log_viewer::log_viewer(text_logger& in_text_log, environment& in_env)
	: imgui_window(in_text_log, in_env, 1400, 700)
{
	set_title(std::string(XMIDICTRL_NAME) + " - Log Viewer");

	m_log_msg_flags = ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort;
	if (env().settings().default_text_sort() == sort_mode::ascending)
		m_log_msg_flags = m_log_msg_flags | ImGuiTableColumnFlags_PreferSortAscending;
	else
		m_log_msg_flags = m_log_msg_flags | ImGuiTableColumnFlags_PreferSortDescending;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create widgets
 */
void log_viewer::create_widgets()
{
	ImGui::Text("Debug Mode:");
	ImGui::SameLine(150);

	if (env().settings().log_midi())
		ImGui::TextColored(value_color(), "Enabled (%i)", static_cast<int>(log().count()));
	else
		ImGui::TextColored(value_color(), "Disabled");

	ImGui::SameLine(ImGui::GetWindowWidth() - 420);

	if (ImGui::Button(UI_SPACER_2 ICON_FA_ROTATE_RIGHT UI_SPACER_2 "Reload Aircraft Profile" UI_SPACER_2))
		plugin::instance().load_profile();

	ImGui::SameLine(ImGui::GetWindowWidth() - 190);

	if (ImGui::Button(UI_SPACER_2 ICON_FA_TRASH_CAN UI_SPACER_2 "Clear Messages" UI_SPACER_2))
		log().clear();

	ImGui::NewLine();
	ImGui::TextColored(title_color(), "MESSAGES");

	ImGui::SameLine(150);

	if (ImGui::Button(UI_SPACER_2 ICON_FA_ARROW_LEFT UI_SPACER_2)) {
		if (m_page > 0)
			m_page--;
	}

	ImGui::SameLine();

	size_t start = 0;
	size_t end = 0;

	if (m_log_sort_mode == sort_mode::ascending) {
		start = m_page * c_no_msg_page;
		end = (m_page + 1) * c_no_msg_page;

		if (end > log().count())
			end = log().count();
	} else if (log().count() > 0) {
		start = (m_page + 1) * c_no_msg_page;
		end = m_page * c_no_msg_page;

		if (start > log().count() - 1)
			start = log().count() - 1;
	}

	size_t no_pages = (log().count() + c_no_msg_page - 1) / c_no_msg_page;
	ImGui::Text("%s", fmt::format("{} / {}", m_page + 1, no_pages).c_str());

	ImGui::SameLine();

	if (ImGui::Button(UI_SPACER_2 ICON_FA_ARROW_RIGHT UI_SPACER_2)) {
		if ((m_page + 1) * c_no_msg_page < log().count())
			m_page++;
	}

	ImGui::Separator();

	ImGui::BeginChild("LOG_TABLE");

	if (ImGui::BeginTable("tableLogMessages",
						  4,
						  ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable
							  | ImGuiTableFlags_ScrollY)) {
		ImGui::TableSetupColumn("No", ImGuiTableColumnFlags_WidthFixed, 25);
		ImGui::TableSetupColumn("Date/Time", m_log_msg_flags, 200);
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 120);
		ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs()) {
			if (sort_specs->SpecsDirty && sort_specs->SpecsCount > 0) {
				auto spec = sort_specs->Specs[0];
				if (spec.ColumnIndex == 1 && spec.SortDirection == ImGuiSortDirection_Ascending)
					m_log_sort_mode = sort_mode::ascending;
				else
					m_log_sort_mode = sort_mode::descending;
			}
		}

		if (m_log_sort_mode == sort_mode::ascending) {
			for (size_t i = start; i < end; i++) add_log_row(i, log().message(i));
		} else if (log().count() > 0) {
			for (size_t i = start; i > end; i--) add_log_row(i, log().message(i));
		}
	}
	ImGui::EndTable();
	ImGui::EndChild();
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Add a log message to the table
 */
void log_viewer::add_log_row(size_t in_no, text_log_msg* in_msg)
{
	ImGui::TableNextRow();

	ImGui::TableNextColumn();
	ImGui::TextUnformatted(fmt::format("{}", in_no).c_str());

	ImGui::TableNextColumn();
	ImGui::TextUnformatted(in_msg->time.c_str());

	ImGui::TableNextColumn();
	ImGui::TextUnformatted(in_msg->get_log_level_text().c_str());

	ImGui::TableNextColumn();
	ImGui::TextUnformatted(in_msg->text.c_str());
}

} // Namespace xmidictrl
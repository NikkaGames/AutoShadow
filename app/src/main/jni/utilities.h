//
// Created by nikag on 11/10/2022.
//

using namespace ImGui;

std::string string_to_hex(const std::string& input) {
    static const char hex_digits[] = "0123456789ABCDEF";
    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input) {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

int hex_value(unsigned char hex_digit) {
    static const signed char hex_values[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };
    int value = hex_values[hex_digit];
    if (value == -1) throw std::invalid_argument(std::string(OBFUSCATE("invalid hex digit")));
    return value;
}

void ColorPicker(const char* name, ImVec4 &color) {
    ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_AlphaPreview;
    static ImVec4 backup_color;

    bool open_popup = ImGui::ColorButton((std::string(name) + std::string(OBFUSCATE("##3b"))).c_str(), color, misc_flags);
    //open_popup |= ImGui::Button("Palette");
    if (open_popup) {
        ImGui::OpenPopup(name);
        backup_color = color;
    }
    if (ImGui::BeginPopup(name)) {
        ImGui::Text(OBFUSCATE("Select Color"));
        ImGui::Separator();
        ImGui::ColorPicker4(OBFUSCATE("##picker"), (float*)&color, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
        ImGui::SameLine();

        ImGui::BeginGroup(); // Lock X position
        ImGui::Text(OBFUSCATE("Current"));
        ImGui::ColorButton(OBFUSCATE("##current"), color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
        ImGui::Text(OBFUSCATE("Previous"));
        if (ImGui::ColorButton(OBFUSCATE("##previous"), backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
            color = backup_color;
        ImGui::EndGroup();
        ImGui::EndPopup();
    }
}

void ArosiumStyle()
{
    ImGui::StyleColorsDark();

    auto& Style = ImGui::GetStyle();
    Style.WindowTitleAlign = ImVec2(0.500f, 0.500f);
    Style.WindowMenuButtonPosition = -1;

    Style.Colors[ImGuiCol_WindowBg] = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
    Style.Colors[ImGuiCol_PopupBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.940f);
    Style.Colors[ImGuiCol_Border] = ImVec4(1.000f, 1.000f, 1.000f, 0.500f);
    Style.Colors[ImGuiCol_FrameBg] = ImVec4(0.121f, 0.121f, 0.121f, 0.540f);
    Style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.203f, 0.203f, 0.203f, 0.400f);
    Style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.312f, 0.312f, 0.312f, 0.670f);
    Style.Colors[ImGuiCol_TitleBg] = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
    Style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
    Style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
    Style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
    Style.Colors[ImGuiCol_CheckMark] = ImVec4(0.541f, 0.541f, 0.541f, 1.000f);
    Style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.401f, 0.402f, 0.403f, 1.000f);
    Style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.506f, 0.506f, 0.506f, 1.000f);
    Style.Colors[ImGuiCol_Button] = ImVec4(0.000f, 0.000f, 0.000f, 0.400f);
    Style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.121f, 0.121f, 0.121f, 1.000f);
    Style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.293f, 0.294f, 0.294f, 1.000f);
    Style.Colors[ImGuiCol_Header] = ImVec4(0.147f, 0.146f, 0.146f, 0.310f);
    Style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.225f, 0.222f, 0.222f, 0.800f);
    Style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.411f, 0.411f, 0.411f, 1.000f);
    Style.Colors[ImGuiCol_Separator] = ImVec4(0.273f, 0.273f, 0.273f, 0.500f);
    Style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.377f, 0.381f, 0.385f, 0.780f);
    Style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.498f, 0.498f, 0.498f, 1.000f);
    Style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.200f);
    Style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
    Style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.950f);
    Style.Colors[ImGuiCol_Tab] = ImVec4(0.000f, 0.000f, 0.000f, 0.862f);
    Style.Colors[ImGuiCol_TabHovered] = ImVec4(0.195f, 0.195f, 0.195f, 0.800f);
    Style.Colors[ImGuiCol_TabActive] = ImVec4(0.260f, 0.260f, 0.260f, 1.000f);
    Style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.000f, 0.000f, 0.000f, 0.972f);
    Style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.177f, 0.177f, 0.177f, 1.000f);
    Style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.316f, 0.316f, 0.316f, 0.350f);
    Style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.606f, 0.606f, 0.606f, 1.000f);

    ImGuiIO& io = ImGui::GetIO();
}

void ArosiumStyleOld()
{
    ImGui::StyleColorsDark();
    auto& Style = ImGui::GetStyle();
    Style.WindowTitleAlign = ImVec2(0.500f, 0.500f);
    Style.WindowMenuButtonPosition = -1;
    Style.Colors[ImGuiCol_WindowBg] = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
    Style.Colors[ImGuiCol_Border] = ImVec4(0.390f, 0.390f, 0.390f, 0.500f);
    Style.Colors[ImGuiCol_FrameBg] = ImVec4(0.169f, 0.169f, 0.169f, 0.540f);
    Style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.359f, 0.359f, 0.359f, 0.400f);
    Style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.610f, 0.610f, 0.610f, 0.670f);
    Style.Colors[ImGuiCol_TitleBg] = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
    Style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
    Style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
    Style.Colors[ImGuiCol_CheckMark] = ImVec4(0.468f, 0.468f, 0.468f, 1.000f);
    Style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.229f, 0.229f, 0.229f, 1.000f);
    Style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.299f, 0.299f, 0.299f, 1.000f);
    Style.Colors[ImGuiCol_Button] = ImVec4(0.238f, 0.238f, 0.238f, 0.400f);
    Style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.303f, 0.303f, 0.303f, 1.000f);
    Style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.364f, 0.364f, 0.364f, 1.000f);
    Style.Colors[ImGuiCol_Header] = ImVec4(0.368f, 0.368f, 0.368f, 0.310f);
    Style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.545f, 0.545f, 0.545f, 0.800f);
    Style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.615f, 0.615f, 0.615f, 1.000f);
    Style.Colors[ImGuiCol_Separator] = ImVec4(0.277f, 0.277f, 0.277f, 0.500f);
    Style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.450f, 0.448f, 0.448f, 0.780f);
    Style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.649f, 0.649f, 0.649f, 1.000f);
    Style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.610f, 0.610f, 0.610f, 0.200f);
    Style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.730f, 0.732f, 0.736f, 0.670f);
    Style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.950f);
    Style.Colors[ImGuiCol_Tab] = ImVec4(0.143f, 0.143f, 0.143f, 0.862f);
    Style.Colors[ImGuiCol_TabHovered] = ImVec4(0.225f, 0.225f, 0.225f, 0.800f);
    Style.Colors[ImGuiCol_TabActive] = ImVec4(0.412f, 0.414f, 0.416f, 1.000f);
    Style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.130f, 0.130f, 0.130f, 0.972f);
    Style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.121f, 0.121f, 0.121f, 1.000f);
    Style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.636f, 0.636f, 0.636f, 0.350f);
    Style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.645f, 0.645f, 0.645f, 1.000f);
}


void InitStyle()
{
    ArosiumStyle();

    auto& Style = ImGui::GetStyle();
    Style.WindowTitleAlign = ImVec2(0.500f, 0.500f);
    Style.WindowMenuButtonPosition = -1;
    Style.WindowRounding = 24.000f;
    Style.ChildRounding = 8.000f;
    Style.PopupRounding = 8.000f;
    Style.FrameRounding = 8.000f;
    Style.ScrollbarRounding = 3.000f;
    Style.ScrollbarSize = 36.000f;
    Style.GrabRounding = 6.000f;

    Style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.020f, 0.020f, 0.020f, 0.000f);
    Style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.130f, 0.130f, 0.130f, 1.000f);
    Style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.238f, 0.238f, 0.238f, 1.000f);
    Style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.342f, 0.342f, 0.342f, 1.000f);

    Style.WindowPadding = ImVec2(20.000f, 11.000f);
    //Style.WindowRounding = 12.000f;
    //Style.ChildRounding = 5.000f;
    //Style.PopupRounding = 6.000f;
    Style.FramePadding = ImVec2(10.000f, 5.000f);
    //Style.FrameRounding = 8.000f;
    Style.FrameBorderSize = 1.000f;
    Style.ItemInnerSpacing = ImVec2(16.000f, 4.000f);
    Style.CellPadding = ImVec2(9.000f, 11.000f);
    //Style.ScrollbarRounding = 12.000f;
    //Style.GrabRounding = 7.000f;
    Style.LogSliderDeadzone = 6.000f;
    //Style.TabRounding = 8.000f;
    Style.TabBorderSize = 1.000f;
    Style.ButtonTextAlign = ImVec2(0.000f, 0.000f);
    ImGuiIO& io = ImGui::GetIO();
    //Style.WindowRounding = 0;
    Style.Colors[ImGuiCol_Border] = ImVec4(0.229f, 0.229f, 0.229f, 0.500f);

}

void StyleColorsCustom() {
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.11f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.28f, 0.33f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

bool Toggle(const char* label, bool* v) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    float height = ImGui::GetFrameHeight();
    ImVec2 pos = window->DC.CursorPos;

    float width = height * 2.f;
    float radius = height * 0.50f;
    ImVec2 spos = ImVec2(width + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f);
    ImVec2 tpos = ImVec2(pos.x + spos.x, pos.y + spos.y);
    ImRect total_bb(pos, tpos);

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id)) return false;

    float last_active_id_timer = g.LastActiveIdTimer;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) {
        *v = !(*v);
        MarkItemEdited(id);
        g.LastActiveIdTimer = 0.f;
    }

    if (g.LastActiveIdTimer == 0.f && g.LastActiveId == id && !pressed) g.LastActiveIdTimer = last_active_id_timer;

    float t = *v ? 1.0f : 0.0f;

    if (g.LastActiveId == id) {
        float t_anim = ImSaturate( g.LastActiveIdTimer / 0.16f );
        t = *v ? (t_anim) : (1.0f - t_anim);
    }

    ImU32 col_bg = GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    ImVec2 bruh = ImVec2(width, height);
    ImVec2 bruh2 = ImVec2(pos.x + bruh.x, pos.y + bruh.y);
    ImRect frame_bb(pos, bruh2);

    RenderFrame(frame_bb.Min, frame_bb.Max, col_bg, true, height * 0.5f);
    RenderNavHighlight(total_bb, id);

    ImVec2 label_pos = ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y);
    ImGui::RenderText(label_pos, label);
    window->DrawList->AddCircleFilled(ImVec2(pos.x + radius + t * (width - radius * 2.0f), pos.y + radius), radius - 1.5f, ImGui::GetColorU32(ImGuiCol_CheckMark), 36);

    return pressed;
}

std::string hex_to_string(const std::string& input) {
    const auto len = input.length();
    if (len & 1) throw std::invalid_argument(std::string(OBFUSCATE("odd length")));
    std::string output;
    output.reserve(len / 2);
    for (auto it = input.begin(); it != input.end(); ) {
        int hi = hex_value(*it++);
        int lo = hex_value(*it++);
        output.push_back(hi << 4 | lo);
    }
    return output;
}

void DoCrash() {
	int *p = 0;
	*p = 0;
}

typedef unsigned long DWORD;

DWORD findLibrary(const char *library) {
    char filename[0xFF] = {0},
            buffer[1024] = {0};
    FILE *fp = NULL;
    DWORD address = 0;

    sprintf(filename, OBFUSCATE("/proc/self/maps"));

    fp = fopen(filename, OBFUSCATE("rt"));
    if (fp == NULL) {
        perror(OBFUSCATE("fopen"));
        goto done;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (!strstr(buffer, OBFUSCATE("r--p")) && strstr(buffer, library)) {
            address = (DWORD) strtoul(buffer, NULL, 16);
            goto done;
        }
    }

    done:

    if (fp) {
        fclose(fp);
    }

    return address;
}

std::string utf16le_to_utf8(const std::u16string &u16str) {
    if (u16str.empty()) { return std::string(); }
    const char16_t *p = u16str.data();
    std::u16string::size_type len = u16str.length();
    if (p[0] == 0xFEFF) {
        p += 1;
        len -= 1;
    }
    std::string u8str;
    u8str.reserve(len * 3);
    char16_t u16char;
    for (std::u16string::size_type i = 0; i < len; ++i) {
        u16char = p[i];
        if (u16char < 0x0080) {
            u8str.push_back((char) (u16char & 0x00FF));
            continue;
        }
        if (u16char >= 0x0080 && u16char <= 0x07FF) {
            u8str.push_back((char) (((u16char >> 6) & 0x1F) | 0xC0));
            u8str.push_back((char) ((u16char & 0x3F) | 0x80));
            continue;
        }
        if (u16char >= 0xD800 && u16char <= 0xDBFF) {
            uint32_t highSur = u16char;
            uint32_t lowSur = p[++i];
            uint32_t codePoint = highSur - 0xD800;
            codePoint <<= 10;
            codePoint |= lowSur - 0xDC00;
            codePoint += 0x10000;
            u8str.push_back((char) ((codePoint >> 18) | 0xF0));
            u8str.push_back((char) (((codePoint >> 12) & 0x3F) | 0x80));
            u8str.push_back((char) (((codePoint >> 06) & 0x3F) | 0x80));
            u8str.push_back((char) ((codePoint & 0x3F) | 0x80));
            continue;
        }
        {
            u8str.push_back((char) (((u16char >> 12) & 0x0F) | 0xE0));
            u8str.push_back((char) (((u16char >> 6) & 0x3F) | 0x80));
            u8str.push_back((char) ((u16char & 0x3F) | 0x80));
            continue;
        }
    }

    return u8str;
}

uintptr_t str2uptr(const char *c) {
    int base = 16;
    // See if this function catches all possibilities.
    // If it doesn't, the function would have to be amended
    // whenever you add a combination of architecture and
    // compiler that is not yet addressed.
    static_assert(sizeof(uintptr_t) == sizeof(unsigned long)
                  || sizeof(uintptr_t) == sizeof(unsigned long long),
                  "sample handle.");

    // Now choose the correct function ...
    if (sizeof(uintptr_t) == sizeof(unsigned long)) {
        return strtoul(c, nullptr, base);
    }

    // All other options exhausted, sizeof(uintptr_t) == sizeof(unsigned long long))
    return strtoull(c, nullptr, base);
}

typedef struct _monoString {
    void *klass;
    void *monitor;
    int length;
    const char *toChars(){
        std::u16string ss((char16_t *) getChars(), 0, getLength());
        std::string str = utf16le_to_utf8(ss);
        return str.c_str();
    }
    char chars[0];

    char *getChars() {
        return chars;
    }
    int getLength() {
        return length;
    }
} monoString;

std::string gen_random(const int len) {
    std::string id;
    static const char gsfid[] =
            "0123456789"
            "abcdefghijklmnopqrstuvwxyz";
    srand( (unsigned) time(nullptr) * getpid());
    id.reserve(len);
    for (int i = 0; i < len; ++i)
        id += gsfid[rand() % (sizeof(gsfid) - 1)];
    return id;
}

bool contains(std::string in, std::string target) {
    if(strstr(in.c_str(), target.c_str())) {
        return true;
    }
    return false;
}

bool equals(const std::string &s1, const std::string &s2) {
    if (s1.length() != s2.length()) return false;
    int result = 0;
    for (size_t i = 0; i < s1.length(); i++) {
        result |= (s1[i] ^ s2[i]);
    }
    return result == 0;
}

jobject GetActivityContext(JNIEnv* env) {
    jclass uplayer = env->FindClass(OBFUSCATE("com/unity3d/player/UnityPlayer"));
    jfieldID cmeth = env->GetStaticFieldID(uplayer, OBFUSCATE("currentActivity"), OBFUSCATE("Landroid/app/Activity;"));
    jobject currt = env->NewGlobalRef(env->GetStaticObjectField(uplayer, cmeth));
    return currt;
}

jobject GetContext(JNIEnv* globalEnv) {
    jclass activityThread = globalEnv->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jmethodID currentActivityThread = globalEnv->GetStaticMethodID(activityThread, OBFUSCATE("currentActivityThread"), OBFUSCATE("()Landroid/app/ActivityThread;"));
    jobject at = globalEnv->CallStaticObjectMethod(activityThread, currentActivityThread);
    jmethodID getApplication = globalEnv->GetMethodID(activityThread, OBFUSCATE("getApplication"), OBFUSCATE("()Landroid/app/Application;"));
    jobject context = globalEnv->CallObjectMethod(at, getApplication);
    return context;
}

void Toast(JNIEnv *env, jobject thiz, const char *text, int length) {
    jstring jstr = env->NewStringUTF(text);
    jclass toast = env->FindClass(OBFUSCATE("android/widget/Toast"));
    jmethodID methodMakeText =env->GetStaticMethodID(toast,OBFUSCATE("makeText"),OBFUSCATE("(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;"));
    jobject toastobj = env->CallStaticObjectMethod(toast, methodMakeText, thiz, jstr, length);
    jmethodID methodShow = env->GetMethodID(toast, OBFUSCATE("show"), OBFUSCATE("()V"));
    env->CallVoidMethod(toastobj, methodShow);
}

static int str_to_int(const char* valueof) {
    return atoi(valueof);
}

std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

namespace base64 {
    inline std::string get_base64_chars() {
        static std::string base64_chars = OBFUSCATE("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
        return base64_chars;
    }
    inline std::string from_base64(std::string const &data) {
        int counter = 0;
        uint32_t bit_stream = 0;
        std::string decoded;
        int offset = 0;
        const std::string base64_chars = get_base64_chars();
        for (auto const &c : data) {
            auto num_val = base64_chars.find(c);
            if (num_val != std::string::npos) {
                offset = 18 - counter % 4 * 6;
                bit_stream += num_val << offset;
                if (offset == 12) {
                    decoded += static_cast<char>(bit_stream >> 16 & 0xff);
                }
                if (offset == 6) {
                    decoded += static_cast<char>(bit_stream >> 8 & 0xff);
                }
                if (offset == 0 && counter != 4) {
                    decoded += static_cast<char>(bit_stream & 0xff);
                    bit_stream = 0;
                }
            } else if (c != '=') {
                return std::string();
            }
            counter++;
        }
        return decoded;
    }
}

DWORD getAbsoluteAddress(const char *libraryName, DWORD relativeAddr) {
    uintptr_t libBase = findLibrary(libraryName);
    if (libBase == 0)
        return 0;
    return (reinterpret_cast<DWORD>(libBase + relativeAddr));
}

bool isLibraryLoaded(const char *libraryName) {
    //libLoaded = true;
    char line[512] = {0};
    FILE *fp = fopen(OBFUSCATE("/proc/self/maps"), OBFUSCATE("rt"));
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            std::string a = line;
            if (!strstr(line, OBFUSCATE("r--p")) && strstr(line, libraryName)) {
                return true;
            }
        }
        fclose(fp);
    }
    return false;
}

bool isconnected(JNIEnv* env, jobject context) {
    jclass cont = env->FindClass(OBFUSCATE("android/content/Context"));
    jclass cn = env->FindClass(OBFUSCATE("android/net/ConnectivityManager"));
    jclass netc = env->FindClass(OBFUSCATE("android/net/NetworkCapabilities"));
    jmethodID getser = env->GetMethodID(cont, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    jmethodID getcncaps = env->GetMethodID(cn, OBFUSCATE("getNetworkCapabilities"), OBFUSCATE("(Landroid/net/Network;)Landroid/net/NetworkCapabilities;"));
    jmethodID getact = env->GetMethodID(cn, OBFUSCATE("getActiveNetwork"), OBFUSCATE("()Landroid/net/Network;"));
    jmethodID gethas = env->GetMethodID(netc, OBFUSCATE("hasTransport"), OBFUSCATE("(I)Z"));
    jobject conser = env->CallObjectMethod(context, getser, env->NewStringUTF(OBFUSCATE("connectivity")));
    jobject getcnact = (jobject) env->CallObjectMethod(conser, getact);
    jobject getcnc = (jobject) env->CallObjectMethod(conser, getcncaps, getcnact);
    return env->CallBooleanMethod(getcnc, gethas, std::atoi(OBFUSCATE("4")));
}

void displayKeyboard(JNIEnv* env) {
    jclass uplayer = env->FindClass(OBFUSCATE("com/unity3d/player/UnityPlayer"));
    jfieldID cmeth = env->GetStaticFieldID(uplayer, OBFUSCATE("currentActivity"), OBFUSCATE("Landroid/app/Activity;"));
    jobject currt = env->NewGlobalRef(env->GetStaticObjectField(uplayer, cmeth));
    jclass aycls = env->FindClass(OBFUSCATE("android/app/Activity"));
    jmethodID gss = env->GetMethodID(aycls, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    jobject ss = env->CallObjectMethod(currt, gss, env->NewStringUTF(OBFUSCATE("input_method")));
    jclass imcls = env->FindClass(OBFUSCATE("android/view/inputmethod/InputMethodManager"));
    jmethodID tgsifm = env->GetMethodID(imcls, OBFUSCATE("toggleSoftInput"), OBFUSCATE("(II)V"));
    env->CallVoidMethod(ss, tgsifm, 2,0);
}

void copyText(JNIEnv* env, jobject context, std::string text) {
	jclass aycls = env->FindClass(OBFUSCATE("android/app/Activity"));
    jmethodID gss = env->GetMethodID(aycls, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    jobject ss = env->CallObjectMethod(context, gss, env->NewStringUTF(OBFUSCATE("clipboard")));
	jclass clcls = env->FindClass(OBFUSCATE("android/content/ClipData"));
    jmethodID clmeth = env->GetStaticMethodID(clcls, OBFUSCATE("newPlainText"), OBFUSCATE("(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Landroid/content/ClipData;"));
    jclass imcls = env->FindClass(OBFUSCATE("android/content/ClipboardManager"));
    jmethodID tgsifm = env->GetMethodID(imcls, OBFUSCATE("setPrimaryClip"), OBFUSCATE("(Landroid/content/ClipData;)V"));
    env->CallVoidMethod(ss, tgsifm, env->CallStaticObjectMethod(clcls, clmeth, env->NewStringUTF(OBFUSCATE("unknown")), env->NewStringUTF(text.c_str())));
	Toast(env, GetContext(env), OBFUSCATE("Copied to Clipboard!"), 1);
}

void setDialogMD(jobject ctx, JNIEnv *env, const char *title, const char *msg){
    jclass Alert = env->FindClass(OBFUSCATE("android/app/AlertDialog$Builder"));
    jmethodID AlertCons = env->GetMethodID(Alert, OBFUSCATE("<init>"), OBFUSCATE("(Landroid/content/Context;)V"));
    jobject MainAlert = env->NewObject(Alert, AlertCons, ctx);
    jmethodID setTitle = env->GetMethodID(Alert, OBFUSCATE("setTitle"), OBFUSCATE("(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;"));
    env->CallObjectMethod(MainAlert, setTitle, env->NewStringUTF(title));
    jmethodID setMsg = env->GetMethodID(Alert, OBFUSCATE("setMessage"), OBFUSCATE("(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;"));
    env->CallObjectMethod(MainAlert, setMsg, env->NewStringUTF(msg));
    jmethodID setCa = env->GetMethodID(Alert, OBFUSCATE("setCancelable"), OBFUSCATE("(Z)Landroid/app/AlertDialog$Builder;"));
    env->CallObjectMethod(MainAlert, setCa, false);
    jmethodID create = env->GetMethodID(Alert, OBFUSCATE("create"), OBFUSCATE("()Landroid/app/AlertDialog;"));
    jobject creaetob = env->CallObjectMethod(MainAlert, create);
    jclass AlertN = env->FindClass(OBFUSCATE("android/app/AlertDialog"));
    jmethodID show = env->GetMethodID(AlertN, OBFUSCATE("show"), OBFUSCATE("()V"));
    env->CallVoidMethod(creaetob, show);
}

const char* readClip(JNIEnv* env, jobject context) {
    jclass uplr = env->FindClass(OBFUSCATE("com/unity3d/player/UnityPlayer"));
    jmethodID uobj = env->GetMethodID(uplr, OBFUSCATE("<init>"), OBFUSCATE("(Landroid/content/Context;)V"));
	jobject umain = env->NewObject(uplr, uobj, context);
    jmethodID lmao = env->GetMethodID(uplr, OBFUSCATE("getClipboardText"), OBFUSCATE("()Ljava/lang/String;"));
    jstring fvalue = static_cast<jstring>(env->CallObjectMethod(umain, lmao));
    return env->GetStringUTFChars(fvalue, 0);
}

std::string getStorage(JNIEnv* globalEnv) {
	jclass activityClass = globalEnv->FindClass(OBFUSCATE("android/os/Environment"));
	jmethodID getFilesDirMethod = globalEnv->GetStaticMethodID(activityClass, OBFUSCATE("getExternalStorageDirectory"), OBFUSCATE("()Ljava/io/File;"));
	jobject filesDirObj = globalEnv->CallStaticObjectMethod(activityClass, getFilesDirMethod);
	jclass fileClass = globalEnv->FindClass(OBFUSCATE("java/io/File"));
	jmethodID getPathMethod = globalEnv->GetMethodID(fileClass, OBFUSCATE("getAbsolutePath"), OBFUSCATE("()Ljava/lang/String;"));
	jstring cacheDir = (jstring) globalEnv->CallObjectMethod(filesDirObj, getPathMethod);
	return std::string(globalEnv->GetStringUTFChars(cacheDir, 0));
}

const char *getApkSign(JNIEnv *env, jobject context) {
    jclass versionClass = env->FindClass(OBFUSCATE("android/os/Build$VERSION"));
    jfieldID sdkIntFieldID = env->GetStaticFieldID(versionClass, OBFUSCATE("SDK_INT"), OBFUSCATE("I"));
    int sdkInt = env->GetStaticIntField(versionClass, sdkIntFieldID);
    jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
    jmethodID pmMethod = env->GetMethodID(contextClass, OBFUSCATE("getPackageManager"), OBFUSCATE("()Landroid/content/pm/PackageManager;"));
    jobject pm = env->CallObjectMethod(context, pmMethod);
    jclass pmClass = env->GetObjectClass(pm);
    jmethodID piMethod = env->GetMethodID(pmClass, OBFUSCATE("getPackageInfo"), OBFUSCATE("(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;"));
    jmethodID pnMethod = env->GetMethodID(contextClass, OBFUSCATE("getPackageName"), OBFUSCATE("()Ljava/lang/String;"));
    jstring packageName = (jstring) env->CallObjectMethod(context, pnMethod);
    int flags;
    if (sdkInt >= atoi(OBFUSCATE("28"))) {
        flags = 0x08000000; // PackageManager.GET_SIGNING_CERTIFICATES
    } else {
        flags = 0x00000040; // PackageManager.GET_SIGNATURES
    }
    jobject packageInfo = env->CallObjectMethod(pm, piMethod, packageName, flags);
    jclass piClass = env->GetObjectClass(packageInfo);
    jobjectArray signatures;
    if (sdkInt >= atoi(OBFUSCATE("28"))) {
        jfieldID signingInfoField = env->GetFieldID(piClass, OBFUSCATE("signingInfo"), OBFUSCATE("Landroid/content/pm/SigningInfo;"));
        jobject signingInfoObject = env->GetObjectField(packageInfo, signingInfoField);
        jclass signingInfoClass = env->GetObjectClass(signingInfoObject);
        jmethodID signaturesMethod = env->GetMethodID(signingInfoClass, OBFUSCATE("getApkContentsSigners"), OBFUSCATE("()[Landroid/content/pm/Signature;"));
        jobject signaturesObject = env->CallObjectMethod(signingInfoObject, signaturesMethod);
        signatures = (jobjectArray) (signaturesObject);
    } else {
        jfieldID signaturesField = env->GetFieldID(piClass, OBFUSCATE("signatures"), OBFUSCATE("[Landroid/content/pm/Signature;"));
        jobject signaturesObject = env->GetObjectField(packageInfo, signaturesField);
        if (env->IsSameObject(signaturesObject, nullptr)) {
            return OBFUSCATE("");
        }
        signatures = (jobjectArray) (signaturesObject);
    }
    jobject firstSignature = env->GetObjectArrayElement(signatures, 0);
    jclass signatureClass = env->GetObjectClass(firstSignature);
    jmethodID signatureByteMethod = env->GetMethodID(signatureClass, OBFUSCATE("toByteArray"), OBFUSCATE("()[B"));
    jobject signatureByteArray = (jobject) env->CallObjectMethod(firstSignature, signatureByteMethod);
    jclass mdClass = env->FindClass((OBFUSCATE("java/security/MessageDigest")));
    jmethodID mdMethod = env->GetStaticMethodID(mdClass, OBFUSCATE("getInstance"), OBFUSCATE("(Ljava/lang/String;)Ljava/security/MessageDigest;"));
    jobject md5Object = env->CallStaticObjectMethod(mdClass, mdMethod, env->NewStringUTF(OBFUSCATE("MD5")));
    jmethodID mdUpdateMethod = env->GetMethodID(mdClass, OBFUSCATE("update"), OBFUSCATE("([B)V"));// The return value of this function is void, write V
    env->CallVoidMethod(md5Object, mdUpdateMethod, signatureByteArray);
    jmethodID mdDigestMethod = env->GetMethodID(mdClass, OBFUSCATE("digest"), OBFUSCATE("()[B"));
    jobject fingerPrintByteArray = env->CallObjectMethod(md5Object, mdDigestMethod);
    jsize byteArrayLength = env->GetArrayLength(static_cast<jarray>(fingerPrintByteArray));
    jbyte *fingerPrintByteArrayElements = env->GetByteArrayElements(static_cast<jbyteArray>(fingerPrintByteArray), JNI_FALSE);
    char *charArray = (char *) fingerPrintByteArrayElements;
    char *md5 = (char *) calloc(2 * byteArrayLength + 1, sizeof(char));
    int k;
    for (k = 0; k < byteArrayLength; k++) {
        sprintf(&md5[2 * k], OBFUSCATE("%02X"), charArray[k]); // Not sure if the cast is needed
    }
    return md5;
}

const char* getCacheDir(JNIEnv *env) {
  jclass activityClass = env->FindClass(OBFUSCATE("com/unity3d/player/UnityPlayerActivity"));
  jmethodID getFilesDirMethod = env->GetMethodID(activityClass, OBFUSCATE("getCacheDir"), OBFUSCATE("()Ljava/io/File;"));
  jobject filesDirObj = env->CallObjectMethod(GetContext(env), getFilesDirMethod);
  jclass fileClass = env->FindClass(OBFUSCATE("java/io/File"));
  jmethodID getPathMethod = env->GetMethodID(fileClass, OBFUSCATE("getAbsolutePath"), OBFUSCATE("()Ljava/lang/String;"));
  jstring pathObj = (jstring) env->CallObjectMethod(filesDirObj, getPathMethod);
  const char* filesDir = env->GetStringUTFChars(pathObj, NULL);
  //env->ReleaseStringUTFChars(pathObj, dir);
  return filesDir;
}

const char* getApkPath(JNIEnv *globalEnv, jobject context) {
    jclass contextClass = globalEnv->GetObjectClass(context);
    jmethodID getApplicationInfo = globalEnv->GetMethodID(contextClass, OBFUSCATE("getApplicationInfo"), OBFUSCATE("()Landroid/content/pm/ApplicationInfo;"));
    jobject ApplicationInfo_obj = globalEnv->CallObjectMethod(context, getApplicationInfo);
    jclass ApplicationInfoClass = globalEnv->GetObjectClass(ApplicationInfo_obj);
    jmethodID getPackageResourcePath = globalEnv->GetMethodID(contextClass, OBFUSCATE("getPackageResourcePath"), OBFUSCATE("()Ljava/lang/String;"));
    jstring mPackageFilePath = static_cast<jstring>(globalEnv->CallObjectMethod(context, getPackageResourcePath));
    return globalEnv->GetStringUTFChars(mPackageFilePath, 0);
}

const char* getLocale(JNIEnv *env) {
    jclass activityClass = env->FindClass(OBFUSCATE("java/util/Locale"));
    jmethodID getdef = env->GetStaticMethodID(activityClass, OBFUSCATE("getDefault"), OBFUSCATE("()Ljava/util/Locale;"));
	jobject getdefobj = env->CallStaticObjectMethod(activityClass, getdef);
	jmethodID toStr = env->GetMethodID(activityClass, OBFUSCATE("toLanguageTag"), OBFUSCATE("()Ljava/lang/String;"));
	jstring loc = static_cast<jstring>(env->CallObjectMethod(getdefobj, toStr));
    return env->GetStringUTFChars(loc, 0);
}

static int GetAndroidApiLevel() {
    char prop_value[4];
    __system_property_get("ro.build.version.sdk", prop_value);
    return atoi(prop_value);
}

void* FindSym(const char* executable, const char* symname) {
    return xdl_sym(xdl_open(executable, XDL_DEFAULT), symname, NULL);
}

static int dl_iterate_callback(struct dl_phdr_info* info, size_t size, void* data) {
    const char* target_lib = static_cast<const char*>(data);
    if (!info->dlpi_name || !strstr(info->dlpi_name, target_lib)) return 0;
    LOGI("Target library \"%s\" matched at base: %p", target_lib, (void*)info->dlpi_addr);

    uintptr_t xref_target = info->dlpi_addr + 0x1127708;
    size_t page_size = sysconf(_SC_PAGE_SIZE);

    for (size_t i = 0; i < info->dlpi_phnum; i++) {
        const Elf64_Phdr& phdr = info->dlpi_phdr[i];
        if (phdr.p_type != PT_LOAD) continue;

        uintptr_t seg_addr = info->dlpi_addr + phdr.p_vaddr;
        uintptr_t region_start = seg_addr & ~(page_size - 1);
        uintptr_t region_end = (seg_addr + phdr.p_memsz + page_size - 1) & ~(page_size - 1);
        size_t region_size = region_end - region_start;

        const char* target = "nativeInjectEvent";
        char* segment = (char*)seg_addr;
        char* found = (char*)memmem(segment, phdr.p_memsz, target, strlen(target));
        if (!found) continue;

        LOGI("Found \"%s\" at %p", target, ((uintptr_t)found - info->dlpi_addr));
    }
    return 0;
}

void scan_for_string_in_library(const char* target_lib) {
    dl_iterate_phdr(dl_iterate_callback, (void*)target_lib);
}

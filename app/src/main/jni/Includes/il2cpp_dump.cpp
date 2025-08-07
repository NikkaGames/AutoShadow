//
// Created by Nikka on 2025/03/4.
//

#include <android/input.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <jni.h>
#include <sys/system_properties.h>
#include "il2cpp_dump.h"
#include "log.h"
#include "il2cpp-tabledefs.h"
#include "Unity/Quaternion.h"
#include "Unity/Unity.h"
#include "Unity/Vector2.h"
#include "Unity/Vector3.h"
#include "Unity/Vector3Silent.h"
#include "Unity/Vector3idk.hpp"
#include "Unity/Vec3.h"
#include "Font.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/backends/imgui_impl_android.h"

#include <utilities.h>
#include IL2CPPCLASS

#define DO_API(r, n, p) r (*n) p

#include IL2CPPAPI

#undef DO_API

static void *il2cpp_handle = nullptr;

void init_il2cpp_api() {
#define DO_API(r, n, p) n = (r (*) p)xdl_sym(il2cpp_handle, #n, NULL)

#include IL2CPPAPI

#undef DO_API
}

bool dump_game = false;
bool setup = false;

uintptr_t il2cpp_base_z = 0x0;
uintptr_t unity_base_z = 0x0;

#include "dump_func.h"

int glWidth, glHeight;
void SetupImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)glWidth, (float)glHeight);
    ImGui_ImplAndroid_Init(nullptr);
    ImGui_ImplOpenGL3_Init(OBFUSCATE("#version 300 es"));
    ImGui::StyleColorsClassic();
    StyleColorsCustom();
    io.IniFilename =  nullptr;
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 35.0f;
    io.Fonts->AddFontFromMemoryTTF(Roboto_Regular, sizeof(Roboto_Regular), 35.f);
    ImGui::GetStyle().ScaleAllSizes(1.35f);
}

typedef enum { TOUCH_ACTION_DOWN = 0, TOUCH_ACTION_UP, TOUCH_ACTION_MOVE } TOUCH_ACTION;

typedef struct {
    TOUCH_ACTION action;
    float x;
    float y;
    int pointers;
    float y_velocity;
    float x_velocity;
}TOUCH_EVENT;
TOUCH_EVENT g_LastTouchEvent;

bool clearMousePos = true;
bool HandleInputEvent(JNIEnv *env, int motionEvent, int x, int y, int p) {
    float velocity_y = (float)((float)y - g_LastTouchEvent.y) / 100.f;
    g_LastTouchEvent = {.action = (TOUCH_ACTION) motionEvent, .x = static_cast<float>(x), .y = static_cast<float>(y), .pointers = p, .y_velocity = velocity_y};
    ImGuiIO &io = ImGui::GetIO();
    io.MousePos.x = g_LastTouchEvent.x;
    io.MousePos.y = g_LastTouchEvent.y;
    if (motionEvent == 2) {
        if (g_LastTouchEvent.pointers > 1) {
            io.MouseWheel = g_LastTouchEvent.y_velocity;
            io.MouseDown[0] = false;
        } else {
            io.MouseWheel = 0;
        }
    }
    if (motionEvent == 0) {
        io.MouseDown[0] = true;
    }
    if (motionEvent == 1) {
        io.MouseDown[0] = false;
    }
    return true;
}

bool (*old_nativeInjectEvent)(JNIEnv* env, jclass clazz, jobject e);
bool hook_nativeInjectEvent(JNIEnv* env, jclass clazz, jobject e) {
    if (!setup) return old_nativeInjectEvent(env, clazz, e);
    if (!env || !clazz || !e)
        return false;
    jclass MotionEvent = env->FindClass(_("android/view/MotionEvent"));
    
    ImGuiIO &io = ImGui::GetIO();
    if (env->IsInstanceOf(e, MotionEvent)) {
        HandleInputEvent(
            env,
            env->CallIntMethod(e, env->GetMethodID(MotionEvent, _("getActionMasked"), _("()I"))),
            env->CallFloatMethod(e, env->GetMethodID(MotionEvent, _("getX"), _("()F"))),
            env->CallFloatMethod(e, env->GetMethodID(MotionEvent, _("getY"), _("()F"))),
            env->CallIntMethod(e, env->GetMethodID(MotionEvent, _("getPointerCount"), _("()I")))
        );
    }
    
    if (!ImGui::GetIO().MouseDownOwnedUnlessPopupClose[0]){
        return old_nativeInjectEvent(env, clazz, e);
    }
    return false;
}

jint (*old_RegisterNatives)(JNIEnv*, jclass, JNINativeMethod*,jint);
jint hook_RegisterNatives(JNIEnv* env, jclass destinationClass, JNINativeMethod* methods, jint totalMethodCount) {
    int currentNativeMethodNumeration;
    for (currentNativeMethodNumeration = 0; currentNativeMethodNumeration < totalMethodCount; ++currentNativeMethodNumeration) {
        if (!strcmp(methods[currentNativeMethodNumeration].name, _("nativeInjectEvent")) ){
            DobbyHook(methods[currentNativeMethodNumeration].fnPtr, (void*)hook_nativeInjectEvent, (void **)&old_nativeInjectEvent);
        }
    }
    return old_RegisterNatives(env, destinationClass, methods, totalMethodCount);
}

////

bool scenery = false, timeh = false, fall = false, crounds = false;
static int current_scene = 0, rounds_count = 1;
MemoryPatch punch;

float (*UDeltaTime)();

bool menu_open = true;
bool oclicked = false;
float ui_dpi_scale = 0.0f, cltime = 0.0f, nalpha = 102.0f;

void StartTricks(bool* p_open) {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL);
    ImGuiWindowFlags window_flags = 0;
    ImGuiWindowFlags icon_flags = 0;
    icon_flags |= ImGuiWindowFlags_NoBackground;
    icon_flags |= ImGuiWindowFlags_NoResize;
    icon_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    icon_flags |= ImGuiWindowFlags_NoTitleBar;
    icon_flags |= ImGuiWindowFlags_NoScrollbar;
    icon_flags |= ImGuiWindowFlags_NoCollapse;
    static bool show_app_metrics = false;
    static bool show_app_stack_tool = false;
    static bool show_demo_window = false;
    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;
    static bool unsaved_document = false;
    //window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
    if (show_app_metrics)       { ImGui::ShowMetricsWindow(&show_app_metrics); }
    if (show_app_stack_tool)    { ImGui::ShowStackToolWindow(&show_app_stack_tool); }
    if (show_demo_window)    { ImGui::ShowDemoWindow(&show_demo_window); }
    
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + glWidth / 4, main_viewport->WorkPos.y + glHeight / 4), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(1100, 650), ImGuiCond_FirstUseEver);

    InitStyle();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + glWidth / 8, main_viewport->WorkPos.y + glHeight / 8), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);
    ImGui::Begin(OBFUSCATE("#buttonmenu"), nullptr, icon_flags);
    if (ImGui::Button(OBFUSCATE("OPEN\nCLOSE"))) {
        if (menu_open) {
            oclicked = true;
        } else {
            menu_open = true;
        }
    }
    ImGui::End();
    if (menu_open && UDeltaTime) {
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + glWidth / 3, main_viewport->WorkPos.y + glHeight / 3), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(1000, 650), ImGuiCond_FirstUseEver);
        if (oclicked) {
            ui_dpi_scale = ImLerp(ui_dpi_scale , 0.0f,  0.5f * (12.0f * UDeltaTime()));
            cltime = ImLerp(cltime , 0.0f,  0.5f * (2.0f * UDeltaTime()));
            nalpha = ImLerp(nalpha , 0.0f,  0.5f * (12.0f * UDeltaTime()));
            ImGui::SetNextWindowSize(ImVec2(1000 * ui_dpi_scale, 650 * ui_dpi_scale));
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + glWidth / (ui_dpi_scale * 3), main_viewport->WorkPos.y + glHeight / (ui_dpi_scale * 3)));
        } else {
            ui_dpi_scale = ImLerp(ui_dpi_scale , 1.0f,  0.5f * (12.0f * UDeltaTime()));
            cltime = ImLerp(cltime , 1.0f,  0.5f * (2.0f * UDeltaTime()));
            nalpha = ImLerp(nalpha , 0.4f,  0.5f * (12.0f * UDeltaTime()));
            ImGui::SetNextWindowSize(ImVec2(1000 * ui_dpi_scale, 650 * ui_dpi_scale));
            if (ui_dpi_scale < 0.999f) ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + glWidth / (ui_dpi_scale * 3), main_viewport->WorkPos.y + glHeight / (ui_dpi_scale * 3)));
        }
    
        if (!ImGui::Begin(std::string(OBFUSCATE("Modded by Nikka | SF2 - 2.41.7 | ARM64")).c_str(), p_open, window_flags)) {
            ImGui::End();
            return;
        }

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::BulletText(OBFUSCATE("Menu Based on ImGui."));
        ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
        ImGui::Spacing();
        if (ImGui::CollapsingHeader(OBFUSCATE("Cheats"))) {
            if (ImGui::BeginTable(OBFUSCATE("t394"), 2)) {
                ImGui::TableNextColumn(); Toggle(OBFUSCATE("Scenery Spoof"), &scenery);
                ImGui::TableNextColumn(); Toggle(OBFUSCATE("No Timeout"), &timeh);
                ImGui::TableNextColumn(); Toggle(OBFUSCATE("Enable Kick, Punches, etc."), &fall);
                ImGui::TableNextColumn(); Toggle(OBFUSCATE("Rounds Count"), &crounds);
                ImGui::EndTable();
            }
            std::vector<const char*> cstr_options = {
                "eggs",
                "neural_network",
                "bamboo_grove",
                "ships",
                "waterfall",
                "shadow_gate",
                "flowers_field",
                "volcano",
                "factory",
                "flying_rocks",
                "burning_town",
                "heaven",
                "lamps_on_water",
                "sakura",
                "arena",
                "moon"
            };
            if (scenery) ImGui::Combo("Select Scene", &current_scene, cstr_options.data(), cstr_options.size());
            if (crounds) ImGui::SliderInt("Rounds Count", &rounds_count, 1, 50);
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Tools"))) {
            ImGui::Spacing();
            if (ImGui::BeginTable(OBFUSCATE("t4"), 3)) {
                ImGui::TableNextColumn(); Toggle(OBFUSCATE("Metrics / Debugger"), &show_app_metrics);
                ImGui::TableNextColumn(); Toggle(OBFUSCATE("Stack Tool"), &show_app_stack_tool);
                ImGui::TableNextColumn(); Toggle(OBFUSCATE("Show Demo Features"), &show_demo_window);
                ImGui::EndTable();
            }
        }
        
        ImGui::PopItemWidth();
        ImVec2 delta = ImGui::GetIO().MouseDelta;
        ImGuiContext& g = *ImGui::GetCurrentContext();
        ImGuiWindow* window = g.CurrentWindow;
        
        const ImGuiID lastHeldStateId = window->GetID("##lastheldstate");
        bool lastHeld = window->DC.StateStorage->GetBool(lastHeldStateId, false);
        bool hovered = false;
        bool held = false;
        if (g.HoveredId == 0) {
            ImGui::ButtonBehavior(
                window->Rect(),
                window->GetID("##scrolldraggingoverlay"),
                &hovered,
                &held,
                ImGuiButtonFlags_MouseButtonLeft
            );
        }
        if ((held || lastHeld) && (delta.x != 0.0f || delta.y != 0.0f)) {
            ImGui::SetScrollX(window, window->Scroll.x - delta.x);
            ImGui::SetScrollY(window, window->Scroll.y - delta.y);
        }
        window->DC.StateStorage->SetBool(lastHeldStateId, held);
        ImGui::End();
    }
}

void closedelay() {
    do {
        if (oclicked) {
            do {
                usleep(1);
            } while (ui_dpi_scale > 0.01f);
            menu_open = false;
            oclicked = false;
        }
        usleep(1);
    } while (true);
}

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
    if (!setup) {
        SetupImgui();
        setup = true;
    }
    ImGuiIO &io = ImGui::GetIO();
    switch (g_LastTouchEvent.action) {
        case TOUCH_ACTION_MOVE:
            if (g_LastTouchEvent.pointers > 1) {
                io.MouseWheel = g_LastTouchEvent.y_velocity;
                io.MouseDown[0] = false;
            } else {
                io.MouseWheel = 0;
            }
            break;
        case TOUCH_ACTION_DOWN:
            io.MouseDown[0] = true;
            break;
        case TOUCH_ACTION_UP:
            io.MouseDown[0] = false;
            break;
        default:
            break;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(glWidth, glHeight), GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, nalpha)));
    StartTricks((bool*) false);
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::GetBackgroundDrawList()->_ResetForNewFrame();
    ImGui::GetBackgroundDrawList()->_ClearFreeMemory();
    return old_eglSwapBuffers(dpy, surface);
}

////////

FieldInfo* gameController;
void (*ShowPunch)(void*, bool);
void (*ShowKick)(void*, bool);
void (*ShowRanged)(void*, bool);
void (*ShowMagic)(void*, bool);

void GameThread() {
    while (true) {
        if (fall) {
            punch.Modify();
        } else {
            punch.Restore();
        }
        if (gameController) {
            void* ret = nullptr;
            il2cpp_field_static_get_value(gameController, &ret);
            if (!ret) continue;
            *(bool*)((uintptr_t)ret + 0x40) = true;
            *(bool*)((uintptr_t)ret + 0x41) = true;
            *(bool*)((uintptr_t)ret + 0x42) = true;
            *(bool*)((uintptr_t)ret + 0x43) = true;
            *(bool*)((uintptr_t)ret + 0x44) = true;
            *(bool*)((uintptr_t)ret + 0x45) = true;
            *(bool*)((uintptr_t)ret + 0x46) = true;
            void* btns = *(void**)((uintptr_t)ret + 0x38);
            if (fall && btns) {
                ShowKick(btns, true);
                ShowPunch(btns, true);
                ShowRanged(btns, true);
                ShowMagic(btns, true);
            }
        }
        usleep(4170);
    }
}

void (*old_FightUpdate)(void*);
void FightUpdate(void* inst) {
    if (inst) {
        void* fight = *(void**)((uintptr_t)inst + 0x90);
        if (!fight) return old_FightUpdate(inst);
        Il2CppClass* klass = ((Il2CppObject*)fight)->klass;
        void *iter = nullptr;
        while (auto field = il2cpp_class_get_fields(klass, &iter)) {
            auto attrs = field->type->attrs;
            auto field_class = il2cpp_class_from_type(field->type);
            if (equals(field_class->name, _("Boolean"))) {
                //*(bool*)((uintptr_t)fight + field->offset) = true;
            }
        }
        //int u1 = *(int*)((uintptr_t)idk + 0x28);
        //float u2 = *(float*)((uintptr_t)idk + 0xFC);
        //LOGI("Fight %p, %d, %f", fight, u1, u2);
    }
    return old_FightUpdate(inst);
}

void SetScene(void* instance, const char* scene, const char* sound) {
    *(monoString**)((uintptr_t)instance + 0x38) = (monoString*)il2cpp_string_new(scene);
    *(monoString**)((uintptr_t)instance + 0x118) = (monoString*)il2cpp_string_new(sound);
}

void (*SetObscuredInt)(uint64_t, int);
void (*SetObscuredFloat)(uint64_t, float);
void (*SetObscuredLong)(uint64_t, long);
void (*SetObscuredBool)(uint64_t, bool);

void (*old_CreateFight)(void*, void*, void*, void*);
void CreateFight(void* inst, void* fparams, void* unk, void* ulist) {
    if (crounds) SetObscuredInt((uint64_t)fparams + 0x6C, rounds_count);
    if (timeh) SetObscuredInt((uint64_t)fparams + 0x7C, 999);
    void* iter = nullptr;
    while (auto field = il2cpp_class_get_fields(((Il2CppObject*)fparams)->klass, &iter)) {
        auto attrs = field->type->attrs;
        auto field_class = il2cpp_class_from_type(field->type);
        if (equals(field_class->name, _("String"))) {
            monoString* rtms = *(monoString**)((uintptr_t)fparams + field->offset);
            if (rtms) LOGI("Giggles: %s : %s", field->name, rtms->toChars());
        }
    }
    if (scenery) {
        switch (current_scene) {
            case 0: {
                SetScene(fparams, "eggs", "fight25_hive");
                break;
            }
            case 1: {
                SetScene(fparams, "neural_network", "fight8_final_boss");
                break;
            }
            case 2: {
                SetScene(fparams, "bamboo_grove", "fight1_samurai_spirit");
                break;
            }
            case 3: {
                SetScene(fparams, "ships", "fight14_ship_battle");
                break;
            }
            case 4: {
                SetScene(fparams, "waterfall", "fight22_heavenly_clouds");
                break;
            }
            case 5: {
                SetScene(fparams, "shadow_gate", "fight30_gates_of_shadows");
                break;
            }
            case 6: {
                SetScene(fparams, "flowers_field", "fight16_the_battlefield_flowers");
                break;
            }
            case 7: {
                SetScene(fparams, "volcano", "fight19_volcano");
                break;
            }
            case 8: {
                SetScene(fparams, "factory", "fight27_factory");
                break;
            }
            case 9: {
                SetScene(fparams, "flying_rocks", "fight28_flying_rocks");
                break;
            }
            case 10: {
                SetScene(fparams, "burning_town", "fight23_burning_town");
                break;
            }
            case 11: {
                SetScene(fparams, "heaven", "fight22_heavenly_clouds");
                break;
            }
            case 12: {
                SetScene(fparams, "lamps_on_water", "fight11_ronin");
                break;
            }
            case 13: {
                SetScene(fparams, "sakura", "fight2_blade_dance");
                break;
            }
            case 14: {
                SetScene(fparams, "arena", "fight5_ninja_in_the_night");
                break;
            }
            case 15: {
                SetScene(fparams, "moon", "fight10_black_warrior");
                break;
            }
            default: {
                break;
            }
        }
    }
    return old_CreateFight(inst, fparams, unk, ulist);
}

bool (*old_AiEnabled)(void*);
bool get_AiEnabled(void* bcfg) {
    return false;
}

bool (*old_ShowPVP)(void*);
bool get_ShowPVP(void* bcfg) {
    return true;
}

void* (*old_Fight)(void*);
void* Fight(void* inst) {
    void* fight = old_Fight(inst);
    void* idk = *(void**)((uintptr_t)fight + 0x30);
    if (!idk) return fight;
   /* void* iter = nullptr;
    while (auto field = il2cpp_class_get_fields(((Il2CppObject*)idk)->klass, &iter)) {
        auto attrs = field->type->attrs;
        auto field_class = il2cpp_class_from_type(field->type);
        if (equals(field_class->name, _("String"))) {
            monoString* rtms = *(monoString**)((uintptr_t)idk + field->offset);
            if (rtms) LOGI("Giggles: %s : %s", field->name, rtms->toChars());
            if (equals(field->name, "HFONGIBJGPH")) {
                *(monoString**)((uintptr_t)idk + field->offset) = (monoString*)il2cpp_string_new("neural_network");
            } else if (equals(field->name, "BLBBOGJKEFM")) {
                *(monoString**)((uintptr_t)idk + field->offset) = (monoString*)il2cpp_string_new("fight8_final_boss");
            }
        }
    }*/
    return fight;
}

void (*old_refreshRate)(void*, int);
void set_refreshRate(void* res, int val) {
    return old_refreshRate(res, 120);
}

void il2cpp_dump(void *handle, char *outDir) {
    do {
        xdl_iterate_phdr(callback_z, NULL, XDL_FULL_PATHNAME);
        usleep(1000);
    } while (!il2cpp_base_z);
    
    il2cpp_handle = handle;
    init_il2cpp_api();
    
    LOGI("Attaching %p", il2cpp_handle);
    
    if (!il2cpp_thread_current()) il2cpp_thread_attach(il2cpp_domain_get());
    
    LOGI("Attached %p", il2cpp_handle);
    
    size_t asize = 0;
    const Il2CppAssembly **assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &asize);
    
    LOGI("Assemblies %zu", asize);
    
    std::vector<std::string> ams;
    
    for (int i = 0; i < asize; ++i) {
        auto image = il2cpp_assembly_get_image(assemblies[i]);
        auto imageName = std::string(image->name);
        auto pos = imageName.rfind('.');
        auto imageNameNoExt = imageName.substr(0, pos);
        ams.push_back(imageNameNoExt);
    }
    
    LOGI("il2cpp_base: %" PRIx64"", il2cpp_base_z);
    
    auto corlib = il2cpp_get_corlib();
    auto assemblyClass = il2cpp_class_from_name(corlib, "System.Reflection", "Assembly");
    auto assemblyLoad = il2cpp_class_get_method_from_name(assemblyClass, "Load", 1);
    auto assemblyGetTypes = il2cpp_class_get_method_from_name(assemblyClass, "GetTypes", 0);
    if (assemblyLoad && assemblyLoad->methodPointer) {
        LOGI("Assembly::Load: %p", assemblyLoad->methodPointer);
    } else {
        LOGI("miss Assembly::Load");
        return;
    }
    if (assemblyGetTypes && assemblyGetTypes->methodPointer) {
        LOGI("Assembly::GetTypes: %p", assemblyGetTypes->methodPointer);
    } else {
        LOGI("miss Assembly::GetTypes");
        return;
    }
	
	uint32_t typeDefinitionsCount = 0;
	int jj = 0;
	int imgc = 0;
	std::stringstream imageOutput;
	std::stringstream mainOut;
	
	for (auto& asem : ams) {
        jj += 1;
		auto assemblie = il2cpp_domain_assembly_open(il2cpp_domain_get(), asem.c_str());
		if (!assemblie) {
			continue;
		}
        auto image = il2cpp_assembly_get_image(assemblie);
		typeDefinitionsCount += image->typeCount;
	}

    LOGI("typeDefinitionsCount: %i", typeDefinitionsCount);
    
	for (auto& asem : ams) {
        auto assemblie = il2cpp_domain_assembly_open(il2cpp_domain_get(), asem.c_str());
		if (!assemblie) continue;
		auto image = il2cpp_assembly_get_image(assemblie);
		if (dump_game) imageOutput << "// Image " << imgc << ": " << image->name << " - " << image->assembly->referencedAssemblyStart << "\n";
        imgc += 1;
		auto imageName = std::string(image->name);
        auto pos = imageName.rfind('.');
        auto imageNameNoExt = imageName.substr(0, pos);
        auto assemblyFileName = il2cpp_string_new(imageNameNoExt.c_str());
		typedef void *(*Assembly_Load_ftn)(Il2CppString*, void*);
		typedef Il2CppArray *(*Assembly_GetTypes_ftn)(void*, void*);
        auto reflectionAssembly = ((Assembly_Load_ftn)(assemblyLoad->methodPointer))(assemblyFileName, nullptr);
        auto reflectionTypes = ((Assembly_GetTypes_ftn)(assemblyGetTypes->methodPointer))(reflectionAssembly, nullptr);
        monoArray<void*>* items = (monoArray<void*>*)reflectionTypes;
        for (int j = 0; j < items->getCapacity(); ++j) {
            auto klass = il2cpp_class_from_system_type((Il2CppReflectionType*)items->getPointer()[j]);
            
            if (equals(klass->name, _("GameController"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Boolean")) && equals(method->name, _("get_IsPunchEnabled")) && method->parameters_count == 0) {
                        LOGI("Patching %s", method->name);
                        punch = MemoryPatch::createWithHex((uintptr_t)method->methodPointer, TRUE);
                        void *iter = nullptr;
                        while (auto field = il2cpp_class_get_fields(klass, &iter)) {
                            auto attrs = field->type->attrs;
                            if (attrs & FIELD_ATTRIBUTE_STATIC) {
                                auto field_class = il2cpp_class_from_type(field->type);
                                if (equals(field_class->name, _("GameController"))) {
                                    gameController = field;
                                    LOGI("Found %s %p", field_class->name, field->offset);
                                }
                            }
                        }
                    }
                }
            } else if (equals(klass->name, _("ActionButtons"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && equals(method->name, _("ShowPunch")) && method->parameters_count == 1) {
                        LOGI("Assigning %s", method->name);
                        ShowPunch = (void(*)(void*, bool))method->methodPointer;
                    } else if (equals(type->name, _("Void")) && equals(method->name, _("ShowKick")) && method->parameters_count == 1) {
                        LOGI("Assigning %s", method->name);
                        ShowKick = (void(*)(void*, bool))method->methodPointer;
                    } else if (equals(type->name, _("Void")) && equals(method->name, _("ShowRanged")) && method->parameters_count == 1) {
                        LOGI("Assigning %s", method->name);
                        ShowRanged = (void(*)(void*, bool))method->methodPointer;
                    } else if (equals(type->name, _("Void")) && equals(method->name, _("ShowMagic")) && method->parameters_count == 1) {
                        LOGI("Assigning %s", method->name);
                        ShowMagic = (void(*)(void*, bool))method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("FightScene"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && equals(method->name, _("FixedUpdate")) && method->parameters_count == 0) {
                        LOGI("Hooking %s", method->name);
                        DobbyHook((void*)method->methodPointer, (void*)FightUpdate, (void**)&old_FightUpdate);
                    } else if (equals(type->name, _("Void")) && equals(method->name, _("CreateFight")) && method->parameters_count == 3) {
                        LOGI("Hooking %s", method->name);
                        DobbyHook((void*)method->methodPointer, (void*)CreateFight, (void**)&old_CreateFight);
                    }
                }
            } else if (equals(klass->name, _("Resolution"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Int32")) && equals(method->name, _("set_refreshRate")) && method->parameters_count == 1) {
                        LOGI("Hooking %s", method->name);
                        DobbyHook((void*)method->methodPointer, (void*)set_refreshRate, (void**)&old_refreshRate);
                    }
                }
            } else if (equals(klass->name, _("BuildSettingsConfig"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Boolean")) && equals(method->name, _("get_AiEnabled")) && method->parameters_count == 0) {
                        LOGI("Hooking %s", method->name);
                        //DobbyHook((void*)method->methodPointer, (void*)get_AiEnabled, (void**)&old_AiEnabled);
                    } else if (equals(type->name, _("Boolean")) && equals(method->name, _("get_ShowPVP")) && method->parameters_count == 0) {
                        LOGI("Hooking %s", method->name);
                        //DobbyHook((void*)method->methodPointer, (void*)get_ShowPVP, (void**)&old_ShowPVP);
                    }
                }
            } else if (equals(klass->name, _("Time"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Single")) && equals(method->name, _("get_deltaTime")) && method->parameters_count == 0) {
                        LOGI("Assigning %s", method->name);
                        UDeltaTime = (float (*)())method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("XmlNode"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("String")) && equals(method->name, _("get_Name")) && method->parameters_count == 0) {
                        //LOGI("Hooking %s", method->name);
                        //DobbyHook((void*)method->methodPointer, (void*)XmlName, (void**)&old_XmlName);
                    } else if (equals(type->name, _("String")) && equals(method->name, _("get_Value")) && method->parameters_count == 0) {
                        //LOGI("Hooking %s", method->name);
                        //DobbyHook((void*)method->methodPointer, (void*)XmlValue, (void**)&old_XmlValue);
                    }
                }
            } else if (equals(klass->name, _("ObscuredInt"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && equals(method->name, _(".ctor")) && method->parameters_count == 1) {
                        LOGI("Assigning %s", method->name);
                        SetObscuredInt = (void(*)(uint64_t,int))method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("ObscuredFloat"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && equals(method->name, _(".ctor")) && method->parameters_count == 1) {
                        LOGI("Assigning %s", method->name);
                        SetObscuredFloat = (void(*)(uint64_t,float))method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("ObscuredLong"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && equals(method->name, _(".ctor")) && method->parameters_count == 1) {
                        LOGI("Assigning %s", method->name);
                        SetObscuredLong = (void(*)(uint64_t,long))method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("ObscuredBool"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && equals(method->name, _(".ctor")) && method->parameters_count == 1) {
                        LOGI("Assigning %s", method->name);
                        SetObscuredBool = (void(*)(uint64_t,bool))method->methodPointer;
                    }
                }
            }
            if (dump_game) mainOut << "\n\n// Assembly: " << imageName << ".dll" << dump_type(klass, j);
        }
	}
	
    if (dump_game) {
        std::ofstream out("/data/user/0/com.nekki.shadowfight/cache/dump.cs");
        out << imageOutput.str();
        out << mainOut.str();
        out.close();
        LOGI("dump done!");
    } else {
        std::thread(closedelay).detach();
        std::thread(GameThread).detach();
        //scan_for_string_in_library("libunity.so");
        DobbyHook(FindSym(_("libEGL.so"), _("eglSwapBuffers")), (void *)hook_eglSwapBuffers, (void **)&old_eglSwapBuffers);
    }
}

jboolean load(JNIEnv *env, jclass clazz, jstring native_library_dir) {
    JavaVM *java_vm;
    jint jvm = env->GetJavaVM(&java_vm);
    ProcMap myproc = KittyMemory::getLibraryMap(_("libmain.so"));
    std::string unit = ReplaceString(std::string(myproc.pathname), _("libmain.so"), _("libunity.so"));
    void* g_unity_handle = dlopen(unit.c_str(), atoi(_("1")));
    jint (*JniLoad)(JavaVM*, void*) = (jint (*)(JavaVM*, void*))dlsym(g_unity_handle, _("JNI_OnLoad"));
    JniLoad(java_vm, nullptr);
    return JNI_TRUE;
}

jboolean unload(JNIEnv *env, jclass clazz) {
    JavaVM *java_vm;
    jint jvm = env->GetJavaVM(&java_vm);
    ProcMap myproc = KittyMemory::getLibraryMap(_("libmain.so"));
    std::string unit = ReplaceString(std::string(myproc.pathname), _("libmain.so"), _("libunity.so"));
    void* g_unity_handle = dlopen(unit.c_str(), atoi(_("1")));
    jint (*JniUnload)(JavaVM*, void*) = (jint (*)(JavaVM*, void*))dlsym(g_unity_handle, _("JNI_OnUnload"));
    JniUnload(java_vm, nullptr);
    dlclose(g_unity_handle);
    return JNI_TRUE;
}

JNINativeMethod methods[] = {
        {_("load"), _("(Ljava/lang/String;)Z"), (void *)load},
        {_("unload"), _("()Z"), (void *)unload}
};

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);
    jclass clazz = globalEnv->FindClass(_("com/unity3d/player/NativeLoader"));
	globalEnv->RegisterNatives(clazz, methods,sizeof(methods) / sizeof(methods[0]));
    DobbyHook((void*)globalEnv->functions->RegisterNatives, (void*)hook_RegisterNatives, (void **)&old_RegisterNatives);
    return JNI_VERSION_1_6;
}

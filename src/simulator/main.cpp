#include "Memory.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <algorithm>
#include <exception>
#include <fmt/format.h>
#include <fstream>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer.h>
#include <numeric>
#include <string_view>
#include <variant>
#include <vector>

namespace ImGui {
#include <imgui_memory_editor.h>

template <typename T, typename... Args>
IMGUI_API void TextFmt(T &&fmt, const Args &...args) {
    std::string str = fmt::format(std::forward<T>(fmt), args...);
    ImGui::TextUnformatted(&*str.begin(), &*str.end());
}
} // namespace ImGui

using namespace SDL2pp;

static constexpr uint32_t SCREEN_WIDTH = 640;
static constexpr uint32_t SCREEN_HEIGHT = 480;
static constexpr uint32_t framerate = 60;

class MemoryPerf {
    size_t counter = 0;
    size_t latency = 0;
    std::vector<size_t> latency_history;

  public:
    MemoryPerf(Garand::Memory &init) { this->counter = init.get_counter(); }

    void update(Garand::Memory &state) {
        auto new_counter = state.get_counter();
        auto latency = state.get_latency() * (new_counter - this->counter);
        this->counter = new_counter;
        this->latency = latency;
        latency_history.push_back(latency);
    }

    size_t get_latency() { return this->latency; }
    double get_latency_avg() {
        if (latency_history.size() == 0) {
            return 0.;
        } else {
            return std::accumulate(latency_history.begin(),
                                   latency_history.end(), size_t{0}) *
                   1. / latency_history.size();
        }
    }
};

void memoryDemoWindow() {
    ImGui::Begin("Memory demo");

    const char *items[] = {"No cache", "L1+L2+L3"};
    static int item_current = 0;

    using Garand::Memory;
    using Garand::Cache;
    static Memory memory;
    static MemoryPerf perf{memory};
    if (ImGui::Combo("Cache mode", &item_current, items, IM_ARRAYSIZE(items))) {
        switch (item_current)
        {
        case 1:
            memory = Memory(0x200000);
            break;

        case 0:
        default:
            memory = Memory();
            break;
        }
        // Attach to perf counter
        perf = MemoryPerf{memory};
    }

    ImGui::Text("Memory size: %llu", memory.get_size());

    ImGui::Text("Latency: %llu", perf.get_latency());
    ImGui::Text("Latency Avg.: %lf", perf.get_latency_avg());

    static Garand::AddressSize address = 0;
    constexpr Garand::AddressSize address_step = 1;
    constexpr Garand::AddressSize address_step_fast = 16;
    static Garand::LoadSize value = 0;
    constexpr Garand::LoadSize value_step = 0;

    ImGui::InputScalar("address", ImGuiDataType_U32, &address, &address_step,
                       &address_step_fast, "%08lX");
    ImGui::InputScalar("value", ImGuiDataType_U32, &value, &value_step,
                       &value_step, "%lu");

    if (ImGui::Button("Load")) {
        value = *memory.load(address);
        perf.update(memory);
    }
    ImGui::SameLine();
    if (ImGui::Button("Store")) {
        memory.store(address, value);
        perf.update(memory);
    }
    static ImGui::MemoryEditor mem_edit;
    mem_edit.DrawContents(memory.get_raw(), memory.get_size());
    ImGui::End();
}

void debuggui() {
    static bool show_demo_window = false;
    static bool show_memory_demo_window = false;
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    if (show_memory_demo_window) {
        memoryDemoWindow();
    }
    // Edit bools storing our window open/close state
    ImGui::Begin("Control UI");
    ImGui::Checkbox("Demo Window", &show_demo_window);
    ImGui::Checkbox("Memory Demo Window", &show_memory_demo_window);
    ImGui::End();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    try {
        fmt::print("Booting\n");
        // Initialize SDL library
        SDL sdl{SDL_INIT_VIDEO};

        Window window("garand", SDL_WINDOWPOS_UNDEFINED,
                      SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                      SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

        // Create accelerated video renderer with default driver
        Renderer renderer{window, -1,
                          SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED};

        // Checking
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForSDLRenderer(window.Get(), renderer.Get());
        ImGui_ImplSDLRenderer_Init(renderer.Get());
        // Main loop
        auto base_ticks = SDL_GetTicks();
        while (1) {
            // Timing: calculate difference between this and previous frame
            // in milliseconds
            auto const frame_ticks = SDL_GetTicks();
            auto const frame_delta = frame_ticks - base_ticks;
            base_ticks = frame_ticks;

            // Event processing:
            // - If window is closed, or Q or Escape buttons are pressed,
            //   quit the application
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT) {
                    return 0;
                } else if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 0;
                    }
                }
            }

            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            debuggui();
            ImGui::Render();

            // Clear screen
            renderer.Clear();

            // Show rendered frame
            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
            renderer.Present();

            // Frame limiter
            SDL_Delay(1);
        }

        // Here all resources are automatically released and library
        // deinitialized
        ImGui_ImplSDLRenderer_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        return 0;
    } catch (std::exception &e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
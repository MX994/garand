#include <SDL2pp/SDL2pp.hh>
#include <algorithm>
#include <exception>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer.h>
#include <numeric>
#include <set>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "Disassemble.hpp"
#include "Instruction.hpp"
#include "Instructions.hpp"
#include "Memory.hpp"
#include "Processor.hpp"
#include "Registers.hpp"

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
static constexpr uint16_t GPU_WIDTH = 128, GPU_HEIGHT = 64;
static constexpr uint32_t framerate = 60;

class MemoryPerf {
    size_t counter = 0;
    size_t latency = 0;
    std::vector<size_t> latency_history;

  public:
    MemoryPerf(Garand::Memory &init) { this->counter = init.get_counter(); }

    void update(Garand::Memory &state) {
        auto new_counter = state.get_counter();
        auto latency = (new_counter - this->counter);
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
    size_t *get_all() { return latency_history.data(); }
    size_t length() { return latency_history.size(); }
};

void cacheMemWindow(Garand::Memory &mem) {
    ImGui::Begin("Cache view");
    static int block_id = 0;
    ImGui::InputInt("Block", &block_id);
    if (block_id >= Garand::CACHE_BLOCK_COUNT) {
        block_id = Garand::CACHE_BLOCK_COUNT - 1;
    }
    if (block_id < 0) {
        block_id = 0;
    }

    static ImGui::MemoryEditor mem_edit;
    auto *block = &mem.Blocks[block_id];

    // fmt::print("Cache block {} read\n", block_id);
    ImGui::Text("Tag: %d", block->Tag);
    ImGui::Text("Valid: %d", block->Valid);
    mem_edit.DrawContents(block->Data, Garand::CACHE_BLOCK_SIZE); // This here
    ImGui::End();
}

void memoryDemoWindow() {
    ImGui::Begin("Memory demo");

    const char *items[] = {"No cache", "Cache"};
    static int item_current = 0;

    using Garand::Memory;
    static Memory memory;
    static MemoryPerf perf{memory};
    if (ImGui::Combo("Cache mode", &item_current, items, IM_ARRAYSIZE(items))) {
        switch (item_current) {
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
    static bool cache_view = false;

    ImGui::Checkbox("Cache Window", &cache_view);

    if (cache_view) {
        cacheMemWindow(memory);
    }

    static Garand::AddressSize address = 0;
    constexpr Garand::AddressSize address_step = 1;
    constexpr Garand::AddressSize address_step_fast = 16;
    static auto value = 0U;
    constexpr auto value_step = 0U;

    ImGui::InputScalar("address", ImGuiDataType_U32, &address, &address_step,
                       &address_step_fast, "%08lX");
    ImGui::InputScalar("value", ImGuiDataType_U32, &value, &value_step,
                       &value_step, "%lu");

    if (ImGui::Button("Load")) {
        value = *memory.load<decltype(value)>(address);
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

void drawRegTable(const char *id, const Garand::Registers &regs) {
    if (ImGui::BeginTable(id, 2, 0)) {
        for (auto i = 0U; i < Garand::REGISTERS_GP_CNT; ++i) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("R%u", i);
            ImGui::TableNextColumn();
            ImGui::Text("%llu", regs.GeneralPurpose[i]);
        }
        for (auto i = 0U; i < Garand::REGISTERS_IO_CNT; ++i) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("I%u", i);
            ImGui::TableNextColumn();
            ImGui::Text("%llu", regs.IO[i]);
        }
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("NZCV");
        ImGui::TableNextColumn();
        ImGui::Text("%d:%d:%d:%d", regs.Condition.Negative, regs.Condition.Zero,
                    regs.Condition.Carry, regs.Condition.Overflow);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("LR");
        ImGui::TableNextColumn();
        ImGui::Text("%llu", regs.LinkRegister);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("SP");
        ImGui::TableNextColumn();
        ImGui::Text("%llu", regs.StackPointer);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("PC");
        ImGui::TableNextColumn();
        ImGui::Text("%llu", regs.ProgramCounter);

        ImGui::EndTable();
    }
}

void instructionDemoWindow() {
    ImGui::Begin("Instruction");
    static auto asmline = Garand::GarandInstruction{
        .ConditionFlags = 0b0001,
        .Operation = 0b000100,
        .InstructionSpecific = 0b00000100000100000010,
    };
    static Garand::Memory mem;
    const auto mne = Garand::disassemble(asmline);
    ImGui::Text("Assembly: %s", mne.c_str());
    constexpr auto value_step = 1;
    static uint8_t cond = asmline.ConditionFlags;
    static uint8_t oper = asmline.Operation;
    static uint32_t insp = asmline.InstructionSpecific;
    ImGui::Text("Modifying instruction");
    if (ImGui::InputScalar("CF", ImGuiDataType_U8, &cond, &value_step,
                           &value_step, "%lX")) {
        asmline.ConditionFlags = cond;
    }
    if (ImGui::InputScalar("OP", ImGuiDataType_U8, &oper, &value_step,
                           &value_step, "%lX")) {
        asmline.Operation = oper;
    }
    if (ImGui::InputScalar("IN", ImGuiDataType_U32, &insp, &value_step,
                           &value_step, "%lX")) {
        asmline.InstructionSpecific = insp;
    }

    static auto regs = Garand::Registers{};
    ImGui::Begin("Register Table");
    drawRegTable("insdemo_reg_table", regs);
    ImGui::End();

    if (ImGui::Button("Run")) {
        auto &ins = asmline;
        auto decode = Garand::Instruction::Decode(ins);
        // Based on Serg change, regs must be uint64_t*
        auto wb = Garand::Instruction::Execute(decode, ins, mem, &regs);
        Garand::Instruction::WriteBack(wb, mem);
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Register")) {
        regs = Garand::Registers{};
    }
    ImGui::End();
}

Garand::Memory *graphic_buffer = nullptr;

constexpr void PipeView(char const *id, auto const src) {
    if (ImGui::TreeNodeEx(id, ImGuiTreeNodeFlags_DefaultOpen)) {
        if (src) {
            ImGui::Text("I: %s", Garand::disassemble(src->Instruction).c_str());
            ImGui::Text("C: %llu", src->CycleCounter);
            ImGui::Text("M: %s", fmt::format("{}", src->CycleNeeded).c_str());
            ImGui::Text("P: %d", src->Processed);
            auto dec = src->decodedInstruction.parameter;
            ImGui::Text("D: %s", fmt::format("{}", dec.Registers).c_str());
            if (dec.Immediate) {
                ImGui::SameLine();
                ImGui::Text("| %s", fmt::format("{}", *dec.Immediate).c_str());
            } else {
                ImGui::SameLine();
                ImGui::Text("| None");
            }
        } else {
            ImGui::Text("(Empty)");
            ImGui::Text("(Empty)");
            ImGui::Text("(Empty)");
            ImGui::Text("(Empty)");
            ImGui::Text("(Empty)");
        }
        ImGui::TreePop();
    }
};

void pipelineDemoWindow() {
    ImGui::Begin("Pipeline");
    static Garand::Processor cpu;
    auto &memory = cpu.ReadMem();
    static bool load_success = true;
    static char path[0x1000];
    constexpr auto value_step = 0;

    static Garand::AddressSize load_offset = 0;
    ImGui::InputScalar("Load Offset (0x)", ImGuiDataType_U32, &load_offset,
                       &value_step, &value_step, "%lX");
    ImGui::InputTextWithHint("Executable", "path goes here", path,
                             IM_ARRAYSIZE(path));
    ImGui::SameLine();
    if (ImGui::Button("Load")) {
        FILE *fd = fopen(path, "rb");
        load_success = !ferror(fd);
        if (load_success) {
            auto src = memory.get_raw() + load_offset;
            auto read_size = memory.get_size() - load_offset;
            fread(src, 1, read_size, fd);
            memory.invalidate();
            fclose(fd);
        }
    }
    if (!load_success) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 160.f / 255, 122.f / 255, 1.f),
                           "(Failed)");
    }

    cacheMemWindow(memory);
    ImGui::Begin("Memory View");
    static ImGui::MemoryEditor mem_edit;
    mem_edit.DrawContents(memory.get_raw(), memory.get_size());
    ImGui::End();

    auto &regs = cpu.Regs();
    ImGui::Begin("Register Table");
    drawRegTable("pipdemo_reg_table", regs);
    ImGui::End();
    static auto is_skipping = false;
    static auto skip_count = 1;
    ImGui::Checkbox("Skipping", &is_skipping);
    if (is_skipping) {
        ImGui::SameLine();
        ImGui::InputInt("count", &skip_count);
    }
    ImGui::Checkbox("Pipeline", &cpu.pipeline);
    ImGui::SameLine();
    if (ImGui::Checkbox("Cache", &memory.cacheOn)) {
        if (!memory.cacheOn) {
            memory.invalidate();
        }
    }
    ImGui::Text("PC: 0x%llX", regs.ProgramCounter);
    if (regs.ProgramCounter + 4 <= memory.get_size()) {
        auto &raw_load = *reinterpret_cast<uint32_t *>(memory.get_raw() +
                                                       regs.ProgramCounter);
        ImGui::Text(
            "Speculating next: %s",
            Garand::disassemble(Garand::Instruction::Encode(raw_load)).c_str());
    } else {
        ImGui::Text("(End of Program)");
    }
    ImGui::Text("Clock: %llu cycle(s)", cpu.ReadClock());

    PipeView("Fetch", cpu.View()[0]);
    PipeView("Decode", cpu.View()[1]);
    PipeView("Execute", cpu.View()[2]);
    PipeView("Write-back", cpu.View()[3]);

    static bool is_running = false;
    static std::set<Garand::AddressSize> breakpoints;
    static Garand::AddressSize breakpoint_buffer;
    auto bp_list_s = fmt::format("{}", breakpoints);
    ImGui::Text("%s", bp_list_s.c_str());
    if (ImGui::Button("Set Breakpoint")) {
        breakpoints.insert(breakpoint_buffer);
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove Breakpoint")) {
        breakpoints.erase(breakpoint_buffer);
    }
    ImGui::InputScalar("Breakpoint", ImGuiDataType_U32, &breakpoint_buffer,
                       &value_step, &value_step, "%lX");
    ImGui::Text("IsRunning: %d", is_running);
    if (ImGui::Button("Run")) {
        is_running = !is_running;
    }
    static bool is_bp_hit = false;
    auto execpc = cpu.ReadExecPC();
    is_bp_hit = (!is_bp_hit) && breakpoints.count(execpc);
    if (is_bp_hit) {
        is_running = false;
    }
    if (is_running) {
        cpu.Step();
    }
    for (auto i = 0; is_running && is_skipping && i < skip_count; ++i) {
        is_bp_hit = (!is_bp_hit) && breakpoints.count(cpu.ReadExecPC());
        if (is_bp_hit) {
            is_running = false;
        }
        if (is_running) {
            cpu.Step();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Step")) {
        cpu.Step();
    }
    if (ImGui::Button("Reset Register")) {
        cpu.ResetRegs();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Scratchpad")) {
        cpu = Garand::Processor();
    }
    ImGui::End();
}

void disassemblerDemoWindow() {
    ImGui::Begin("Disassembler demo");
    static std::vector<uint8_t> mem(0x10000);
    static ImGui::MemoryEditor mem_edit;
    static std::string buffer;
    static char path[0x1000];
    static bool load_success = true;
    if (ImGui::Button("Reset")) {
        mem.clear();
    }
    ImGui::InputTextWithHint("Executable", "path goes here", path,
                             IM_ARRAYSIZE(path));
    ImGui::SameLine();
    if (ImGui::Button("Load")) {
        auto fd = std::fstream(path, std::fstream::in);
        load_success = fd.is_open();
        if (fd.is_open()) {
            fd.read(reinterpret_cast<char *>(mem.data()), mem.size());
        }
    }
    if (!load_success) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 160.f / 255, 122.f / 255, 1.f),
                           "(Failed)");
    }
    ImGui::InputTextMultiline("Disassmble\n", buffer.data(), buffer.size());
    mem_edit.DrawContents(mem.data(), mem.size());
    if (mem_edit.DataEditingTakeFocus) {
        auto iter = mem_edit.DataEditingAddr;
        auto count = 16;
        buffer.clear();
        while ((iter + 4 <= mem.size()) && (--count >= 0)) {
            auto load = *reinterpret_cast<uint32_t *>(&mem[iter]);
            iter += 4;
            buffer += Garand::disassemble(Garand::Instruction::Encode(load));
            buffer += "\n";
        }
    }
    ImGui::End();
}

void simulatorWindow() {
    ImGui::Begin("Simulator");
    static Garand::Processor cpu;
    auto &memory = cpu.ReadMem();
    graphic_buffer = &memory;
    static bool load_success = true;
    static char path[0x1000];
    constexpr auto value_step = 0;

    static Garand::AddressSize load_offset = 0;
    ImGui::InputScalar("Load Offset (0x)", ImGuiDataType_U32, &load_offset,
                       &value_step, &value_step, "%lX");
    ImGui::InputTextWithHint("Executable", "path goes here", path,
                             IM_ARRAYSIZE(path));
    ImGui::SameLine();
    if (ImGui::Button("Load")) {
        FILE *fd = fopen(path, "rb");
        load_success = !ferror(fd);
        if (load_success) {
            auto src = memory.get_raw() + load_offset;
            auto read_size = memory.get_size() - load_offset;
            fread(src, 1, read_size, fd);
            memory.invalidate();
            fclose(fd);
        }
    }
    if (!load_success) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 160.f / 255, 122.f / 255, 1.f),
                           "(Failed)");
    }

    cacheMemWindow(memory);
    ImGui::Begin("Memory View");
    static ImGui::MemoryEditor mem_edit;
    mem_edit.DrawContents(memory.get_raw(), memory.get_size());
    ImGui::End();

    auto &regs = cpu.Regs();
    ImGui::Begin("Register Table");
    drawRegTable("sim_reg_table", regs);
    ImGui::End();
    static auto is_skipping = false;
    static auto skip_count = 1;
    ImGui::Begin("Features");
    ImGui::Checkbox("Turn on Skipping", &is_skipping);
    if (is_skipping) {
        ImGui::SameLine();
        ImGui::PushItemWidth(125.f);
        ImGui::InputInt("count", &skip_count);
        ImGui::PopItemWidth();
    }
    ImGui::Checkbox("Pipeline", &cpu.pipeline);
    ImGui::SameLine();
    if (ImGui::Checkbox("Cache", &memory.cacheOn)) {
        if (!memory.cacheOn) {
            // Invalidate all cache, else the data is thought to be on cache
            memory.invalidate();
        }
    }
    ImGui::End();
    ImGui::Text("PC: 0x%llX", regs.ProgramCounter);
    if (regs.ProgramCounter + 4 <= memory.get_size()) {
        auto &raw_load = *reinterpret_cast<uint32_t *>(memory.get_raw() +
                                                       regs.ProgramCounter);
        ImGui::Text(
            "Speculating next: %s",
            Garand::disassemble(Garand::Instruction::Encode(raw_load)).c_str());
    } else {
        ImGui::Text("(End of Program)");
    }
    ImGui::Text("Clock: %llu cycle(s)", cpu.ReadClock());

    ImGui::Begin("Pipeline view");
    PipeView("Fetch", cpu.View()[0]);
    PipeView("Decode", cpu.View()[1]);
    PipeView("Execute", cpu.View()[2]);
    PipeView("Write-back", cpu.View()[3]);
    ImGui::End();

    static bool is_running = false;
    static std::set<Garand::AddressSize> breakpoints;
    static Garand::AddressSize breakpoint_buffer;
    auto bp_list_s = fmt::format("{}", breakpoints);
    ImGui::Text("%s", bp_list_s.c_str());
    if (ImGui::Button("Set Breakpoint")) {
        breakpoints.insert(breakpoint_buffer);
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove Breakpoint")) {
        breakpoints.erase(breakpoint_buffer);
    }
    ImGui::InputScalar("Breakpoint", ImGuiDataType_U32, &breakpoint_buffer,
                       &value_step, &value_step, "%lX");
    ImGui::Text("IsRunning: %d", is_running);
    if (ImGui::Button("Run")) {
        is_running = !is_running;
    }
    static bool is_bp_hit = false;
    is_bp_hit = (!is_bp_hit) && breakpoints.count(cpu.ReadExecPC());
    if (is_bp_hit) {
        is_running = false;
    }
    if (is_running) {
        cpu.Step();
    }
    for (auto i = 0; is_running && is_skipping && i < skip_count; ++i) {
        is_bp_hit = (!is_bp_hit) && breakpoints.count(cpu.ReadExecPC());
        if (is_bp_hit) {
            is_running = false;
        }
        if (is_running) {
            cpu.Step();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Step")) {
        cpu.Step();
    }
    if (ImGui::Button("Reset Register")) {
        cpu.ResetRegs();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Processor")) {
        cpu = Garand::Processor();
    }
    ImGui::End();
}

void debuggui() {
    static bool show_demo_window = false;
    static bool show_memory_demo_window = false;
    static bool show_instruction_demo_window = false;
    static bool show_pipeline_demo_window = false;
    static bool show_disasm_demo_window = false;
    static bool show_simulator_window = false;
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    if (show_memory_demo_window) {
        memoryDemoWindow();
    }
    if (show_instruction_demo_window) {
        instructionDemoWindow();
    }
    if (show_pipeline_demo_window) {
        pipelineDemoWindow();
    }
    if (show_disasm_demo_window) {
        disassemblerDemoWindow();
    }
    if (show_simulator_window) {
        simulatorWindow();
    }
    // Edit bools storing our window open/close state
    ImGui::Begin("Control UI");
    ImGui::Checkbox("Demo Window", &show_demo_window);
    ImGui::Checkbox("Memory Demo", &show_memory_demo_window);
    ImGui::Checkbox("Instruction Demo", &show_instruction_demo_window);
    ImGui::Checkbox("Pipeline Demo", &show_pipeline_demo_window);
    ImGui::Checkbox("Disassembler Demo", &show_disasm_demo_window);
    ImGui::Checkbox("Simulator", &show_simulator_window);
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
        Renderer renderer{window, -1, SDL_RENDERER_ACCELERATED};

        // Checking
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        // Uncomment this to increase UI framerate
        // io.DeltaTime = 1.f / 2000.f;
        ImGui::StyleColorsDark();
        for (int k = 0; k < SDL_GetNumVideoDrivers(); ++k) {
            fmt::print("Video driver found: {}\n", SDL_GetVideoDriver(k));
        }
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

            // Copy texture
            if (graphic_buffer) {
                Texture sprite(renderer, SDL_PIXELFORMAT_RGB888,
                               SDL_TEXTUREACCESS_STATIC, GPU_WIDTH, GPU_HEIGHT);
                uint8_t *GFXBuffer = graphic_buffer->get_raw();
                sprite.Update(NullOpt,
                              GFXBuffer + *(uint32_t *)(GFXBuffer + 0x1000),
                              GPU_WIDTH * 4);
                renderer.Copy(sprite, NullOpt);
            }

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
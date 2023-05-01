#include "Disassemble.hpp"
#include "Instruction.hpp"
#include "Instructions.hpp"
#include "Memory.hpp"
#include "Processor.hpp"
#include "Registers.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Texture.hh>
#include <algorithm>
#include <exception>
#include <fmt/format.h>
#include <fstream>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer.h>
#include <numeric>
#include <string>
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
static Garand::Memory mem;

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
    static MemoryPerf perf(mem);
    if (ImGui::Combo("Cache mode", &item_current, items, IM_ARRAYSIZE(items))) {
        switch (item_current) {
        case 1:
            mem = Garand::Memory(0x200000);
            break;

        case 0:
        default:
            mem = Garand::Memory();
            break;
        }
        // Attach to perf counter
        perf = MemoryPerf{mem};
    }

    ImGui::Text("Memory size: %llu", mem.get_size());

    ImGui::Text("Latency: %llu", perf.get_latency());
    ImGui::Text("Latency Avg.: %lf", perf.get_latency_avg());
    static bool cache_view = false;

    ImGui::Checkbox("Cache Window", &cache_view);

    if (cache_view) {
        cacheMemWindow(mem);
    }

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
        value = *mem.load(address);
        perf.update(mem);
    }
    ImGui::SameLine();
    if (ImGui::Button("Store")) {
        mem.store(address, value);
        perf.update(mem);
    }
    static ImGui::MemoryEditor mem_edit;
    mem_edit.DrawContents(mem.get_raw(), mem.get_size());
    mem_edit.WriteFn = [](ImU8* data, size_t off, ImU8 d) {
        mem.store(off, d);
        perf.update(mem);
    };
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
        auto wb =
            Garand::Instruction::Execute(decode, ins, mem, (uint64_t *)&regs);
        Garand::Instruction::WriteBack(wb, mem);
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Register")) {
        regs = Garand::Registers{};
    }
    ImGui::End();
}

void pipelineDemoWindow() {
    ImGui::Begin("Pipeline");
    static auto asm_input = std::vector<Garand::GarandInstruction>{};
    static auto asm_input_idx = 0U;
    static auto next_execution = 0U;
    static Garand::Processor cpu;
    if (ImGui::BeginListBox("AssemblyPipeline")) {
        for (auto n = 0U; n < asm_input.size(); ++n) {
            const bool is_selected = (asm_input_idx == n);
            const bool is_next_execution = (next_execution == n);
            const auto mne = Garand::disassemble(asm_input[n]);
            if (ImGui::Selectable(fmt::format("##{0}", mne).c_str(),
                                  is_selected)) {
                asm_input_idx = n;
            }
            ImGui::SameLine();
            if (is_next_execution) {
                ImGui::TextColored(ImVec4(0.533f, 0.929f, 1.0f, 1.0f), "%s",
                                   mne.c_str());
            } else {
                ImGui::Text("%s", mne.c_str());
            }
            // Set the initial focus when opening the combo (scrolling +
            // keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
    constexpr auto value_step = 0;
    static uint8_t cond = 0;
    static uint8_t oper = 0;
    static uint32_t insp = 0;
    ImGui::Text("New instruction");
    ImGui::InputScalar("CF", ImGuiDataType_U8, &cond, &value_step, &value_step,
                       "%lX");
    ImGui::InputScalar("OP", ImGuiDataType_U8, &oper, &value_step, &value_step,
                       "%lX");
    ImGui::InputScalar("IN", ImGuiDataType_U32, &insp, &value_step, &value_step,
                       "%lX");
    if (ImGui::Button("Add")) {
        auto const inst = Garand::GarandInstruction{
            .ConditionFlags = cond,
            .Operation = oper,
            .InstructionSpecific = insp,
        };
        asm_input.push_back(inst);
    }

    cacheMemWindow(mem);
    auto &memory = cpu.ReadMem();
    ImGui::Begin("Memory View");
    static ImGui::MemoryEditor mem_edit;
    mem_edit.DrawContents(mem.get_raw(), mem.get_size());
    mem_edit.WriteFn = [](ImU8* data, size_t off, ImU8 d) {
        mem.store(off, d);
    };
    ImGui::End();

    auto &regs = cpu.Regs();
    ImGui::Begin("Register Table");
    drawRegTable("pipdemo_reg_table", regs);
    ImGui::End();
    if (next_execution < asm_input.size()) {
        ImGui::Text("Next instruction added to queue: %s",
                    Garand::get_ins_mnemonic(asm_input[next_execution]));
    } else {
        ImGui::Text("(End of Program)");
    }
    ImGui::Text("Clock: %llu cycle(s)", cpu.ReadClock());
    ImGui::Text("Fetch:");
    ImGui::SameLine();
    if (cpu.View()[0]) {
        ImGui::Text("%s", Garand::get_ins_mnemonic(cpu.View()[0]->Instruction));
    } else {
        ImGui::Text("(Empty)");
    }
    ImGui::Text("Decode:");
    ImGui::SameLine();
    if (cpu.View()[1]) {
        ImGui::Text("%s", Garand::get_ins_mnemonic(cpu.View()[1]->Instruction));
    } else {
        ImGui::Text("(Empty)");
    }
    ImGui::Text("Execute:");
    ImGui::SameLine();
    if (cpu.View()[2]) {
        ImGui::Text("%s", Garand::get_ins_mnemonic(cpu.View()[2]->Instruction));
    } else {
        ImGui::Text("(Empty)");
    }
    ImGui::Text("Writeback:");
    ImGui::SameLine();
    if (cpu.View()[3]) {
        ImGui::Text("%s", Garand::get_ins_mnemonic(cpu.View()[3]->Instruction));
    } else {
        ImGui::Text("(Empty)");
    }

    if (ImGui::Button("Run")) {
        // TODO
    }
    ImGui::SameLine();
    if (ImGui::Button("Step")) {
        if (next_execution < asm_input.size()) {
            auto ins = asm_input[next_execution];
            cpu.Queue(ins);
            ++next_execution;
        }
        cpu.Step();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Register")) {
        cpu.ResetRegs();
    }
    if (ImGui::Button("Reset Cursor")) {
        next_execution = 0;
    }
    if (ImGui::Button("Reset Scratchpad")) {
        asm_input.clear();
        next_execution = 0;
        asm_input_idx = 0;
    }
    ImGui::End();
}

void disassemblerDemoWindow() {
    ImGui::Begin("Disassembler demo");
    static std::vector<uint8_t> mem(0x10000);
    static ImGui::MemoryEditor mem_edit;
    static std::string buffer;
    static char path[0x100];
    static bool load_success = true;
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
    ImGui::InputTextMultiline("Disassmble:\n", buffer.data(), buffer.size());
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

void debuggui() {
    static bool show_demo_window = false;
    static bool show_memory_demo_window = false;
    static bool show_instruction_demo_window = false;
    static bool show_pipeline_demo_window = false;
    static bool show_disasm_demo_window = false;
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
    // Edit bools storing our window open/close state
    ImGui::Begin("Control UI");
    ImGui::Checkbox("Demo Window", &show_demo_window);
    ImGui::Checkbox("Memory Demo", &show_memory_demo_window);
    ImGui::Checkbox("Instruction Demo", &show_instruction_demo_window);
    ImGui::Checkbox("Pipeline Demo", &show_pipeline_demo_window);
    ImGui::Checkbox("Disassembler Demo", &show_disasm_demo_window);
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

            int w = renderer.GetOutputWidth(), h = renderer.GetOutputHeight();
	        Texture sprite(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, 32, 32);
            {
                sprite.Update(NullOpt, (char *)mem.load(0x1000), 3);
            }		
            renderer.Copy(sprite, NullOpt);

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
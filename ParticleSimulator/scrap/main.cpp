#include <cuda.h>
#include <nvrtc.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>

const char* CodeFromFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Could not open code file '" << filepath << "'!\n";
        return "";
    }

    std::stringstream buf;
    buf << file.rdbuf();

    file.close();
    return buf.str().c_str();
}

void InitGPU() {
    // 1. Start ATR_FORCE
    cuInit(0);
    CUdevice device;
    cuDeviceGet(&device, 0);
    CUcontext ctx;
    cuDevicePrimaryCtxRetain(&ctx, device);
    cuCtxSetCurrent(ctx);
}

CUfunction CompileProgram(const char* code, const char* functionName) {
    // 2. Compile doubleProgram string
    nvrtcProgram prog;
    nvrtcCreateProgram(&prog, code, "k.cu", 0, nullptr, nullptr);
    const char* opts[] = { "--gpu-architecture=compute_75" };
    nvrtcCompileProgram(prog, 1, opts);

    size_t ptxSize;
    nvrtcGetPTXSize(prog, &ptxSize);
    std::string ptx(ptxSize, '\0');
    nvrtcGetPTX(prog, ptx.data());
    nvrtcDestroyProgram(&prog);

    // Keep PTX version compatible with your GPU driver
    ptx.replace(ptx.find(".version"), ptx.find(".target") - ptx.find(".version"), ".version 7.5\n");

    CUmodule module;
    CUfunction kernel;
    cuModuleLoadData(&module, ptx.c_str());
    cuModuleGetFunction(&kernel, module, functionName);

    return kernel;
}

auto LoadToGPU(void* ptr, size_t size) {
    CUdeviceptr gpuData;
    cuMemAlloc(&gpuData, size);
    cuMemcpyHtoD(gpuData, ptr, size);

    return gpuData;
}

//Runs the given function on the GPU and returns the result to varible out
void RunGPU(float* out, size_t size, CUdeviceptr gpuData, CUfunction kernel) {
    void* args[] = { &gpuData };
    cuLaunchKernel(kernel, 1, 1, 1, size, 1, 1, 0, nullptr, args, nullptr);
    cuMemcpyDtoH(out, gpuData, size);
}

void CloseMem(CUdeviceptr gpuData) {
    cuMemFree(gpuData);
}

int main() {
    InitGPU();

    CUfunction doubleProgram = CompileProgram(CodeFromFile("programs/double_program.gpu"), "doubleNums");
    CUfunction quadupleProgram = CompileProgram(CodeFromFile("programs/quaduple_program.gpu"), "quadNums");

    // 3. Copy data to GPU and run kernel
    float numbers[1000];
    for (int i = 0; i < sizeof(numbers) / sizeof(int); i++)
        numbers[i] = i;
    
    auto gpuData = LoadToGPU(numbers, sizeof(numbers));
    float output[1000];

    auto start = std::chrono::steady_clock::now();
    
    RunGPU(output, sizeof(output), gpuData, doubleProgram);
    
    // for (int i = 0; i < sizeof(output) / sizeof(int); i++)
    //     std::cout << output[i] << " ";
    // std::cout << "\n";

    std::cout << output[sizeof(output) / sizeof(float) - 1] << "\n";

    RunGPU(output, sizeof(output), gpuData, quadupleProgram);

    // for (int i = 0; i < sizeof(output) / sizeof(int); i++)
    //     std::cout << output[i] << " ";
    // std::cout << "\n";
    
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
    
    CloseMem(gpuData);
}
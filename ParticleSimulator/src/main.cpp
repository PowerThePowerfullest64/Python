#include <cuda.h>
#include <nvrtc.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstring>

const char* kernelCode = R"(
extern "C" __global__ void doubleNumbers(float* numbers) {
    int i = threadIdx.x;
    numbers[i] *= 2.0f;
}
)";

std::string CodeFromFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Could not open code file '" << filepath << "'!\n";
        return;
    }

    std::string res;

}

void InitGPU() {
    // 1. Start GPU
    cuInit(0);
    CUdevice device;
    cuDeviceGet(&device, 0);
    CUcontext ctx;
    cuDevicePrimaryCtxRetain(&ctx, device);
    cuCtxSetCurrent(ctx);
}

CUfunction CompileProgram(const char* code, const char* functionName) {
    // 2. Compile kernel string
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

//Runs the given function on the GPU and returns the result
void RunGPU(float* out, size_t size, CUdeviceptr gpuData, CUfunction kernel) {
    void* args[] = { &gpuData };
    cuLaunchKernel(kernel, 1, 1, 1, 4, 1, 1, 0, nullptr, args, nullptr);
    cuMemcpyDtoH(out, gpuData, size);
}

int main() {
    InitGPU();

    CUfunction kernel = CompileProgram(kernelCode, "doubleNumbers");

    // 3. Copy data to GPU and run kernel
    float numbers[4] = { 1, 2, 3, 4 };
    auto gpuData = LoadToGPU(numbers, sizeof(numbers));
    RunGPU(numbers, sizeof(numbers), gpuData, kernel);

    cuMemFree(gpuData);
}
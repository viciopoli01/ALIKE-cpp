// get_patches.hpp

#ifndef GET_PATCHES_CUDA_HPP
#define GET_PATCHES_CUDA_HPP

#ifdef __cplusplus
extern "C" {
#endif

// Declaration of the CUDA function (use `extern "C"` for proper linkage)
torch::Tensor get_patches_cuda(const torch::Tensor &map,
                               torch::Tensor &points,
                               int radius);
#ifdef __cplusplus
}
#endif

#endif // GET_PATCHES_CUDA_HPP

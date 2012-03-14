#ifndef VECTOROPERATIONS_H_
#define VECTOROPERATIONS_H_

#define NONE_SUPPORTED 0x00
#define SSE3_SUPPORTED 0x03
#define SSE41_SUPPORTED 0x04

#ifdef __cplusplus
	extern "C" size_t whatSse(); 
	extern "C" void add(float* a, const float *b, const size_t size);
	extern "C" void sub(float* a, const float *b, const size_t size);
	extern "C" float mulSSE3(const float* a, const float *b, const size_t size); 
	extern "C" float mulSSE41(const float* a, const float *b, const size_t size); 
#endif

#endif

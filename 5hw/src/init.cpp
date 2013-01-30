#include "init.h"

//not work in i7, i5... 
//This problem write here http://stackoverflow.com/questions/1647190/cpuid-on-intel-i7-processors
//This article get answer but i didn't read http://software.intel.com/en-us/articles/intel-64-architecture-processor-topology-enumeration/
//May be add one "if", then CPU is i*, and take to cpuid 0x0B

void cpuID(unsigned i, unsigned regs[4]) {
#ifdef _WIN32
	__cpuid((int *)regs, (int)i);
#else
	asm volatile
		("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
		: "a" (i), "c" (0));
		// ECX is set to zero for CPUID function 4
#endif
}


int get_count_processors() { 
	unsigned regs[4];

	// Get vendor
	char vendor[12];
	cpuID(0, regs);
	((unsigned *)vendor)[0] = regs[1]; // EBX
	((unsigned *)vendor)[1] = regs[3]; // EDX
	((unsigned *)vendor)[2] = regs[2]; // ECX
	std::string cpuVendor = std::string(vendor, 12);

	// Get CPU features
	cpuID(1, regs);
	unsigned cpuFeatures = regs[3]; // EDX

	// Logical core count per CPU
	cpuID(1, regs);
	unsigned int logical = (regs[1] >> 16) & 0xff; // EBX[23:16]
	std::clog << " logical cpus: " << logical << std::endl;

	unsigned int cores = logical;
	if (cpuVendor == "GenuineIntel") {
		// Get DCP cache info
		cpuID(4, regs);
		cores = ((regs[0] >> 26) & 0x3f) + 1; // EAX[31:26] + 1
	} else if (cpuVendor == "AuthenticAMD") {
		// Get NC: Number of CPU cores - 1
		cpuID(0x80000008, regs);
		cores = ((unsigned)(regs[2] & 0xff)) + 1; // ECX[7:0] + 1
	}

	std::clog << "    cpu cores: " << cores << std::endl;

	// Detect hyper-threads  
	bool hyperThreads = cpuFeatures & (1 << 28) && cores < logical;

	std::clog << "hyper-threads: " << (hyperThreads ? "true" : "false") << std::endl;

	if (hyperThreads) {
		return logical; 
	} else {
		return cores;  
	} 
}


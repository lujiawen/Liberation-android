#include <string>
#include <stdio.h>
#include <sys/mman.h>
#include <byteswap.h>
#include <unistd.h>
#include <vector>
#include <mutex>

#if defined(__i386__)
#  define PATCHSIZE 6
#elif defined(__aarch64__)
#  define PATCHSIZE 6
#elif defined(__arm__)
#  define PATCHSIZE 12
#endif

using bytes = std::vector<uint8_t>;

inline namespace Liberation
{

class Patch
{
public:
	static Patch *Setup(void* _target, char *data, size_t len);
	static Patch *Setup(void* _target, uint32_t data);
	static Patch *Setup(void* _target, std::string data);

	virtual void Apply();
	virtual void Reset();

private:
	Patch() = default;
	Patch(void* _target, char *data, size_t len);
	~Patch();

protected:
	void* _t_addr;
	size_t _patchSize;
	bytes _patchBytes;
	bytes _origBytes;
};

class Hook
{
public:
	static Hook *getInstance();
	void unhook();
	void hook(void *func, void *newaddr, void **origFunc);
	void doHook(void *func, void *newaddr, void **origFunc);
	void doUnHook(void *func, void *newaddr, void **origFunc);

protected:
	void safemepls(void *f, void *n, void **o);
	void *func; void *newAddr; void **origFunc;
	bytes _origBytes;

private:
	Hook();
	~Hook();
	static std::mutex lock;
	static bool instanceFlag;
	static Hook *instance;
};

} //Liberation
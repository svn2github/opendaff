#include <daffviz/DAFFVizGlobalLock.h>

#include <mutex>

namespace DAFFViz {

class GlobalLockImpl : public GlobalLock
{
public:
	inline void lock() 
	{ 
		m_mutex.lock();
	};

	inline void unlock()
	{ 
		m_mutex.unlock();
	};

private:
	std::mutex m_mutex;
};

GlobalLockImpl g_oGlobalLock;

GlobalLock* GlobalLock::getInstance() { return &g_oGlobalLock; }

GlobalLock::~GlobalLock() {}

} // End of namespace "FXVTK2"

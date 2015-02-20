#include <FXVTK2GlobalLock.h>

#include <fx.h>
#include <FXThread.h>

namespace FXVTK2 {

class GlobalLockImpl : public GlobalLock {
public:
	void lock() const { m_mutex.lock(); }
	void unlock() const { m_mutex.unlock(); }

private:
	mutable FXMutex m_mutex;
};

GlobalLockImpl g_oGlobalLock;

GlobalLock* GlobalLock::getInstance() { return &g_oGlobalLock; }

GlobalLock::~GlobalLock() {}

} // End of namespace "FXVTK2"

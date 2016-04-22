#include <DAFFReader.h>
#include "DAFFReaderImpl.h"

DAFFReader* DAFFReader::create()
{
	return new DAFFReaderImpl;
}

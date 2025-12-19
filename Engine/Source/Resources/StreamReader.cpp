#include "BsPrecompileHeader.h"
#include "StreamReader.h"

namespace Pixie
{
    bool StreamReader::ReadString(std::string& string)
    {
        size_t size;
        if (!ReadData((char*)&size, sizeof(size_t))) 
            return false;

        string.resize(size);
        return ReadData((char*)string.data(), sizeof(char) * size);
    }
}
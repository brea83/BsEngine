#include "BsPrecompileHeader.h"
#include "FileStream.h"

namespace Pixie
{
    //---------------------------------------------------
    // FileStreamWriter
    FileStreamWriter::FileStreamWriter(const std::filesystem::path& path, bool writeInBinary)
        : m_Path(path)
    {
        if(writeInBinary)
            m_Stream = std::ofstream(path, std::ifstream::out | std::ifstream::binary);
        else
            m_Stream = std::ofstream(path, std::ifstream::out);
    }
    
    FileStreamWriter::~FileStreamWriter()
    {
        m_Stream.close();
    }

    bool FileStreamWriter::WriteData(const char* data, size_t size)
    {
        m_Stream.write(data, size);
        return true;
    }

    //---------------------------------------------------
    // FileStreamReader

    FileStreamReader::FileStreamReader(const std::filesystem::path& path, bool writeInBinary)
        : m_Path(path)
    {
        if(writeInBinary)
            m_Stream = std::ifstream(path, std::ifstream::in | std::ifstream::binary);
        else
            m_Stream = std::ifstream(path, std::ifstream::in);
    }

    FileStreamReader::~FileStreamReader()
    {
        m_Stream.close();
    }

    bool FileStreamReader::ReadData(char* destination, size_t size)
    {
        m_Stream.read(destination, size);
        return true;
    }
}
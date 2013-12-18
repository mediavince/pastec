#ifndef BACKWARDINDEXREADERACCESS_H
#define BACKWARDINDEXREADERACCESS_H

#include <sys/types.h>

#include <cstring>

#include <iostream>
#include <string>
#include <fstream>


using namespace std;


class BackwardIndexReaderAccess
{
public:
    virtual ~BackwardIndexReaderAccess() {}
    virtual bool open(string indexPath) = 0;
    virtual void moveAt(u_int64_t pos) = 0;
    virtual void read(char *p_data, unsigned i_nbBytes) = 0;
    virtual bool endOfIndex() = 0;
    virtual void reset() = 0;
    virtual void close() = 0;
};


class BackwardIndexReaderFileAccess : public BackwardIndexReaderAccess
{
public:
    virtual bool open(string indexPath)
    {
        ifs.open(indexPath.c_str(), ios_base::binary);
        if (!ifs.good())
            return false;
        return true;
    }

    virtual void moveAt(u_int64_t pos)
    {
        ifs.seekg(pos);
    }

    virtual void read(char *p_data, unsigned i_nbBytes)
    {
        ifs.read(p_data, i_nbBytes);
    }

    virtual bool endOfIndex()
    {
        return ifs.eof();
    }

    virtual void reset()
    {
        ifs.clear();
    }

    virtual void close()
    {
        ifs.close();
    }

private:
    ifstream ifs;
};


class BackwardIndexReaderMemAccess : public BackwardIndexReaderAccess
{
public:
    virtual bool open(string indexPath)
    {
        ifstream ifs;
        ifs.open(indexPath.c_str(), ios_base::binary);
        if (!ifs.good())
        {
            ifs.close();
            return false;
        }

        ifs.seekg(0, std::ifstream::end);
        i_fileSize = ifs.tellg();

        // Try to allocate the needed size for the entire file.
        p_indexData = new char[i_fileSize];
        if (p_indexData == NULL)
        {
            cout << "Couldn't allocate the space to store the backward index file in memory." << endl;
            ifs.close();
            return false;
        }
        ifs.clear();

        // Copy the file in memory.
        u_int64_t i = 0;
        while(!ifs.eof())
        {
            ifs.read(p_indexData + i, 1);
            i++;
        }

        i_curPos = 0;

        ifs.close();
        return true;
    }

    virtual void moveAt(u_int64_t pos)
    {
        i_curPos = pos;
    }

    virtual void read(char *p_data, unsigned i_nbBytes)
    {
        memcpy(p_data, p_indexData + i_curPos, i_nbBytes);
        i_curPos += i_nbBytes;
    }

    virtual bool endOfIndex()
    {
        return i_curPos == i_fileSize;
    }

    virtual void reset()
    {
        i_curPos = 0;
    }

    virtual void close()
    {
        delete p_indexData;
    }

private:
    char *p_indexData;
    u_int64_t i_fileSize;
    u_int64_t i_curPos;
};

#endif // BACKWARDINDEXREADERACCESS_H
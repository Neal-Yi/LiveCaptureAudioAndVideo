#ifndef LIVEBUFFER_H
#define LIVEBUFFER_H

#include "buffer.h"


class liveBuffer
{
    public:
        liveBuffer();
        virtual ~liveBuffer();
        Buffer* audioBuf;
        Buffer* videoBuf;
        bool    isLive;
        bool    init(int audioBufferSize, int videoBufferSize);
        void    init(unsigned char* abuf, int audioBufferSize,unsigned char* vbuf, int videoBufferSize );
		void	close();
    protected:
    private:
		bool createBufferBySelf;
        bool initialized;
};

#endif // LIVEBUFFER_H

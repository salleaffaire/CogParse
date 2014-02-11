
#include "cg_bit_extractor.hpp"


cg_bit_buffer::cg_bit_buffer()
{
   mContextBitPos     = 0;
   mContextBytePos    = 0;
   mBuffer            = 0;
   
   mNumBytes          = 0;    //  Total number of bytes in the buffer 
   mLastBitPos        = 0;    //  Bit position of the last valid data bit in the buffer  
}

cg_bit_buffer::~cg_bit_buffer() {}

void cg_bit_buffer::InitBuffer(unsigned char *Buffer, unsigned int NumBytes)
{
   mBuffer            = Buffer;
   mNumBytes          = NumBytes;
   mContextBitPos     = 0;
   mContextBytePos    = 0;
   mLastBitPos        = 0; 
}

void cg_bit_buffer::InitBuffer(cg_byte_stream *ByteStream)
{
   mBuffer            = ByteStream->mpByteStream;
   mNumBytes          = ByteStream->mByteCount;
   mContextBitPos     = 0;
   mContextBytePos    = 0;     
   mLastBitPos        = 0; 
}

// Max is 24-bit
unsigned int cg_bit_buffer::GetBits(unsigned char n)
{
   unsigned int ulValue;
   
   ulValue = cg_bit_buffer::ShowBits(n);
   cg_bit_buffer::FlushBits(n);
   
   return ulValue;
}

void  cg_bit_buffer::FlushBits(unsigned char n)
{
   unsigned char ucNewValue = (unsigned char) (mContextBitPos + n);

   mContextBitPos = (unsigned char)(ucNewValue & 7);

   if(ucNewValue > 7)
   {
      mContextBytePos = (unsigned int)(mContextBytePos + (ucNewValue >> 3));
   }
}

unsigned int cg_bit_buffer::ShowBits(unsigned char n)
{

   register unsigned int inf;
   unsigned char*	buff	= &(mBuffer[mContextBytePos]);

   inf  = *(buff)   << 24;
   inf |= *(buff+1) << 16;
   inf |= *(buff+2) << 8;

	return ((inf<<mContextBitPos) >> (32-n));
}



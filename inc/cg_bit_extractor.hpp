
#ifndef RBSP_BITEXTRACTOR_HPP___
#define RBSP_BITEXTRACTOR_HPP___

class cg_byte_stream
{
public:
	unsigned char *mpByteStream;
	unsigned int   mByteCount;
};


class cg_bit_buffer
{
public:
   cg_bit_buffer();
   ~cg_bit_buffer();

   void           InitBuffer(unsigned char *InputBuffer, unsigned int NumBytes);
   void           InitBuffer(cg_byte_stream *cg_byte_stream);

   unsigned int   GetBits(unsigned char n);
   void           FlushBits(unsigned char n);
   unsigned int   ShowBits(unsigned char n);

private:
   unsigned char *mBuffer;
   unsigned int   mNumBytes;   //  Total number of bytes in the buffer 
   
   unsigned char  mContextBitPos;
   unsigned int   mContextBytePos;
   unsigned char  mLastBitPos; //  Bit position of the last valid data bit in the buffer
};

class cg_bit_code
{
public:
   
   
       
};

#endif

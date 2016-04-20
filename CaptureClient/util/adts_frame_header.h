#ifndef YXB_ADTS_FRAME_HEADER
#define YXB_ADTS_FRAME_HEADER
#define ADTS_MAX_FRAME_BYTES  ((1 << 13) - 1)
#define MAX_PCE_SIZE  320
#define ADTS_HEADER_SIZE   7
int adtsFrameHeader(void* priv_data, uint8_t* header, int pktSize);
typedef struct ADTSContext {
    AVClass *avclass;
    int write_adts;
    int objecttype;
    int sample_rate_index;
    int channel_conf;
    int pce_size;
    int apetag;
    int id3v2tag;
    uint8_t pce_data[MAX_PCE_SIZE];
} ADTSContext;
#endif
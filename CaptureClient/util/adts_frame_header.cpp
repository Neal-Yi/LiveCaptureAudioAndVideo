#include "ffmpeg.h"
#include "adts_frame_header.h"

int adts_write_frame_header(ADTSContext *ctx,
                                   uint8_t *header, int size, int pce_size)
{

    unsigned full_frame_size = (unsigned)ADTS_HEADER_SIZE + size + pce_size;
    if (full_frame_size > ADTS_MAX_FRAME_BYTES) {
    	fprintf(stderr, "the adts frame size is too big\n");
        return AVERROR_INVALIDDATA;
    }
    uint16_t t;
    /* adts_fixed_header */
    // put_bits(&pb, 12, 0xfff);   /* syncword */
    // put_bits(&pb, 1, 0);        /* ID */
    // put_bits(&pb, 2, 0);        /* layer */
    // put_bits(&pb, 1, 1);        /* protection_absent */
    header[0] = 0xff;
    header[1] = 0xf1;
    // put_bits(&pb, 2, ctx->objecttype); /* profile_objecttype */
    // put_bits(&pb, 4, ctx->sample_rate_index);
    // put_bits(&pb, 1, 0);        /* private_bit */
    // put_bits(&pb, 3, ctx->channel_conf); /* channel_configuration */
    header[2] = ctx->objecttype << 6;
    header[2] |= ctx->sample_rate_index << 2;
    header[2] |= ctx->channel_conf>> (3 -2);
    header[3] = ctx->channel_conf << 6;  
    // put_bits(&pb, 1, 0);         //original_copy 
    // put_bits(&pb, 1, 0);        /* home */
    /* adts_variable_header */
    // put_bits(&pb, 1, 0);        /* copyright_identification_bit */
    // put_bits(&pb, 1, 0);        // copyright_identification_start 
    // put_bits(&pb, 13, full_frame_size); /* aac_frame_length */
    header[3] |= full_frame_size >>(13 - 2);
    header[4] = full_frame_size >>(13 - 2 - 8);
    header[5] = full_frame_size <<(8 - 3);
	header[5] = 0x7ff >>(11 - 5);
    header[6] = 0x7ff << ( 8 - 6); 
    // put_bits(&pb, 11, 0x7ff);   /* adts_buffer_fullness */

    // put_bits(&pb, 2, 0);        /* number_of_raw_data_blocks_in_frame */

    // flush_put_bits(&pb);
    return 0;
}
int adtsFrameHeader(void* priv_data, uint8_t* header, int pktSize){

    ADTSContext* adts =(ADTSContext*)priv_data;
    return adts_write_frame_header(adts, header, pktSize, adts->pce_size); 

}
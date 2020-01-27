
#define AD_HOC_DIMS_MAX 2
#define AD_HOC_NESTED_MAX 1
#define AD_HOC_ID_BYTES 1

#define RBUF_INIT(TYPE, LENPOW2 ) volatile struct { \
						uint32_t wr; \
						uint32_t rd; \
						TYPE data[ 1 << (LENPOW2)]; \
						}
#define RBUF_PUT( RBUF, INN)  \
						{ if((RBUF).rd == (RBUF).wr ) { \
						(RBUF).wr = 0; \
						(RBUF).rd = 0; \
						} \
						(RBUF).data[ RBUF_MASK(RBUF) & (RBUF).wr++] = INN; }

#define RBUF_MASK(RBUF) ( sizeof((RBUF).data)/sizeof((RBUF).data[0]) -1)
#define RBUF_ISEMPTY(RBUF) ((RBUF).rd == (RBUF).wr)
#define RBUF_ISFULL(RBUF) ((RBUF).rd + RBUF_MASK(RBUF) == (RBUF).wr)
#define RBUF_GET( RBUF)  (RBUF).data[ RBUF_MASK(RBUF) & ((RBUF).rd++)]

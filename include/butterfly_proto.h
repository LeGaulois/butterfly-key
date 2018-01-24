#ifndef BUTTERFLY_PROTO_H
#define BUTTERFLY_PROTO_H


#include <stdint.h>
#include "list.h"
#include "device.h"

/*Type de requete butterfly*/
#define BPROTO_TYPE_QUERY 		0
#define BPROTO_TYPE_RESPONSE 	1


/**
 * Type d'éléments butterfly
 */
#define BPROTO_TLV_TYPE_END             0x00
#define BPROTO_TLV_TYPE_EK              0x01
#define BPROTO_TLV_TYPE_CK              0x02
#define BPROTO_TLV_TYPE_F1_PUBLIC_KEY   0x03
#define BPROTO_TLV_TYPE_F2_PUBLIC_KEY   0x04
#define BPROTO_TLV_TYPE_CERTIFICATE     0x05

#define BPROTO_HEADER_FIX_LENGTH 		8
#define TLV_FIX_LENGTH    				3

typedef struct s_bproto_header 	s_bproto_header;
typedef struct s_bproto_tlv 	s_bproto_tlv;


struct s_bproto_tlv {
    uint8_t         type;
    uint16_t        length;
    unsigned char   *value;
};


struct s_bproto_header {
    uint16_t    id;
    uint8_t     type;
    uint8_t     status;
    uint32_t    nbtlv;
    
    list        *list_tlv;    
};


s_bproto_header* bproto_header_init(void);
int bproto_header_free(s_bproto_header **bp_h);
int bproto_header_add_tlv(s_bproto_header *b_ph, s_bproto_tlv *tlv);
s_bproto_tlv* bproto_tlv_init(void);
int bproto_tlv_free(void **tlv);
int bproto_tlv_compare(void *tlv1, void *tlv2);
size_t bproto_get_size(const s_bproto_header *bp_h);
size_t bproto_get_list_tlv_size(const s_bproto_header *bp_h);


size_t bproto_to_char(const s_bproto_header *bp_h, 
				unsigned char **buffer);
size_t bproto_header_to_char(const s_bproto_header *bp_h,
					unsigned char **buffer);
size_t bproto_list_tlv_to_char(const s_bproto_header *bp_h, 
					unsigned char **buffer);
size_t bproto_tlv_to_char(s_bproto_tlv *bp_tlv, 
					unsigned char **buffer);
					

s_bproto_header* bdevice_prepare_request_to_server(
										t_bdevice *bd);		
s_bproto_tlv* bdevice_ek_to_tlv(t_bdevice *bd);
s_bproto_tlv* bdevice_ck_to_tlv(t_bdevice *bd);
s_bproto_tlv* bdevice_f1_public_to_tlv(t_bdevice *bd);
s_bproto_tlv* bdevice_f2_public_to_tlv(t_bdevice *bd);


s_bproto_tlv* string_to_bproto_tlv(unsigned char *str_tlv,
							size_t len_str_list_tlv);
int string_to_bproto_tlv_list(unsigned char *str_list_tlv,
				size_t len_str_list_tlv, s_bproto_header *bp_h);
s_bproto_header* string_to_bproto_header(unsigned char *str, size_t len_str);
void bproto_header_print(const s_bproto_header *bp_h);
#endif

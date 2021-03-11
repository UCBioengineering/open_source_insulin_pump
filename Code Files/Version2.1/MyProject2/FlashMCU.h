#ifndef	FLASHMCU_H_INCLUDED
#define FLASHMCU_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void store_delivery_data(void);

void get_delivery_data(void);

void split_to_8(void);

void join_to_16(void);

void check_writes(void);

void flash_initialise(void);


#ifdef __cplusplus
}
#endif
#endif
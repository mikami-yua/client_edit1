#pragma once
#ifndef OT_H
#define OT_H

#include<openssl/rsa.h>
#include"im_client.h"
int ot_send_rsa_msg(char* get_n, char* get_e, char* get_d);

int ot_send_rand_msg(char* msg1, char* msg2);

int ot_recv_compute_v(char* v, char* r_k, char* n, char* e, char* msg1, char* msg2);

int ot_compute_ki_msg(char* k_msg1, char* k_msg2, char* v, char* msg1, char* msg2, char* d, char* n);

int ot_encode_msg(char* en_msg1, char* en_msg2, char* k1, char* k2, char* msg1, char* msg2);

int ot_decode_msg(char* de_msg1, char* de_msg2, char* en_msg1, char* en_msg2, char* k);
#endif // !OT_H

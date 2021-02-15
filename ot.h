#pragma once
#ifndef OT_H
#define OT_H

#include<openssl/rsa.h>
#include"im_client.h"
int ot_send_rsa_msg(char* n, char* e);

int ot_send_rand_msg(char *msg1,char *msg2);

int ot_recv_compute_v(char* v);

int ot_send_ki_msg(char* k_msg1, char* k_msg2);

int ot_decode_msg();
#endif // !OT_H

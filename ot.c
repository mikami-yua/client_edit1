#include"ot.h"

/*
生成RSA的N E
@get_n:生成的N
@get_e：生成的E
@get_d:生成的D
*/
int ot_send_rsa_msg(char *get_n,char *get_e,char *get_d) {
	BIGNUM* p, * q, * e, * N, * d, * m, * c, * mm, * mul_ppqq;
	BN_CTX* ctx = BN_CTX_new();//上下文
	BIGNUM* pp, * qq, * one, * gcd;//pp是p-1，qq是q-1，one等于1，gcd为最大公约数
	p = BN_new();
	q = BN_new();
	e = BN_new();
	N = BN_new();
	pp = BN_new();
	qq = BN_new();
	one = BN_new();
	mul_ppqq = BN_new();//这个数为 (p-1)*(q-1)
	gcd = BN_new();
	BN_one(one);
	char* ch_p, * ch_q, * ch_N, * ch_e,*ch_d;
	BN_generate_prime(p, 50, NULL, NULL, NULL, NULL, NULL);//产生一个素数，赋给p
	BN_generate_prime(q, 50, NULL, NULL, NULL, NULL, NULL);//产生一个素数，赋给q

	ch_p = BN_bn2dec(p);//将BIGNUM类型转换为char *，再转换为CString，输出到界面
	printf("%s\n", ch_p);

	ch_q = BN_bn2dec(q);//同上
	printf("%s\n", ch_q);

	BN_mul(N, p, q, ctx);//N=p*q


	ch_N = BN_bn2dec(N);
	printf("n:%s\n", ch_N);
	strcpy(get_n, ch_N);

	BN_sub(pp, p, one);//pp = p -1
	BN_sub(qq, q, one);//qq = q- 1

	BN_mul(mul_ppqq, pp, qq, ctx);//mul_ppqq = (p-1)*(q-1)

	BN_rand(e, 8, 0, 0);//产生一个随机数赋值给e e要尽量小
	BN_gcd(gcd, e, mul_ppqq, ctx);//求e和(p-1)(q-1)的最大公约数

	while (BN_is_one(gcd) != 1)//如果gcd不为1，一直循环，直到产生一个与(p-1)*(q-1)互质的e
	{
		BN_rand(e, 8, 0, 0);
		BN_gcd(gcd, e, mul_ppqq, ctx);
	}

	ch_e = BN_bn2dec(e);
	printf("e:%s\n", ch_e);
	strcpy(get_e, ch_e);

	//求d
	//BN_mod_inverse(d, e, mul_ppqq, ctx);//求e 模 (p-1)*(q-1) 的逆元，赋给d
	
	d = BN_new();
	BN_mod_inverse(d, e, mul_ppqq, ctx);
	ch_d = BN_bn2dec(d);
	printf("d:%s\n", ch_d);
	strcpy(get_d, ch_d);
	BN_CTX_free(ctx);//上下文
}

/*
发送方生成两个随机消息 128bit
@msg1:生成的随机消息1
@msg2:生成的随机消息2
*/
int ot_send_rand_msg(char* msg1, char* msg2) {
	BIGNUM* rnd;
	rnd = BN_new();
	char *show;
	//BN_random
	int bits =128;
	int top = 0;
	int bottom = 0;
	//测试top = -1
	BN_rand(rnd, bits, top, bottom);
	show = BN_bn2dec(rnd);
	strcpy(msg1, show);
	BN_rand(rnd, bits, top, bottom);
	show = BN_bn2dec(rnd);
	strcpy(msg2, show);
	BN_free(rnd);
	/*
	BN_rand(BIGNUM *rnd,int bits,int top,int bottom):生成一个强密码学随机函数，是一个无法预测的随机数。
	rnd:需要生成的随机数，bits：需要生成随机数的位数，top:为设置最高位的值，top=-1表示最高位设置为0,top=0表示最高位设置为1,top=1时，最高两位都设置成1，
	bottom:不为0时，可以生成奇的随机数。
	*/
}

/*
接收方计算v  char *msg1,char *msg2,char *n,char *e  BN函数使用说明：https://blog.csdn.net/jnxxhzz/article/details/81235981
接收方生成一个随机值k，根据字节0、1选择选择用随机消息1计算还是使用随机消息2计算
@v：返回的最后生成的随机v，需要返回发送方
@r_k:返回的生成的随机数k
@n:RSA生成的N
@e：RSA生成的E
@msg1:随机消息1
@msg2：随机消息2
*/
int ot_recv_compute_v(char *v,char *r_k,char * n,char *e,char *msg1,char *msg2) {
	int choose = 0;//这里使用define定义是0还是1

	//int BN_dec2bn(BIGNUM * *a, const char* str);	将十进制字符串转换为大数
	BIGNUM* k, * r1, * r2, * N, *E;
	BN_CTX* ctx = BN_CTX_new();//上下文
	BIGNUM* ke, * rke;
	/*
	*************************************RSA******************************
	997968194927129
	1099880031894913
	n:1097645290066559398487820794777
	e:161
	d:299977594800798268497777234785
	1097645290066559398487820794777
	161
	******************************RANDOM MSG******************************
	261710562662361005308106670777051919343
	290584205874689139095264735403588360371
	*/
	//char* msg1 = "261710562662361005308106670777051919343";
	//char* msg2 = "290584205874689139095264735403588360371";
	//char* n = "1097645290066559398487820794777";
	//char* e = "161";

	k = BN_new();
	r1 = BN_new();
	r2 = BN_new();
	N = BN_new();
	E = BN_new();
	ke = BN_new();
	rke = BN_new();
	BN_dec2bn(&r1, msg1);//成功转化
	BN_dec2bn(&r2, msg2);
	BN_dec2bn(&N, n);
	BN_dec2bn(&E, e);
	BN_rand(k,64, 0, 0);//k为随机生成的64位数字

	char* rand_k;
	rand_k = BN_bn2dec(k);
	strcpy(r_k, rand_k);

	//int BN_exp(BIGNUM *r, BIGNUM *a, BIGNUM *p, BN_CTX *ctx);	计算a的p次方，值储存在r中, r = a ^ p 如果成功返回1, 否则返回0
	BN_exp(ke, k, E, ctx);
	//int BN_mod_add(BIGNUM *r, BIGNUM *a, BIGNUM *b, const BIGNUM *m, BN_CTX *ctx);计算a与b的和，再模m，值储存在r中, r = (a + b) % m如果成功返回1, 否则返回0
	BN_mod_add(rke, r1, ke, N, ctx);//r1还是r2根据choose选择

	char* ans;
	ans = BN_bn2dec(rke);
	strcpy(v, ans);
	BN_CTX_free(ctx);
}

/*
计算ki 可能会因为v的值过大产生影响 考虑：不需要使用128位的rsa，（OT128 和rsa的密钥长度之间是否有关，需要仔细考虑）
@k_msg1:返回的根据v和第一个随机消息计算的k1
@k_msg2:返回的根据v和第二个随机消息计算的k2
@v:收到接收方的v
@msg1：随机消息1
@msg2：随机消息2
@d：RSA生成的D
@N：RSA生成的N
*/
int ot_compute_ki_msg(char *k_msg1,char *k_msg2,char *v,char *msg1,char *msg2,char *d,char *n) {
	BIGNUM* k1, * k2,*V,*MSG1,*MSG2,*D,*N;
	BN_CTX* ctx = BN_CTX_new();//上下文
	BIGNUM* kk1, * kk2;//存放中间结果
	//char* v, * msg1, * msg2;//v是接收方返回的v，msg1，msg2是本地生成的随机消息
	//char* v = "413506832435888678249531951641";
	//char* msg1 = "261710562662361005308106670777051919343";
	//char* msg2 = "290584205874689139095264735403588360371";
	//char* d = "299977594800798268497777234785";
	//char* n = "1097645290066559398487820794777";
	/*
	*************************************RSA******************************
	997968194927129
	1099880031894913
	n:1097645290066559398487820794777
	e:161
	d:299977594800798268497777234785
	1097645290066559398487820794777
	161
	******************************RANDOM MSG******************************
	261710562662361005308106670777051919343
	290584205874689139095264735403588360371
	******************************compute v******************************
	random k:15583185683416052835
	413506832435888678249531951641
	*/
	k1 = BN_new();
	k2 = BN_new();
	V = BN_new();
	MSG1 = BN_new();
	MSG2 = BN_new();
	D = BN_new();
	N = BN_new();
	kk1 = BN_new();
	kk2 = BN_new();
	BN_dec2bn(&V,v);//成功转化
	BN_dec2bn(&MSG1,msg1);
	BN_dec2bn(&MSG2,msg2);
	BN_dec2bn(&N, n);
	BN_dec2bn(&D, d);
	BN_sub(kk1, V, MSG1);
	BN_sub(kk2, V, MSG2);
	//int BN_mod_exp(BIGNUM *r, BIGNUM *a, const BIGNUM *p, const BIGNUM *m, BN_CTX *ctx);	计算a的p次方，再模m，值储存在r中, r = (a ^ p) % m如果成功返回1, 否则返回0
	BN_mod_exp(k1, kk1, D, N,ctx);
	BN_mod_exp(k2, kk2, D, N,ctx);
	char* ans1;
	char* ans2;//存放最终结果ki
	ans1 = BN_bn2dec(k1);
	ans2 = BN_bn2dec(k2);
	strcpy(k_msg1,ans1);
	strcpy(k_msg2, ans2);
	BN_CTX_free(ctx);
}

/*
加密真实信息，使用ot_send_ki_msg得到的k_msg
@en_msg1:返回的第一个秘密的消息
@en_msg2:返回的第二个秘密的消息
@k1：计算得出的第一个k
@k2：计算得出的第二个k
@msg1：真实的消息1
@msg2：真实的消息2
【注意】：长度的问题需要以后解决，这里token的长度和ot的长度都是128位即16个char类型。导致实际需要的ki也只需要16个长度即可
不需要把消息的真实长度也一并返回，strlen可计算
*/
int ot_encode_msg(char *en_msg1,char* en_msg2,char *k1,char *k2,char *msg1,char *msg2) {
	//1.第一步：把一个string转换为数字
	//1.转为16进制，转为10进制
	//char* msg1 = "abcdefghijklmn";
	//char* msg2 = "opqrstuvwxyz";
	//printf("ascii code %c\n", msg1[1]+1);//char可以直接加减1
	//printf("strlen() function %d\n", strlen(msg1));//strlen函数可以获得真实长度
	int len1 = strlen(msg1);
	int len2 = strlen(msg2);
	//char* k1, * k2;
	/*
	******************************compute ki******************************
	15583185683416052835
	754543401445420028359943540951
	*/
	//char* k1 = "15583185683416052835";
	//char* k2 = "754543401445420028359943540951";
	char ans1[100];
	char ans2[100];
	memset(ans1, 0, sizeof(ans1));
	memset(ans2, 0, sizeof(ans2));
	for (int i = 0; i < len1; i++) {
		ans1[i] = msg1[i]+k1[i];
	}
	for (int i = 0; i < len2; i++) {
		ans2[i] = msg2[i]+k2[i];
	}
	strcpy(en_msg1, ans1);
	strcpy(en_msg2, ans2);
	//枡晻灊洘洖灎Γ
	//¨ⅸぇ
}

/*
解密消息,接收方根据自己的k解密
@de_msg1:返回的解密消息1
@de_msg2:返回的解密消息2
@en_msg1：加密消息1
@en_msg2：加密消息2
@k:生成的k（计算v的时候生成）
*/
int ot_decode_msg(char *de_msg1,char *de_msg2,char *en_msg1,char* en_msg2,char *k) {
	//char* en_msg1 = "枡晻灊洘洖灎Γ";
	//char* en_msg2 = "¨ⅸぇ";
	//char* k1 = "15583185683416052835";
	//char* k2 = "754543401445420028359943540951";
	char ans1[100];
	char ans2[100];
	int len1 = strlen(en_msg1);
	int len2 = strlen(en_msg2);
	printf("len    %d\n", len1);
	memset(ans1, 0, sizeof(ans1));
	memset(ans2, 0, sizeof(ans2));
	for (int i = 0; i < len1; i++) {
		ans1[i] = en_msg1[i] - k[i];
	}
	for (int i = 0; i < len2; i++) {
		ans2[i] = en_msg2[i] - k[i];
	}
	strcpy(de_msg1, ans1);
	strcpy(de_msg2, ans2);
}




/*验证
*************************************RSA******************************
997968194927129
1099880031894913
n:1097645290066559398487820794777
e:161
d:299977594800798268497777234785
1097645290066559398487820794777
161
******************************RANDOM MSG******************************
261710562662361005308106670777051919343
290584205874689139095264735403588360371
******************************compute v******************************
random k:15583185683416052835
413506832435888678249531951641
******************************compute ki******************************
15583185683416052835
754543401445420028359943540951
100******************************msg encode******************************
挆槣槜煗煝灎灓
ΕェЁ│ì
******************************msg decode******************************
len    14
abcdefghijklmn
opqrstuvwxyz


*/
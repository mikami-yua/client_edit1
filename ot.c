#include"ot.h"

/*
生成RSA的N E
*/
int ot_send_rsa_msg(char *get_n,char *get_e) {
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
	BN_CTX_free(ctx);//上下文
}

/*
发送方生成两个随机消息 128bit
BN_rand(BIGNUM *rnd,int bits,int top,int bottom):生成一个强密码学随机函数，是一个无法预测的随机数。
rnd:需要生成的随机数，bits：需要生成随机数的位数，top:为设置最高位的值，top=-1表示最高位设置为0,top=0表示最高位设置为1,top=1时，最高两位都设置成1，
bottom:不为0时，可以生成奇的随机数。
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
}

/*
接收方计算v  char *msg1,char *msg2,char *n,char *e
BN函数使用说明：https://blog.csdn.net/jnxxhzz/article/details/81235981
*/
int ot_recv_compute_v(char *v) {
	int choose = 0;//这里使用define定义是0还是1

	//int BN_dec2bn(BIGNUM * *a, const char* str);	将十进制字符串转换为大数
	BIGNUM* k, * r1, * r2, * N, *E;
	BN_CTX* ctx = BN_CTX_new();//上下文
	BIGNUM* ke, * rke;
	/*
	*************************************RSA******************************
	1036984106178649
	1002262634640097
	n:1039330422338518836714100688953
	e:621235219395667
	1039330422338518836714100688953
	621235219395667
	******************************RANDOM MSG******************************
	227572743194070730677249100196652847482
	332306341904421406533589007695289570300
	*/
	char* msg1 = "227572743194070730677249100196652847482";
	char* msg2 = "332306341904421406533589007695289570300";
	char* n = "1039330422338518836714100688953";
	char* e = "191";

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
	BN_rand(k, 32, 0, 0);//k为随机生成的32位数字

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
计算ki 可能会因为v的值过大产生影响
考虑：不需要使用128位的rsa，（OT128 和rsa的密钥长度之间是否有关，需要仔细考虑）

*/
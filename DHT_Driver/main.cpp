#include <pi_dht_read.h>

int main() {
	float t, h;
	int res =  pi_dht_read(22, 7, &t, &h);
	
}

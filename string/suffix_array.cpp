const int MAXN = 51000;
struct Sfx {
	int i, key[2];
	bool operator < (const Sfx& s) const {
		return key[0] == s.key[0] ? key[1] < s.key[1] : key[0] < s.key[0];
	}
} sfx[MAXN], temp[MAXN];// _rank from 0 to n - 1
int _rank[MAXN], bucket[MAXN], height[MAXN], RMQ[MAXN][20];
//radixSort，first key[1] then key[0]
void radixSort(Sfx* in, int n, int idx, Sfx* out) {
	memset(bucket, 0, sizeof(int) * (n + 1));
	for (int i = 0; i < n; i++)
		bucket[in[i].key[idx]]++;
	for (int i = 1; i <= n; i++)
		bucket[i] += bucket[i - 1];
	for (int i = n - 1; i >= 0; i--)//for down
		out[--bucket[in[i].key[idx]]] = in[i];
}/*text can't contain 0, 0 is used as terminal*/
void buildSA(const int* text, int n) {
	for (int i = 0; i < n; i++) {
		sfx[i].i = i;
		sfx[i].key[0] = text[i];
	}
	sort(sfx, sfx + n);
	for (int i = 0; i < n; i++)
		sfx[i].key[1] = 0;
	int wid = 1;
	while (wid < n) {
		_rank[sfx[0].i] = 0;
		for (int i = 1; i < n; i++)
			_rank[sfx[i].i] = _rank[sfx[i - 1].i] + (sfx[i - 1] < sfx[i]);
		for (int i = 0; i < n; i++) {
			sfx[i].i = i;
			sfx[i].key[0] = _rank[i];
			sfx[i].key[1] = i + wid < n ? _rank[i + wid]: 0;
		}
		radixSort(sfx, n, 1, temp);
		radixSort(temp, n, 0, sfx);
		wid <<= 1;
	}
}//height[i] = lcp(suffix(sa[i - 1]), suffix[sa[i]])
void calHeight(const int* text, int* _rank, int n) {
	for(int i = 0; i < n; i++)
		_rank[sfx[i].i] = i;
	height[0] = 0;
	for(int i = 0, k = 0, j; i < n; i++) {
		if(_rank[i] != 0) {
			if(k > 0) k-- ;
			for (j = sfx[_rank[i] - 1].i; text[i + k] == text[j + k]; k++);
			height[_rank[i]] = k;
		}
	}
}//n = len(text),height[0] means nothing
void buildRMQ(int n, int* height) {
	for(int i = 1; i <= n; i++) RMQ[i][0] = height[i - 1];
	for (int j = 1; j <= log(n + 0.00) / log(2.0); j++)
		for (int i = 1; i + (1 << j) - 1 <= n; i++)
			RMQ[i][j] = min(RMQ[i][j - 1], RMQ[i + (1 << (j - 1))][j - 1]);
}
int queryRMQ(int a, int b) {
	int len = log(b - a + 1.0) / log(2.0);
	return min(RMQ[a][len], RMQ[b - (1 << len) + 1][len]);
}
int queryLCP(int a, int b) {//a, b is 0-based
	a = _rank[a] + 1;
	b = _rank[b] + 1;
	if(a > b) swap(a, b);
	return queryRMQ(a + 1, b);
}

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

int N, M, n;

vector <int> p;
vector < vector <bool> > hwall, vwall;

int find(int x)
{
	return p[x] ^ x ? p[x] = find(p[x]) : x;
}

void uni(int x, int y)
{
	p[find(y)] = p[find(x)];
}

int main(void)
{
	freopen("maze.maz", "w", stdout);

	scanf("%d%d", &M, &N);
	
	p.resize((N * 2) * (M * 2) + 10, 0);
	hwall.resize(N * 2 + 10, vector <bool> (M * 2 + 10, 0));
	vwall.resize(N * 2 + 10, vector <bool> (M * 2 + 10, 0));

	srand(time(NULL));

	n = M;

	for (int i = 0; i <= N + 1; i++) {
		for (int j = 0; j <= M + 1; j++) {
			p[n * i + j] = n * i + j;
		}
	}

	for (int i = 1; i < M; i++) {
		if (rand() % 2 && find(n + i) != find(n + i + 1)) {
			uni(n + i, n + i + 1);
			hwall[1][i] = 1;
		}
	}

	for (int i = 1; i < N; i++) {
		int flag = 0;
		for (int j = 1; j <= M; j++) {
			if ((!flag || rand() % 2) && find(i * n + j) != find((i + 1) * n + j)) {
				vwall[i][j] = 1;
				uni(i * n + j, (i + 1) * n + j);
			}
			flag = !(find(i * n + j) != find(i * n + j + 1));
		}
		for (int j = 1; j < M; j++) {
			if (rand() % 2 && find((i + 1) * n + j) != find((i + 1) * n + j + 1)) {
				uni((i + 1) * n + j, (i + 1) * n + j + 1);
				hwall[i + 1][j] = 1;
			}
		}
	}

	for (int i = 1; i < M; i++) {
		if (find(N * n + i) != find(N * n + i + 1)) {
			uni(N * n + i, N * n + i + 1);
			hwall[N][i] = 1;
		}
	}

	for (int cnt = (N < M ? N : M) / 2; cnt > 0; ) {
		int removed = 0;
		if (rand() & 1) {
			for (int i = 1; i <= N && !removed; i++) {
				for (int j = 1; j < M && !removed; j++) {
					if ((rand() & 1) && !hwall[i][j]) {
						hwall[i][j] = 1;
						removed = 1;
					}
				}
			}
		} else {
			for (int i = 1; i < N && !removed; i++) {
				for (int j = 1; j <= M && !removed; j++) {
					if ((rand() & 1) && !vwall[i][j]) {
						vwall[i][j] = 1;
						removed = 1;
					}
				}
			}
		}
		cnt -= removed;
	}

	printf("%d %d\n", N, M);

	putchar('+');
	for (int i = 1; i <= M; i++) {
		putchar('-');
		putchar('+');
	}
	puts("");

	for (int i = 1; i <= N; i++) {
		putchar('|');
		for (int j = 1; j <= M; j++) {
			putchar(' ');
			putchar(hwall[i][j] ? ' ' : '|');
		}
		puts("");
		putchar('+');
		for (int j = 1; j <= M; j++) {
			putchar(vwall[i][j] ? ' ' : '-');
			putchar('+');
		}
		puts("");
	}

	return 0;
}

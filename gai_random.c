/*
	Randomise getaddrinfo() results
	Copyright 2017  Simon Arlott

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>

#include <dlfcn.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>

int getaddrinfo(const char *node, const char *service,
		const struct addrinfo *hints, struct addrinfo **res) {
	int (*next_getaddrinfo)(const char*, const char*, const struct addrinfo*, struct addrinfo**) = dlsym(RTLD_NEXT, "getaddrinfo");
	int ret = (*next_getaddrinfo)(node, service, hints, res);

	if (ret) {
		return ret;
	}

	struct timespec tp;
	if (clock_gettime(CLOCK_REALTIME, &tp)) {
		return ret;
	}

	unsigned int seed = tp.tv_sec ^ tp.tv_nsec;

	struct addrinfo *p;
	size_t len = 0;

	/* put results into an array */
        for (p = *res; p; p = p->ai_next) {
		len++;
	}

	struct addrinfo **list = malloc(sizeof(struct addrinfo *) * len);
	if (list == NULL) {
		return ret;
	}

	size_t i = 0;
        for (p = *res; p; p = p->ai_next) {
		list[i++] = p;
	}

	/* shuffle results */
	for (i = 0; i < len - 1; i++) {
		size_t j = i + (rand_r(&seed) % (len - i));
		struct addrinfo tmp = *list[i];
		*list[i] = *list[j];
		*list[j] = tmp;
		list[j]->ai_next = list[i]->ai_next;
		list[i]->ai_next = tmp.ai_next;
	}

	free(list);

	return ret;
}

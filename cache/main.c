#include <stdio.h>
#include <malloc.h>

typedef unsigned char uchar;

typedef struct cache_line_s {

    uchar valid;
    uchar frequency;
    long int tag;
    uchar *block;
} cache_line_t;

typedef struct cache_s {
    uchar s;
    uchar t;
    uchar b;
    uchar E;
    cache_line_t **cache;
} cache_t;

void print_cache(cache_t cache) {

    int S = 1 << cache.s;
    int B = 1 << cache.b;

    for (int i = 0; i < S; i++) {
        printf("Set %d\n", i);
        for (int j = 0; j < cache.E; j++) {

            printf("%1d %d 0x%0*lx ", cache.cache[i][j].valid,
                   cache.cache[i][j].frequency, cache.t,
                   cache.cache[i][j].tag);

            for (int k = 0; k < B; k++) {
                printf("%02x ", cache.cache[i][j].block[k]);
            }
            puts("");
        }
    }
}

cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E) {
    // we create a new cache struct
    cache_t newCache;

    // we initialize its params
    newCache.s = s;
    newCache.t = t;
    newCache.b = b;
    newCache.E = E;
    newCache.cache = NULL;

    // we calculate how many bytes we should allocate
    int numOfSets = 1 << s;
    int linePerSet = E;
    int numOfBlocksInLine = 1 << b;

    // we first allocate the cache sets
    newCache.cache = (cache_line_t **) malloc(numOfSets * sizeof(cache_line_t *));
    // if allocation didn't work we stop the process
    if (newCache.cache == NULL) {
        printf("allocation failed!");
    }
    // for each set in cache...
    for (int i = 0; i < numOfSets; ++i) {
        // we allocate the lines of each set
        newCache.cache[i] = (cache_line_t *) malloc(linePerSet * sizeof(cache_line_t));
        // for each set we initialize its values
        for (int j = 0; j < linePerSet; ++j) {
            newCache.cache[i][j].valid = 0;
            newCache.cache[i][j].frequency = 0;
            newCache.cache[i][j].tag = 0;
            newCache.cache[i][j].block = calloc(numOfBlocksInLine, sizeof(uchar));
        }
    }
    // we return the new cache
    return newCache;
}

uchar read_byte(cache_t cache, uchar *start, long int off) {
    /* first we will extract the tag index and offset of the 'off' address */

    /* by shifting the num of bits prior to the tag, we will remain with the tag's bits
    only */
    long int adsTag = off >> (cache.s + cache.b);
    // same idea here for extracting the address' bits without the prior offset bits
    int adsBitsWithoutOffset = (off >> cache.b);

    /* we shall do & operation between the address' bits without the offset bits and the
     * binary 1..111 as the length of the index of 1 set, by doing so we will remain
     * only with the address' set index because all the rest will simply drop because
     * it will be & with 0 */

    int adsIndexOfSet = adsBitsWithoutOffset & ((1 << cache.s) - 1);

    /* same thing here. we shall do & operation between the full address bits and the
     * binary 1..111 as the length of the offset of 1 set, by doing so we will remain
     * only with the address' offset because all the rest will simply drop because
     * it will be & with 0 */
    int adsOffset = off & ((1 << cache.b) - 1);

    /* now after we have the "id" of the address in the cache we first check if it
    already exists in cache */

    // for each line in the set in which our address is at...
    for (int i = 0; i < cache.E; i++) {
        // we check if the block is valid and the tag is equal to our address' tag
        if (cache.cache[adsIndexOfSet][i].valid && cache.cache[adsIndexOfSet][i].tag ==
                                                   adsTag) {
            // if we found a match we update frequency and return the value
            cache.cache[adsIndexOfSet][i].frequency++;
            return cache.cache[adsIndexOfSet][i].block[adsOffset];
        }
    }

    /* else, the value is not in cache, so first we will look for the "oldest" line in
    our set, so we could delete it and keep the temporal locality*/

    /* as long as we don't find a smaller Freq the first line in the set is the one we
    about to override*/
    int smallestFreq = 0;

    // for each line in the set in which our address is at...
    for (int i = 1; i < cache.E; i++) {
        if (cache.cache[adsIndexOfSet][i].frequency <
            cache.cache[adsIndexOfSet][smallestFreq].frequency) {
            // if we found a line with a smaller Freq we update
            smallestFreq = i;
        }
    }

    // now we shall override the cache line with the smallest Freq, with the new value
    cache.cache[adsIndexOfSet][smallestFreq].valid = 1;
    cache.cache[adsIndexOfSet][smallestFreq].frequency = 1;
    cache.cache[adsIndexOfSet][smallestFreq].tag = adsTag;

    // we insert the value to the block
    for (int i = 0; i < (1 << cache.b); i++) {
        // we first calculate the offset needed in the cache line
        int cacheOffset = (off >> cache.b) << cache.b;
        // we align the offset to the cache block size, so we have the full block inserted
        cache.cache[adsIndexOfSet][smallestFreq].block[i] = start[cacheOffset + i];
    }

    // we return the inserted value from the cache
    return cache.cache[adsIndexOfSet][smallestFreq].block[adsOffset];
}

uchar write_byte(cache_t cache, uchar *start, long int off, uchar value) {
    /* first we will extract the tag index and offset of the 'off' address */

    /* by shifting the num of bits prior to the tag, we will remain with the tag's bits
    only */
    long int adsTag = off >> (cache.s + cache.b);
    // same idea here for extracting the address' bits without the prior offset bits
    int adsBitsWithoutOffset = (off >> cache.b);

    /* we shall do & operation between the address' bits without the offset bits and the
     * binary 1..111 as the length of the index of 1 set, by doing so we will remain
     * only with the address' set index because all the rest will simply drop because
     * it will be & with 0 */

    int adsIndexOfSet = adsBitsWithoutOffset & ((1 << cache.s) - 1);

    /* same thing here. we shall do & operation between the full address bits and the
     * binary 1..111 as the length of the offset of 1 set, by doing so we will remain
     * only with the address' offset because all the rest will simply drop because
     * it will be & with 0 */
    int adsOffset = off & ((1 << cache.b) - 1);

    /* now after we have the "id" of the address in the cache we first check if it
    exists in our cache */

    for (int i = 0; i < cache.E; i++) {
        if (cache.cache[adsIndexOfSet][i].valid && cache.cache[adsIndexOfSet][i].tag ==
                                                   adsTag) {
            // if we found it we will update the value's Freq and return the value
            cache.cache[adsIndexOfSet][i].frequency++;
            cache.cache[adsIndexOfSet][i].block[adsOffset] = value;

            /* since we are working in the write-through we also update the value in
            the 'RAM' memory */

            start[off] = value;
            // we return the new value
            return value;
        }
    }

    /* else, the value is not in cache, so first we will look for the "oldest" line in
    our set, so we could delete it and keep the temporal locality*/

    /* as long as we don't find a smaller Freq the first line in the set is the one we
    about to override*/
    int smallestFreq = 0;

    // for each line in the set in which our address is at...
    for (int i = 1; i < cache.E; i++) {
        if (cache.cache[adsIndexOfSet][i].frequency <
            cache.cache[adsIndexOfSet][smallestFreq].frequency) {
            // if we found a line with a smaller Freq we update
            smallestFreq = i;
        }
    }

    // now we shall override the cache line with the smallest Freq, with the new value
    cache.cache[adsIndexOfSet][smallestFreq].valid = 1;
    cache.cache[adsIndexOfSet][smallestFreq].frequency = 1;
    cache.cache[adsIndexOfSet][smallestFreq].tag = adsTag;

    // we insert the value to the block
    for (int i = 0; i < (1 << cache.b); i++) {
        cache.cache[adsIndexOfSet][smallestFreq].block[i] = start[off + i];
    }

    // we now write the new value to both cache and memory
    cache.cache[adsIndexOfSet][smallestFreq].block[adsOffset] = value;
    start[off] = value;

    // we return the new value
    return value;
}


int main() {
    int n;

    printf("Size of data: ");
    scanf("%d", &n);

    uchar *mem = malloc(n);

    printf("Input data >> ");

    for (int i = 0; i < n; i++)
        scanf("%hhd", mem + i);

    int s, t, b, E;

    printf("s t b E: ");
    scanf("%d %d %d %d", &s, &t, &b, &E);

    cache_t cache = initialize_cache(s, t, b, E);

    while (1) {

        scanf("%d", &n);
        if (n < 0)
            break;
        read_byte(cache, mem, n);

    }

    puts("");
    print_cache(cache);

    free(mem);
}
--Sola Oladosu
Everything


--Ji Chan
Everything

---Best Cache
    The way we began this experiment was to start with thinking about 
what parameters would result in less cycles, and therefore handle data faster. Cycles are increased significantly 
if memory has to be accessed, so we thought about which parameters would result in memory being accessed less frequently.
So, we hypothesized that between lru and fifo, lru would be faster. This is because evicting less used blocks means
ones more likely to be accessed will stay in the cache and will not have to be searched for in memory.
We hypothesized that write-allocate would be faster than no-write-allocate because it stores 
to the cache on a miss. So, if the block needs to be accessed again, you would not
have to waste cycles looking for it in memory. We hypothesized write-back would be faster than
write-through because we only have to write to memory if the block is dirty and has to be evicted.
Then, we hypothesized that a bigger number of slots would result in less cylces because you would be able to store more data
in the cache and have less evictions, which require accessing memory if the block is not in memory already. 
A bigger number of slots is only efficient if it is set associative because to check for hits, you have to check every block. 
But, with sets, you have indices that let you narrow down sections of blocks to search for data in.
We also hypothesized that a smaller number of bytes would be faster, because in the event of moving memory, less memory
has to be moved, taking fewer cycles. We ran these experiements on four sizeable trace files so we could test with different data that
shows noticeable differences in statistics.
    We tested write-allocate versus no-write-allocate, write-through versus write-back,
and lru versus fifo first. We did these first because we figured changes to cache size,
block size, and set number would be less significant to changing the number of cycles are a different type of change. 
We found that write-allocate usually has more load and store hits but always
more cycles. This makes sense because always storing missed data to the cache means
it has to do more evictions when it gets full, but it is overall faster because we don't have to access memory
on blocks we previously missed. So, write-allocate is slower but more efficient, especially
because increasing cache size would probably reduce the cycles issue. 
    Second, we tested write-through versus write-back and found that hits and misses are the same for them,
but cycles are less for write-back. This was because instead of always storing to memory, we only store if 
the block is evicted from the cache.
    Third, we tested lru versus fifo, and we found that between them, all statistics were the same, except but lru takes less cycles.
This result matched the reasoning in our hypothesis.
    After we had those pieces of information, we experiemented with having larger versus smaller block sizes. We found that bigger block size usually results in more hits
but has two to four times more cycles, so takes significantly longer. This is expected because having less memory to transfer is faster. 
    So,  that write-allocate, write-back, lru, and a smaller block size result in less cycles and therefore better speed. Next, we looked
at if a set, direct, or fully associative configuration would result in less cycles. We found that set has way faster total cycles, and more hits overall.
This follows the reasoning in our hypothesis.
    Lastly, we experiemented with having more total blocks. We found having more blocks halves the cycle count, so is significantly faster, which makes sense because
    it also results in more hits. This is expected because having a bigger cache means less instances of having to access memory, which takes more cycles
    In conclusion, the fastest, and therefore best configuration is one that is write-allocate, write-back, lru, set associative with a large number of blocks and a 
small block size. We have included the exact statistics from our experiement in a Google doc link below.

-Link
https://docs.google.com/document/d/16zbTmUHAFROE65M2FVaZBxNFcZ556v9S6B4LTyrSh-U/edit?usp=sharing


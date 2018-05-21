/**
* Computer Organization
* Simulate Cache Assignment
* * @author DCMMC
*/

#include <iostream>
#include <vector>
#include <utility> // pair
#include <tuple>
#include <bitset>
#include <cmath>
#include <unordered_map>
#include <string>
#include "DLList.h"

// asume machine byte is 8 bits and word is 16 bits
#define BYTE_SIZE 8
#define WORD_SIZE (BYTE_SIZE * 2)

using namespace std;

/**
 * 缓存块单位
 */
template <size_t Tag_L, size_t Data_L>
struct cache_t
{
    // empty flag: indicates emty if Valid == false
    bool Valid;
    // Tag
    bitset<Tag_L> Tag;
    // Data
    bitset<Data_L> Data;
};

/**
 * Direct Mapped
 * @param addrReqs 内存地址请求序列, 五位二进制, word address
 * @param cache 模拟缓存, tuple<Valid, Tag, Data>, 其中 Data 用请求的序号来表示, Tag 为分组内存(每组内存大小跟缓存一样大)
 * 的组号, 用来标记该缓存块是否存储的是这个组里面对应的这一块, 否则就是 Miss, Valid 就一位, 用来表明内存的 empty 状态(内存刚加电
 * 的时候, 所有块都初始化为 1), Data 就是存储数据的, 大小跟内存的块一样, 缓存的最小单位就是块, 而内存不是(内存最小单位为字节, 
 * 如果块的单位就是字节的话, 那么就不需要块内地址了, 主存地址的最低 k 为就是 Cache 块号)
 * 
 * 这里按照题目要求, cache 有 8-words, 并且内存地址是 word address(也就是以 word 为单位的地址), 所以 direct mapping 情况下,
 * 内存地址 0b11111 的高两位是 Tag(也就是组号, 用于验证是否缓存中的这一块是否是位于内存的这一个组(group)中,
 * 中间两位是 index (块号, 用于直接映射到缓存), 低一位是块内地址(offset)
 */
void directMapped(const vector<bitset<5>> &addrReqs, vector<cache_t<2, 2 * WORD_SIZE>> &cache)
{
    int count = 0;
    // 1 表示 hint, 0 表示 miss
    vector<bool> hits;
    // 重置 cache, valid 位全部重置为 false
    for (auto i : cache)
    {
        i.Valid = false;
        i.Tag = bitset<2>(0);
        i.Data = bitset<2 * WORD_SIZE>(0);
    }

    for (auto i : addrReqs)
    {
        count++;
        unsigned long block_index = i.to_ulong() / 2 % 4, tag = i.to_ulong() / 8;
        if (cache[block_index].Valid && cache[block_index].Tag.to_ulong() == tag)
        {
            hits.push_back(1);
            // update data
            if (i[0] == 1)
                cache[block_index].Data &= bitset<WORD_SIZE * 2>(0xffff);
            else
                cache[block_index].Data &= bitset<WORD_SIZE * 2>(0xffff << 16);
            cache[block_index].Data |= bitset<WORD_SIZE * 2>(i[0] == 1 ? (count << WORD_SIZE) : count);
        }
        else
        {
            // 要么是该块是 empty 要么是别的组的相应的块在这里
            hits.push_back(0);
            cache[block_index].Valid = true;
            cache[block_index].Tag &= bitset<2>(0);
            cache[block_index].Tag |= bitset<2>(tag);
            if (i[0] == 1)
                cache[block_index].Data &= bitset<WORD_SIZE * 2>(0xffff);
            else
                cache[block_index].Data &= bitset<WORD_SIZE * 2>(0xffff << 16);
            // 如果 offset 是 1 就放在高 8 位
            cache[block_index].Data |= bitset<WORD_SIZE * 2>(i[0] == 1 ? (count << WORD_SIZE) : count);
        }
    }

    cout << "Direct mapping:\n";
    // output hits
    cout << "Hints: \n";
    int hintCount = 0, missCount = 0;
    for (auto i : hits)
    {
        cout << (i ? "Hints " : "Miss ");
        if (i)
            hintCount++;
        else
            missCount++;
    }
    cout << "\nHints: " << hintCount << ", Misses: " << missCount << endl;
    // cache values
    count = 0;
    cout << "Cache: \nBlocks\tTag\tData\n";
    for (auto i : cache)
    {
        cout << count << ", " << (count + 1) << "\t" << (i.Valid ? i.Tag.to_string() : "empty")
             << "\t" << (i.Valid ? to_string((i.Data.to_ulong() & 0xffff)) : "empty") << "\t"
             << (i.Valid ? to_string(i.Data.to_ulong() >> 16) : "empty") << "\n";
        count += 2;
    }
    cout << endl;
}

/**
 * set associativity
 * direct mapped 就相当于 1-way set associative (也就是 set index 有 2^k 个)
 * full associative 相当于 2^k-way set associative (2^k就是缓存的块数, 也就是 set index 只有 1 个), 对于 set index 直接
 * 位操作就的得到, 而 set 中的 block 数只能顺序遍历, 并且 set 中的所有 block 都满了的时候需要采用 LRU 策略替换
 * 
 * replacement approach: LRU(least recently used), 假设最近最不访问的那个缓存块在后面的操作中也不怎么会被访问到
 * 典型的 LRU implement: 使用 double linkedlist 和 hashMap
 * 
 * @template Block_Size 指定一个块的内存单位(单位可以使 word 也可以是 byte, 取决于具体机器, 一般机器内存都是以 byte 为单位,
 * 但是练习里面是以 word 为单位的)数, 这里单位为 word, 取值 1 或 2
 * @template Set_Size 组的大小, 例如 Set_Size 为 2 表示 2-way set associative
 * ! Block_Words 和 Set_Size 必须是有效的值, 这里不做任何正确性检查
 * * Benchmark show that Random have anology same efficient as LRU when high-way set associative and large-capacity cache
 * 
 * ! 这里用 HashMap 和 Double LinkedList 来实现 LRU, 其实真实的硬件肯定不会搞这么复杂的结构, 都是针对具体情况
 * ! 设计标志位和硬件的(比如 2-way s.a. 就只需要 log_2 {2!} = 1 位就可以搞定, 对于 n-way, optimall 情况下
 * ! 应该是 log_2 {n!} 为就可以搞定这个队列标志位)
 * 
 * * 分组关联缓存地址映射的好处: 因为大部分程序是 code segment, data segment, stack segment
 * * 并且大部分数据存取的时候都会紧接着存取下一个邻近单元, 组关联相比直接映射能够减少冲突的可能性.
 * * Ref: https://courses.cs.washington.edu/courses/cse378/09wi/lectures/lec16.pdf
 * * Ref: http://www.csbio.unc.edu/mcmillan/Media/L20Spring2013.pdf
 * * Ref: http://inst.eecs.berkeley.edu/~cs162/sp15/static/lectures/13.pdf
 */
template <size_t Block_Size, size_t Set_Size>
vector<cache_t<5 - (int)(log(Block_Size) / log(2)) - (int)(log(8 / Set_Size / Block_Size) / log(2)),
               Block_Size * WORD_SIZE>>
setAssociativity(const vector<bitset<5>> &addrReqs,
                 vector<cache_t<5 - (int)(log(Block_Size) / log(2)) - (int)(log(8 / Set_Size / Block_Size) / log(2)),
                                Block_Size * WORD_SIZE>>
                     cache =
                         vector<cache_t<5 - (int)(log(Block_Size) / log(2)) - (int)(log(8 / Set_Size / Block_Size) / log(2)),
                                        Block_Size * WORD_SIZE>>(8 / Block_Size,
                                                                 cache_t<5 - (int)(log(Block_Size) / log(2)) - (int)(log(8 / Set_Size / Block_Size) / log(2)),
                                                                         Block_Size * WORD_SIZE>()))
{
    int count = 0;
    // 1 表示 hint, 0 表示 miss
    vector<bool> hits;
    // double linkedlist used by LRU, most recently used page at the start of the list,
    // least recently used at the end of the list.
    // blockLists store the address(index) of cache block
    vector<LinkedList<unsigned char>> blockLists = vector<LinkedList<unsigned char>>(8 / Set_Size 
        / Block_Size, LinkedList<unsigned char>());
    // key is block number, the value is pointer address of list node
    vector<unordered_map<size_t, LinkedList<unsigned char>::Node *>> blockPointerMap(8 / Set_Size
        / Block_Size, unordered_map<size_t, LinkedList<unsigned char>::Node *>());
    // 重置 cache, valid 位全部重置为 false
    count = 0;
    for (auto i : cache)
    {
        blockLists[count / Set_Size].addFirst(count);
        blockPointerMap[count / Set_Size][count] = blockLists[count / Set_Size].getNode(0);
        i.Valid = false;
        i.Tag = bitset<5 - (int)(log(Block_Size) / log(2)) - (int)(log(8 / Set_Size / Block_Size) / log(2))>(0);
        i.Data = bitset<Block_Size * WORD_SIZE>(0);
        count++;
    }

    count = 0;
    for (auto i : addrReqs)
    {
        count++;

        // * express can be simplier
        // set_index may be 0, and Tag_L may be 0(block amount of memory equals to block amount of cache)
        size_t set_index = i.to_ulong() / Block_Size % (8 / Set_Size / Block_Size),
               tag = i.to_ulong() / Block_Size / (8 / Set_Size / Block_Size);

        // if is full, using LRU
        bool not_full = false;
        // scan all blocks in the set
        // block_index 是以 block 为单位的, 缓存的最小单位就是块
        for (int block_index = Set_Size * set_index; block_index < Set_Size * (set_index + 1);
             block_index++)
        {
            if (cache[block_index].Valid && cache[block_index].Tag.to_ulong() == tag)
            {
                // hits
                hits.push_back(1);
                // update data
                cache[block_index].Data &= (Block_Size == 2 ? (i[0] == 1 ? 
                bitset<WORD_SIZE * Block_Size>(string(WORD_SIZE, '1')) : 
                    (bitset<WORD_SIZE * Block_Size>(string(WORD_SIZE, '1')) << WORD_SIZE))
                     : bitset<WORD_SIZE * Block_Size>(0));
                // * FIXME to adapte more cases of Block_Size
                cache[block_index].Data |= bitset<WORD_SIZE * Block_Size>(Block_Size == 2 && i[0] == 1
                                                                              ? (count << WORD_SIZE)
                                                                              : count);

                // move the hint block address to the start of list
                LinkedList<unsigned char>::Node *node = blockPointerMap[set_index][block_index];
                unsigned char index = node->item;
                blockLists[set_index].removeNode(node);
                blockLists[set_index].addFirst(index);

                not_full = true;
                break;
            }
            else if (!cache[block_index].Valid)
            {
                // 该块是 empty
                hits.push_back(0);
                cache[block_index].Valid = true;
                cache[block_index].Tag &= bitset<5 - (int)(log(Block_Size) / log(2)) -
                                                 (int)(log(8 / Set_Size / Block_Size) / log(2))>(0);
                cache[block_index].Tag |= bitset<5 - (int)(log(Block_Size) / log(2)) -
                                                 (int)(log(8 / Set_Size / Block_Size) / log(2))>(tag);
                // update data
                cache[block_index].Data &= (Block_Size == 2 ? (i[0] == 1 ? 
                bitset<WORD_SIZE * Block_Size>(string(WORD_SIZE, '1')) 
                    : (bitset<WORD_SIZE * Block_Size>(string(WORD_SIZE, '1')) << WORD_SIZE)) 
                    : bitset<WORD_SIZE * Block_Size>(0));
                cache[block_index].Data |= bitset<WORD_SIZE * Block_Size>(Block_Size == 2 && i[0] == 1
                                                                              ? (count << WORD_SIZE)
                                                                              : count);

                // move the new block address to the start of list
                LinkedList<unsigned char>::Node *node = blockPointerMap[set_index][block_index];
                unsigned char index = node->item;
                blockLists[set_index].removeNode(node);
                blockLists[set_index].addFirst(index);

                not_full = true;
                break;
            }
        }

        // s.a. or full associative
        if (Set_Size != 1 && !not_full)
        {
            // miss
            hits.push_back(0);
            // full, using LRU
            // remove the last of list and move the new block to the start of the list.
            LinkedList<unsigned char>::Node *node = blockLists[set_index].getNode(
                    blockLists[set_index].getSize() - 1);
            unsigned char index = node->item;
            blockLists[set_index].removeNode(node);
            blockLists[set_index].addFirst(index);
            blockPointerMap[set_index][index] = blockLists[set_index].getNode(0);

            cache[index].Tag &= bitset<5 - (int)(log(Block_Size) / log(2)) -
                                       (int)(log(8 / Set_Size / Block_Size) / log(2))>(0);
            cache[index].Tag |= bitset<5 - (int)(log(Block_Size) / log(2)) -
                                       (int)(log(8 / Set_Size / Block_Size) / log(2))>(tag);
            // update data
            cache[index].Data &= (Block_Size == 2 ? (i[0] == 1 ?
                 bitset<WORD_SIZE * Block_Size>(string(WORD_SIZE, '1')) : 
                (bitset<WORD_SIZE * Block_Size>(string(WORD_SIZE, '1')) << WORD_SIZE)) 
                    : bitset<WORD_SIZE * Block_Size>(0));
            cache[index].Data |= bitset<WORD_SIZE * Block_Size>(Block_Size == 2 && i[0] == 1
                                                                    ? (count << WORD_SIZE)
                                                                    : count);
        }
        else if (!not_full)
        {
            // direct mapping missed caused by conflict.
            hits.push_back(0);

            // update data
            cache[Set_Size * set_index].Data &= (Block_Size == 2 ? 
                (i[0] == 1 ? bitset<WORD_SIZE * Block_Size>(string(WORD_SIZE, '1')) : 
                (bitset<WORD_SIZE * Block_Size>(string(WORD_SIZE, '1')) << WORD_SIZE)) 
                    : bitset<WORD_SIZE * Block_Size>(0));
            cache[Set_Size * set_index].Data |= bitset<WORD_SIZE * Block_Size>(Block_Size == 2 && i[0] == 1
                                                                          ? (count << WORD_SIZE)
                                                                          : count);
        }
    }

    // output the results
    cout << (Set_Size == 1 ? "Direct Mapping" : (Set_Size * Block_Size == 8 ? 
        "Full Associative" : (to_string(Set_Size) + " Set Associative"))) << endl;
    // output hits
    cout << "Hints: \n";
    int hitsCnt = 0, missCnt = 0;
    for (auto i : hits)
    {
        cout << (i ? "Hints " : "Miss ");
        if (i)
            hitsCnt++;
        else
            missCnt++;
    }
    cout << "\nHints: " << hitsCnt << ", Misses: " << missCnt << endl;
    // cache values
    count = 0;
    cout << "Cache: \nBlocks\tTag\tData\n";
    for (auto i : cache)
    {
        cout << count << " ~ " << (count + Block_Size - 1) << "\t";
        cout << (i.Valid ? i.Tag.to_string() : "empty") << "\t | ";
        for (int j = 0; j < Block_Size; j++)
        {
            // 单位 word(题目这样要求的)
            string value = (bitset<WORD_SIZE>(((i.Data >> (j * WORD_SIZE)) & bitset<i.Data.size()>(string(WORD_SIZE, '1')))
                                                  .to_ullong()))
                               .to_string();
            cout << value << "(" << stoi(value, 0, 2) << ") | ";
        }
        count += Block_Size;
        cout << "\n";
    }
    cout << endl;

    return cache;
}

int main(void)
{
    const vector<bitset<5>> addrReqs = {
        bitset<5>(0b00000), bitset<5>(0b00001), bitset<5>(0b00010), bitset<5>(0b00011),
        bitset<5>(0b01000), bitset<5>(0b01001), bitset<5>(0b01010), bitset<5>(0b01011),
        bitset<5>(0b00000), bitset<5>(0b00001), bitset<5>(0b00010), bitset<5>(0b00111),
        bitset<5>(0b00011), bitset<5>(0b00000), bitset<5>(0b00001)};
    vector<cache_t<2, 2 * WORD_SIZE>> cache(4, cache_t<2, 2 * WORD_SIZE>());
    directMapped(addrReqs, cache);

    // direct mapped
    setAssociativity<2, 1>(addrReqs);
    // 2-way s.a.
    setAssociativity<2, 2>(addrReqs);
    // 4-way s.a.
    setAssociativity<1, 4>(addrReqs);
    // full associative
    setAssociativity<1, 8>(addrReqs);

    return 0;
}
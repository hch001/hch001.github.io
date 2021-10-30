import requests, re, redis, time
from typing import List


class BitVector:
    def __init__(self, cap: int):
        self.__cap = cap
        self.__size = int((cap + 31 - 1) / 31)
        self.__list = [0] * self.__size

    def set_bit(self, pos: int):
        idx = int(pos / 31)
        sub_idx = int(pos - idx * 31)
        self.__list[idx] |= 1 << sub_idx

    def get_bit(self, pos: int):
        idx = int(pos / 31)
        sub_idx = int(pos - idx * 31)
        return self.__list[idx] & (1 << sub_idx)


class HashFunc:
    def __init__(self, cap, seed=401):
        self.__cap = cap
        self.__seed = seed

    def get_value(self, value: str):
        hash_value = 0
        for i in range(value.__len__()):
            hash_value = (hash_value * self.__seed + ord(value[i])) % self.__cap

        return hash_value


class BloomFilter:
    def __init__(self, hash_list: List[int], cap):
        self.__hash_list = [HashFunc(cap, seed) for seed in hash_list]
        self.__bit_vector = BitVector(cap)

    def exists(self, value: str) -> bool:
        for hash_func in self.__hash_list:
            if not self.__bit_vector.get_bit(hash_func.get_value(value)):
                return False

        return True

    def add(self, value: str):
        for hash_func in self.__hash_list:
            self.__bit_vector.set_bit(hash_func.get_value(value))


class ReBloomFilter:
    def __init__(self, hash_list: List[int], cap):
        self.__hash_list = [HashFunc(cap, seed) for seed in hash_list]
        self.__redis = redis.Redis('localhost')

    def exists(self, value: str) -> bool:
        for hash_func in self.__hash_list:
            if not self.__redis.getbit("bf", hash_func.get_value(value)):
                return False

        return True

    def add(self, value: str):
        for hash_func in self.__hash_list:
            self.__redis.setbit('bf', hash_func.get_value(value), 1)


def test():
    capacity = int(1e4 * 3)
    bloom_filter = BloomFilter([401, 101], capacity)

    add_num = int(1e4)
    test_num = int(1e4 * 2)

    for add_value in range(add_num):
        bloom_filter.add('value' + str(add_value))

    positive_error_count = 0
    negative_error_count = 0
    positive = 0
    negative = 0

    for test_value in range(test_num):
        if bloom_filter.exists('value' + str(test_value)):
            positive += 1
            if test_value >= add_num:
                positive_error_count += 1
        else:
            negative += 1
            if test_value < add_num:
                negative_error_count += 1

    print("positive count: " + str(positive))
    print("negative count: " + str(negative))
    print("positive error count: " + str(positive_error_count))
    print("negative error count: " + str(negative_error_count))
    print("positive error rate: %.3f " % (positive_error_count * 100 / positive) + "%")
    print("negative error rate: %.3f " % (negative_error_count * 100 / negative) + '%')


if __name__=='__main__':
    test()

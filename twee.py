#!/bin/python3

from functools import reduce


class DataBlock:

    INDX = 1

    def __init__(self, data):
        self.data = data
        self.index = DataBlock.INDX
        DataBlock.INDX += 1
    
    def __str__(self):
        return f'( {self.index} {self.data} )';

    def __lt__(self, a):
        return self.data < a

    def __gt__(self, a):
        return self.data > a

class TweeNode:
    def __init__(self, data, left = None, right = None):
        self.left = left
        self.right = right
        self.data = data
        self.heigth = 1

    def __str__(self):
        a = f'{str(self.left)} ' if(self.left != None) else ''
        b = f' {str(self.right)}' if(self.right != None) else ''

        return f'{a}{str(self.data)}{b}'

    def connect(self, left = None, right = None):
        if left != None:
            self.left = left
        if right != None:
            self.right = right

    def insert(self, tw):
        if tw.data < self.data:
            if(self.left != None):
                self.left.insert(tw)
            else:
                self.left = tw
        elif tw.data > self.data:
            if(self.right != None):
                self.right.insert(tw)
            else:
                self.right = tw


class TweeList:
    def __init__(self):
        self.list = []
        self.root = None

    def __str__(self):
        return reduce(lambda a, b: (str(a.data) if isinstance(a, TweeNode) else a) + ' ' + str(b.data), self.list)

    def insert(self, data):
        if len(self.list) == 0:
            self.root = TweeNode(data)
            self.list.append(self.root)
        else:
            a = TweeNode(data)

            start = 0
            mid = 0
            end = len(self.list) - 1

            while end >= start:

                mid = (end + start)//2

                if   self.list[mid].data < data: end   = mid - 1
                elif self.list[mid].data > data: start = mid + 1
                else: return False

            # print(mid , ':', str(self))

            self.list.insert(mid, a)
            self.root.insert(a)         # generating connections


k = TweeList()

a = ['black', 'red', 'green', 'blue', 'magenta', 'cyan', 'yellow', 'white']

for i in a:
    k.insert(DataBlock(i))

print('Ascending Sorted Order  ->', k)
print('Descending Sorted Order ->', k.root)

# Search Time by value : O(log2(n))
# Search Time by index : O(1)
# Insertion Time : O(log2(n))

# Space complexity: O(n)

# Example that maybe good:
# Search by value occurs by (key, value) pairing
# Data can be sorted by asending order of value
# will behave like ordered dictionary

# This example works on the same principle as the C program. here however we keep the data sorted in
# reverse order to prove that this system is independent of the ordering scheme.

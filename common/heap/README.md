# Heap Implementations

## Base Heaps

Base heaps support the following operations:

- `bool Empty() const;` - Check if the heap is empty.
- `unsigned Size() const;` - Get the number of elements in the heap.
- `void Add(const TData& data);` - Add a new element to the heap.
- `TData Top() const;` - Get the top element of the heap.
- `void Pop();` - Remove the top element from the heap.
- `TData Extract();` - Extract the top element from the heap.

Optional operations:

- `void ExtractAll();` - Extract all elements from the heap.
- `void Union(TSelf& heap);` - Union with another heap.

## Extended Heaps

Extended heaps support the following operations in addition to the base operations:

- `void DecreaseValue(unsigned key, const TValue& value);` - Decrease the value of an element.
- `void DecreaseValueIfLess(unsigned key, const TValue& value);` - Decrease the value if the new value is less.

Optional operations:

- `void IncreaseValue(unsigned key, const TValue& value);` - Increase the value of an element.
- `void Set(unsigned key, const TValue& value);` - Set the value of an element.
- `void ExtractAll();` - Extract all elements from the heap.

## UKVM Heaps

UKVM heaps support the following operations:

- `bool Empty() const;` - Check if the heap is empty.
- `unsigned Size() const;` - Get the number of elements in the heap.
- `unsigned UKeySize() const;` - Get the number of unique keys in the heap.
- `bool InHeap(unsigned key) const;` - Check if a key is in the heap.
- `const TValue& Get(unsigned key) const;` - Get the value associated with a key.
- `std::vector<TValue> GetValues() const;` - Get all values in the heap.
- `void AddNewKey(unsigned key, const TValue& value, bool skip_heap = false);` - Add a new key-value pair to the heap.
- `void DecreaseValue(unsigned key, const TValue& value);` - Decrease the value of an element.
- `void DecreaseValueIfLess(unsigned key, const TValue& value);` - Decrease the value if the new value is less.
- `void IncreaseValue(unsigned key, const TValue& value);` - Increase the value of an element.
- `void Set(unsigned key, const TValue& value);` - Set the value of an element.
- `void Add(const TData& data);` - Add a new element to the heap.
- `unsigned TopKey() const;` - Get the key of the top element.
- `const TValue& TopValue() const;` - Get the value of the top element.
- `TData Top() const;` - Get the top element of the heap.
- `void Pop();` - Remove the top element from the heap.
- `unsigned ExtractKey();` - Extract the key of the top element.
- `const TValue& ExtractValue();` - Extract the value of the top element.
- `TData Extract();` - Extract the top element from the heap.

Optional operations:

- `void ExtractAll();` - Extract all elements from the heap.

## Monotone Heaps

Monotone heaps assume that the priority for new elements is never less than the current top priority.

# Heaps
* Base heaps support next operations:
    * bool Empty() const;
    * unsigned Size() const;
    * void Add(const TData& data);
    * TData Top() const;
    * void Pop();
    * TData Extract();
    * void Union(TSelf& heap);
* UKey Value Maps support next operations:
    * bool Empty() const;
    * unsigned Size() const;
    * unsigned UKeySize() const;
    * const TValue& Get(unsigned key) const;
    * std::vector<TValue> GetValues() const;
    * void AddNewKey(unsigned key, const TValue& value, bool skip_heap = false);
    * void DecreaseValue(unsigned key, const TValue& value);
    * void DecreaseValueIfLess(unsigned key, const TValue& value);
    * void IncreaseValue(unsigned key, const TValue& value);
    * void Set(unsigned key, const TValue& value);
    * void Add(const TData& data);
    * unsigned TopKey() const;
    * const TValue& TopValue() const;
    * TData Top() const;
    * void Pop();
    * unsigned ExtractKey();
    * const TValue& ExtractValue();
    * TData Extract();

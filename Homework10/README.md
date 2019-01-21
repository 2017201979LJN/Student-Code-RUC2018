# avl-tree-zip
The task of Advanced Programming class on 2018/12/26. Compress and decompress an avl-tree with (1 `double` + 1 `char`) for each node, or its insert sequence.

# Compress an [avl-tree](/Menci/avl-tree)
Generate random data and test:

```bash
make with-structure
./with-structure <<< "[n] [seed]" > data.bin
```

# Compress an [avl-tree](/Menci/avl-tree)'s insert sequence
Generate random data:

```bash
make insert-sequence-data-generator
./insert-sequence-data-generator <<< "[n] [seed]" > data.in
```

Test:

```bash
make insert-sequence
./insert-sequence zip data.in data.zipped
./insert-sequence unzip data.zipped data.out
diff data.in data.out # Should output nothing.
```

# Compress ratio
For a avl-tree with 1,000,000 nodes, method 1 (with full precise `double`) output about 8,500,000 bytes data. Method 2 (keep only 3 decimal places for `double`, and each `double` is in [-1000, 1000]) can compress a 10 MiB input data to about 3 MiB.

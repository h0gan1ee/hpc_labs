# Optimization Doc

- Change compiler to clang++
- Add compiler flags: `--Ofast -ffast-math -mcpu=apple-m1 -mtune=native`
- Use simd library in Apple's Accelerate Framework
  - `simd_distance(_:_:)`: For computing distances

# Output logs

Original:

```
Case 1 Computing time: 3600.07ms
There are 0 points' labels are different from original file.
Case 2 Computing time: 12740.4ms
There are 0 points' labels are different from original file.
Case 3 Computing time: 2484.94ms
There are 0 points' labels are different from original file.
```

After using simd for computing distances:

```
Case 1 Computing time: 3569.33ms
There are 0 points' labels are different from original file.
Case 2 Computing time: 12953ms
There are 0 points' labels are different from original file.
Case 3 Computing time: 2503.59ms
There are 0 points' labels are different from original file.
```
Directory with tests comparing runtimes for diffrent implementations

### Sample usage (cpp implementations)
```console
[user@yourOS pso-test]$ python test-cpp-implementations.py 
Compiled ../pso-cpp successfully
../pso-cpp run 1/20: 0.00693573 s
../pso-cpp run 2/20: 0.01128 s
../pso-cpp run 3/20: 0.0120479 s
../pso-cpp run 4/20: 0.0115593 s
../pso-cpp run 5/20: 0.0102718 s
../pso-cpp run 6/20: 0.00867193 s
../pso-cpp run 7/20: 0.0106281 s
../pso-cpp run 8/20: 0.00719393 s
../pso-cpp run 9/20: 0.0079464 s
../pso-cpp run 10/20: 0.00866508 s
../pso-cpp run 11/20: 0.0128459 s
../pso-cpp run 12/20: 0.00735295 s
../pso-cpp run 13/20: 0.00716445 s
../pso-cpp run 14/20: 0.00827436 s
../pso-cpp run 15/20: 0.00874579 s
../pso-cpp run 16/20: 0.012988 s
../pso-cpp run 17/20: 0.0104322 s
../pso-cpp run 18/20: 0.0106548 s
../pso-cpp run 19/20: 0.00916909 s
../pso-cpp run 20/20: 0.00654443 s

../pso-cpp Average Runtime: 0.009 s
Compiled ../pso-cpp-avx successfully
../pso-cpp-avx run 1/20: 0.00836502 s
../pso-cpp-avx run 2/20: 0.0104555 s
../pso-cpp-avx run 3/20: 0.0105615 s
../pso-cpp-avx run 4/20: 0.0149282 s
../pso-cpp-avx run 5/20: 0.0104115 s
../pso-cpp-avx run 6/20: 0.0138826 s
../pso-cpp-avx run 7/20: 0.00551236 s
../pso-cpp-avx run 8/20: 0.00837875 s
../pso-cpp-avx run 9/20: 0.0131201 s
../pso-cpp-avx run 10/20: 0.00675639 s
../pso-cpp-avx run 11/20: 0.0141213 s
../pso-cpp-avx run 12/20: 0.0125008 s
../pso-cpp-avx run 13/20: 0.00943127 s
../pso-cpp-avx run 14/20: 0.0122651 s
../pso-cpp-avx run 15/20: 0.00956205 s
../pso-cpp-avx run 16/20: 0.013091 s
../pso-cpp-avx run 17/20: 0.0190226 s
../pso-cpp-avx run 18/20: 0.00994257 s
../pso-cpp-avx run 19/20: 0.0126576 s
../pso-cpp-avx run 20/20: 0.0106166 s

../pso-cpp-avx Average Runtime: 0.011 s

Comparison:
Scalar Average: 0.009469 s
AVX Average: 0.011279 s
Scalar is 16.052052% faster
[r4qq@fedora pso-test]$ python test-cpp-implementations.py 
Compiled ../pso-cpp successfully
../pso-cpp run 1/20: 0.0172162 s
../pso-cpp run 2/20: 0.00810878 s
../pso-cpp run 3/20: 0.0164771 s
../pso-cpp run 4/20: 0.00920113 s
../pso-cpp run 5/20: 0.00714547 s
../pso-cpp run 6/20: 0.00805249 s
../pso-cpp run 7/20: 0.0138162 s
../pso-cpp run 8/20: 0.014412 s
../pso-cpp run 9/20: 0.0121127 s
../pso-cpp run 10/20: 0.00802473 s
../pso-cpp run 11/20: 0.0113676 s
../pso-cpp run 12/20: 0.0115478 s
../pso-cpp run 13/20: 0.00648527 s
../pso-cpp run 14/20: 0.0102432 s
../pso-cpp run 15/20: 0.0153435 s
../pso-cpp run 16/20: 0.00881123 s
../pso-cpp run 17/20: 0.0121697 s
../pso-cpp run 18/20: 0.0100964 s
../pso-cpp run 19/20: 0.00796855 s
../pso-cpp run 20/20: 0.0101056 s

../pso-cpp Average Runtime: 0.011 s
Compiled ../pso-cpp-avx successfully
../pso-cpp-avx run 1/20: 0.00794752 s
../pso-cpp-avx run 2/20: 0.0124767 s
../pso-cpp-avx run 3/20: 0.00961827 s
../pso-cpp-avx run 4/20: 0.0144367 s
../pso-cpp-avx run 5/20: 0.0101809 s
../pso-cpp-avx run 6/20: 0.0071417 s
../pso-cpp-avx run 7/20: 0.00641553 s
../pso-cpp-avx run 8/20: 0.0112774 s
../pso-cpp-avx run 9/20: 0.00903282 s
../pso-cpp-avx run 10/20: 0.00926144 s
../pso-cpp-avx run 11/20: 0.00719914 s
../pso-cpp-avx run 12/20: 0.00992462 s
../pso-cpp-avx run 13/20: 0.0116067 s
../pso-cpp-avx run 14/20: 0.0115146 s
../pso-cpp-avx run 15/20: 0.00881259 s
../pso-cpp-avx run 16/20: 0.00565458 s
../pso-cpp-avx run 17/20: 0.0110719 s
../pso-cpp-avx run 18/20: 0.0117971 s
../pso-cpp-avx run 19/20: 0.0101931 s
../pso-cpp-avx run 20/20: 0.0117158 s

../pso-cpp-avx Average Runtime: 0.010 s

Comparison:
Scalar Average: 0.010935 s
AVX Average: 0.009864 s
AVX is 9.796976% faster
```
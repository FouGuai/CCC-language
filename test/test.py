def power(x, exp):
    if exp == 0:
        return 1
    elif exp == 1:
        return x
    elif exp % 2 != 0:
        y = power(x * x, exp // 2) * x
        return y
    else:
        y = power(x * x, exp // 2)
        return y

def getDigit(x):
    ans = 0
    while x != 0:
        ans += x % 10
        x = x // 10
    return ans

for i in range(0, 10000000):
    x = getDigit(power(i, 2))
    if x == 16:
        print(i, power(i, 2))
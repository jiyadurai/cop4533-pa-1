import random
def main():
    for i in range(12):
        n = 2**i
        preferences = [list(range(1, n+1)) for _ in range(2*n)]
        for li in preferences:
            random.shuffle(li)
        # print(n)
        # for li in preferences:
        #     print(li)
        #
        # print("\n\n\n")
        with open("inputs/" + str(n) + ".txt", "w") as f:
            f.write(f"{n}\n")
            for lis in preferences:
                for i in lis:
                    f.write(f"{i} ")
                f.write("\n")

if __name__ == "__main__":
    main()
if __name__ == "__main__":
    with open("temp.txt", "w") as f:
        for x in range(2):
            for y in range(2):
                for z in range(2):
                    for a in range(2):
                        for b in range(2):
                            for c in range(2):
                                counter = x + y + z + a + b + c
                                F = 0
                                if counter >=2 and counter <=4:
                                    F = 1
                                f.write(f"{x}{y}{z}{a}{b}{c}\t{F}\n")
import matplotlib.pyplot as plt
import numpy as np


def main():
    # gather the data from outputs: 1time.txt, 2time.txt ...
    x = [2 ** i for i in range(12)]
    y1 = []
    y2 = []
    y3 = []
    # y4 = []
    for val in x:
        with open(f"outputs/{val}time.txt") as f:
            time_list = [int(line.strip()) for line in f]
            y1.append(time_list[2])
            y2.append(time_list[1])
            y3.append(time_list[0])
            # y4.append(time_list[3])

    coefficients = np.polyfit(x, y1, 2)
    model = np.poly1d(coefficients)
    polyline = np.linspace(min(x), max(x), 48)
    modeled_y1 = model(polyline)
    fig, ax1 = plt.subplots()
    ax1.set_title("Input Size vs. Total Execution Time")
    ax1.set_xlabel("Number of Hospitals/Students")
    ax1.set_ylabel("Time (nanoseconds)")
    # ax.set_xticks(x)
    # ax1.set_xscale("log")
    # ax1.set_yscale("log")
    ax1.plot(x, y1, marker='o', linestyle='-', label='Time Values')
    # ax1.set_xscale("log")
    # ax1.set_yscale("log")
    ax1.plot(polyline, modeled_y1, color='red', label='Quadratic Fit Line')
    ax1.legend()
    # ax2 = ax1.twinx()
    # ax2.set_xscale("log")
    # ax2.set_yscale("log")
    # ax2.plot(x, y2, marker='o', linestyle='-')
    # ax3 = ax1.twinx()
    # ax3.set_xscale("log")
    # ax3.set_yscale("log")
    # ax3.plot(x, y3, marker='o', linestyle='-')
    # ax3 = ax1.twinx()
    # ax3.set_xscale("log")
    # ax3.set_yscale("log")
    # ax3.plot(x, y4, marker='o', linestyle='-')
    plt.show()
    fig.savefig("outputs/chartresultsfit.png")
    fig, ax1 = plt.subplots()
    ax1.set_title("Input Size vs. Total Execution Time")
    ax1.set_xlabel("Number of Hospitals/Students")
    ax1.set_ylabel("Time (nanoseconds)")
    ax1.set_xscale("log")
    ax1.set_yscale("log")
    ax1.plot(x, y1, marker='o', linestyle='-', label='Time Values')
    ax1.plot(polyline, modeled_y1, color='red', label='Quadratic Fit Line')
    ax1.legend()
    plt.show()
    fig.savefig("outputs/chartresultsloglog.png")

    fig, ax2 = plt.subplots()
    coefficients2 = np.polyfit(x, y2, 2)
    model2 = np.poly1d(coefficients2)
    modeled_y2 = model2(polyline)
    ax2.set_title("Input Size vs. Total Verification Time")
    ax2.set_xlabel("Number of Hospitals/Students")
    ax2.set_ylabel("Time (nanoseconds)")
    ax2.set_xscale("log")
    ax2.set_yscale("log")
    ax2.plot(x, y2, marker='o', linestyle='-', label='Time Values')
    ax2.plot(polyline, modeled_y2, color='red', label='Quadratic Fit Line')
    ax2.legend()
    plt.show()
    fig.savefig("outputs/chartresultsverification.png")

    fig, ax3 = plt.subplots()
    coefficients3 = np.polyfit(x, y3, 2)
    model3 = np.poly1d(coefficients3)
    modeled_y3 = model3(polyline)
    ax3.set_title("Input Size vs. Total Matching Time")
    ax3.set_xlabel("Number of Hospitals/Students")
    ax3.set_ylabel("Time (nanoseconds)")
    ax3.set_xscale("log")
    ax3.set_yscale("log")
    ax3.plot(x, y3, marker='o', linestyle='-', label='Time Values')
    ax3.plot(polyline, modeled_y3, color='red', label='Quadratic Fit Line')
    ax3.legend()
    plt.show()
    fig.savefig("outputs/chartresultsmatching.png")

# fig, ax = plt.subplots()
    # ax.set_title("Input Size vs. Total Execution Time")
    # ax.set_xlabel("Number of Hospitals/Students")
    # ax.set_ylabel("Time (nanoseconds)")
    # # ax.set_xticks(x)
    # ax.set_xscale("log")
    # ax.set_yscale("log")
    # ax.plot(x, y2, marker='o', linestyle='-')
    # plt.show()
    # fig.savefig("outputs/charttotal.png")

if __name__ == "__main__":
    main()
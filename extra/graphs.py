# a script that plots the results of the benchmarking

import matplotlib.pyplot as plt
import numpy as np

import re

def convert_to_seconds(time_str):
    # Extract minutes and seconds using regular expression
    match = re.match(r'(\d+)m([\d,]+)s', time_str)

    if match:
        minutes = int(match.group(1))
        seconds = float(match.group(2).replace(',', '.'))  # Replace comma with dot for proper float conversion
        total_seconds = minutes * 60 + seconds
        return total_seconds
    else:
        raise ValueError("Invalid time string format")

def plot_graphs():
    # get the data
    files = ["compress", "extract", "tree"]

    data_dict = {}

    # read csv file with ; as separator
    for file in files:
        file_path = "output/benchmark_results_" + file + ".csv"
        leftist_data = np.genfromtxt(file_path, delimiter=';', dtype=str)

        # Extract file names and corresponding execution times
        file_names = leftist_data[:, 0]
        execution_times = np.array([convert_to_seconds(time_str) for time_str in leftist_data[:, 1]])

        # Plotting
        fig, ax = plt.subplots(figsize=(9, 6))

        # Adjust the width of the bars
        bar_width = 0.8

        # Create bar chart
        bars = ax.bar(file_names, execution_times, width=bar_width)

        # Add text labels on top of each bar
        for bar, exec_time in zip(bars, execution_times):
            height = bar.get_height()
            ax.text(bar.get_x() + bar.get_width() / 2, height, f'{exec_time:.3f} s', ha='center', va='bottom')

        ax.set_ylabel('Execution Time (seconds)')
        ax.set_title(f'Benchmark Results - {file}')

        # Save the plot with the specified filename
        plt.savefig(f'graphs/{file}benchmark.png')

        plt.close()

    # Read data from the first file
    sortData = np.genfromtxt("output/benchmark_results_sort.csv", delimiter=';', dtype=str)

    # Read data from the second file
    sort_bashData = np.genfromtxt("output/benchmark_results_sort_bash.csv", delimiter=';', dtype=str)

    file_names1 = sortData[:, 0]
    execution_times1 = np.array([convert_to_seconds(time_str) for time_str in sortData[:, 1]])

    # Read data from the second file
    file_names2 = sort_bashData[:, 0]
    execution_times2 = np.array([convert_to_seconds(time_str) for time_str in sort_bashData[:, 1]])

    # Plotting
    fig, ax = plt.subplots(figsize=(12, 6))

    bar_width = 0.4  # Adjust the width of the bars
    index = np.arange(len(file_names1))  # Index for the bars

    # Plot bars for the first file
    bars1 = ax.bar(index, execution_times1, bar_width, label='extern sort')

    # Plot bars for the second file next to the first
    bars2 = ax.bar(index + bar_width, execution_times2, bar_width, label='bash sort', alpha=0.7)

    # Add text labels on top of each bar for the first file
    for bar, exec_time in zip(bars1, execution_times1):
        height = bar.get_height()
        ax.text(bar.get_x() + bar.get_width() / 2, height, f'{exec_time:.3f} s', ha='center', va='bottom')

    # Add text labels on top of each bar for the second file
    for bar, exec_time in zip(bars2, execution_times2):
        height = bar.get_height()
        ax.text(bar.get_x() + bar.get_width() / 2, height, f'{exec_time:.3f} s', ha='center', va='bottom')

    ax.set_ylabel('Execution Time (seconds)')
    ax.set_title('Benchmark Results - sort')
    ax.set_xticks(index + bar_width / 2)
    ax.set_xticklabels(file_names1)
    ax.legend()

    # Save the plot with the specified filename
    plt.savefig("graphs/sortbenchmark.png")

    plt.close()



if __name__ == "__main__":
    plot_graphs()